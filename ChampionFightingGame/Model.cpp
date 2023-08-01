#include "Model.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <fstream>

#include "Bone.h"
#include "Animation.h"
#include "Shader.h"

#include "GLM Helpers.h"
#include "GLEW Helpers.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "ThreadManager.h"

#include "utils.h"

ModelData::ModelData() {
	flip_matrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	dummy_matrix = nullptr;
	dummy_vec = nullptr;
	dummy_quat = nullptr;
	global_transform = glm::mat4(0.0);
	directory = "";
	trans_id = -1;
	skeleton_loaded = false;
}

ModelData::ModelData(std::string path) {
	load_model(path);
}

void ModelData::load_model(std::string path) {
	dummy_matrix = new glm::mat4(1.0);
	dummy_vec = new glm::vec3(0.0);
	dummy_quat = new glm::quat(1.0, 0.0, 0.0, 0.0);

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		return;
	}

	global_transform = ass_converter(scene->mRootNode->mTransformation.Inverse());

	directory = path.substr(0, path.find_last_of('/')) + "/";
	std::string skeleton_path = directory.substr(0, directory.find_last_of('m')) + "skeleton.smd";
	skeleton_loaded = skeleton.load_skeleton(skeleton_path);

	process_scene(scene);
	if (skeleton_loaded) {
		process_skeleton(scene->mRootNode);
	}
}

void ModelData::unload_model() {
	delete dummy_matrix;
	delete dummy_vec;
	delete dummy_quat;
	for (auto& mesh : mesh_data) {
		glDeleteVertexArrays(1, &mesh.VAO);
		glDeleteBuffers(1, &mesh.VBO);
		glDeleteBuffers(1, &mesh.EBO);
	}
}

//This function is to be called from the main thread on models which were loaded in a 
//separate thread.
void ModelData::init_gl_meshes() {
	for (auto& mesh : mesh_data) {
		mesh.init_gl();
	}
}

glm::mat4 ModelData::get_global_transform() const {
	return global_transform;
}

std::string ModelData::get_directory() const {
	return directory;
}

std::size_t ModelData::get_trans_id() const {
	return trans_id;
}

bool ModelData::has_skeleton() const {
	return skeleton_loaded;
}

void ModelData::process_scene(const aiScene* scene) {
	//convert contents of mMaterials[] into materials, updating texture_map
	//load_texture_data handles updates to texture_names
	for (unsigned int mat_index = 0; mat_index < scene->mNumMaterials; mat_index++) {
		aiMaterial* material = scene->mMaterials[mat_index];

		std::vector<ModelTexture> textures;

		std::vector<ModelTexture> diffuseMaps = load_texture_names(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<ModelTexture> specularMaps = load_texture_names(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<ModelTexture> normalMaps = load_texture_names(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<ModelTexture> heightMaps = load_texture_names(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		material_data.emplace_back(textures, material->GetName().C_Str());
		material_map[material_data[mat_index].name] = mat_index;

		//for each texture in the current material, add ordered pair to that texture to texture_map
		for (int tex = 0, max = material_data[mat_index].textures.size(); tex < max; tex++) {
			texture_map[material_data[mat_index].textures[tex].filename].push_back({ mat_index, tex });
		}
	}
	process_node(scene->mRootNode, scene);
}

void ModelData::process_node(aiNode* node, const aiScene* scene) {
	for (int i = 0, max = node->mNumMeshes; i < max; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		int index = mesh_data.size();
		mesh_data.push_back(process_mesh(mesh));
		mesh_map[mesh_data[index].name] = index;
	}
	for (unsigned int i = 0, max = node->mNumChildren; i < max; i++) {
		process_node(node->mChildren[i], scene);
	}
}

Mesh ModelData::process_mesh(aiMesh* mesh) {
	std::vector<ModelVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ModelTexture> textures;
	std::vector<Bone> bone_data = skeleton.bone_data;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		ModelVertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		if (mesh->HasTangentsAndBitangents()) {
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.tangent = vector;
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.bitangent = vector;
		}

		if (mesh->mTextureCoords[0]) {
			//Assimp allows for a texture to have up to 8 texture coords per vertex. The guide I'm following says we'll only need to take a look at 
			//the first one, but if we ever want/need to change that, here's where that'd be.
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_coords = vec;
		}
		else {
			vertex.tex_coords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int i2 = 0; i2 < face.mNumIndices; i2++) {
			indices.push_back(face.mIndices[i2]);
		}
	}

	std::string name = mesh->mName.C_Str();

	if (mesh->HasBones()) {
		if (bone_data.size() == 0) {
			std::cout << "ERROR: Model at " << directory << " has bones but skeleton.smd was not found!\n";
			return Mesh(vertices, indices, &material_data[mesh->mMaterialIndex], name);
		}

		for (int i = 0; i < mesh->mNumBones; i++) {
			aiBone* ai_bone = mesh->mBones[i];
			int bone_id = -1;
			std::string bone_name = Filter(ai_bone->mName.C_Str(), "model-armature_");
			if (skeleton.bone_map.contains(bone_name)) {
				bone_id = skeleton.bone_map[bone_name];
			}

			if (bone_id == -1) {
				std::cout << "ERROR: skeleton.smd at " << directory << " does not match the skeleton for this model!" << "\n";
				continue;
			}

			for (int i2 = 0; i2 < ai_bone->mNumWeights; i2++) {
				int index = ai_bone->mWeights[i2].mVertexId;
				int weight_index = ++vertices[index].num_weights;
				vertices[index].bone_ids[weight_index] = bone_id;
				vertices[index].f_bone_ids[weight_index] = skeleton.bone_data[bone_id].counterpart_id;
				vertices[index].weights[weight_index] = ai_bone->mWeights[i2].mWeight;
			}
		}
	}

	return Mesh(vertices, indices, &material_data[mesh->mMaterialIndex], name);
}

std::vector<ModelTexture> ModelData::load_texture_names(aiMaterial* mat, aiTextureType type, std::string type_name) {
	std::vector<ModelTexture> textures;
	for (unsigned int i = 0, max = mat->GetTextureCount(type); i < max; i++) {
		aiString ai_filename;
		mat->GetTexture(type, i, &ai_filename);
		std::string filename = ai_filename.C_Str();
		bool new_texture = true;
		for (int i2 = 0, max2 = texture_names.size(); i2 < max2; i2++) {
			if (texture_names[i2] == filename) {
				new_texture = false;
			}
		}
		if (new_texture) {
			texture_names.push_back(filename);
		}
		ModelTexture texture;
		texture.filename = filename;
		texture.type_string = type_name + std::to_string(i + 1);
		textures.push_back(texture);
	}
	return textures;
}

void ModelData::process_skeleton(aiNode* root) {
	std::vector<Bone>& bone_data = skeleton.bone_data;
	for (int i = 0, max = bone_data.size(); i < max; i++) {
		aiNode* node;
		if (root->mName.C_Str() == "model-armature_" + bone_data[i].name) {
			node = root;
		}
		else {
			node = root->FindNode(("model-armature_" + bone_data[i].name).c_str());
		}
		if (node == nullptr) {
			continue;
		}
		bone_data[i].anim_matrix = ass_converter(node->mTransformation); //Anim Matrix is the transformation factoring parents
		bone_data[i].bind_matrix = bone_data[i].anim_matrix; //Bind Matrix will be in a vacuum

		//The Anim Matrix is necessary for calculating the Model Matrix, but we want the local transforms
		//stored somewhere while post-processing the skeleton
	}
	for (int i = 0, max = bone_data.size(); i < max; i++) {
		if (bone_data[i].parent_id != -1) {
			bone_data[i].anim_matrix = bone_data[bone_data[i].parent_id].anim_matrix * bone_data[i].anim_matrix;
		}
		bone_data[i].model_matrix = glm::inverse(global_transform * bone_data[i].anim_matrix);
	}
	//Reads through the skeleton, figures out which bone is our base translation bone, figures 
	//out all of its children, and sets up the parent and flip matrices
	if (skeleton.bone_map.contains("Trans")) {
		trans_id = skeleton.bone_map["Trans"];
	}
	glm::vec3 decomp_other_v3;
	glm::vec4 decomp_other_v4;
	glm::vec3 scale_vec;

	for (int i = 0, max = bone_data.size(); i < max; i++) {
		glm::decompose(bone_data[i].bind_matrix, scale_vec, bone_data[i].base_rot, bone_data[i].base_pos, decomp_other_v3, decomp_other_v4);

		if (bone_data[i].parent_id != -1) {
			bone_data[i].model_flip_matrix = bone_data[bone_data[i].counterpart_id].model_matrix;

			bone_data[i].base_pos = rotate(bone_data[i].base_pos, bone_data[bone_data[i].parent_id].base_rot);
			bone_data[i].base_pos += bone_data[bone_data[i].parent_id].base_pos;

			bone_data[i].base_rot = bone_data[bone_data[i].parent_id].base_rot * bone_data[i].base_rot;
		}
	}
}

ModelInstance::ModelInstance() {
	texture_dir = "";
	model = nullptr;
	move = false;
	tpose = false;
}

ModelInstance::ModelInstance(std::string path) {
	load_model_instance(path);
}

ModelInstance::~ModelInstance() {
	unload_model_instance();
}

void ModelInstance::load_model_instance(std::string path) {
	move = false;
	tpose = false;
	texture_dir = "";
	
	model = ResourceManager::get_instance()->get_model(path);

	for (int i = 0, max = model->material_data.size(); i < max; i++) {
		materials.push_back(model->material_data[i]);
	}
	for (int i = 0, max = model->mesh_data.size(); i < max; i++) {
		meshes.push_back(model->mesh_data[i]);
		meshes[i].mat = &materials[get_material_id(meshes[i].mat->name)];
	}
	for (int i = 0, max = model->skeleton.bone_data.size(); i < max; i++) {
		bone_data.push_back(model->skeleton.bone_data[i]);
	}
	process_skeleton_instance();
}

void ModelInstance::load_used_model_instance(std::string path) {
	move = false;
	tpose = false;
	texture_dir = "";

	model = ResourceManager::get_instance()->get_model_keep_user_count(path);

	for (int i = 0, max = model->material_data.size(); i < max; i++) {
		materials.push_back(model->material_data[i]);
	}
	for (int i = 0, max = model->mesh_data.size(); i < max; i++) {
		meshes.push_back(model->mesh_data[i]);
		meshes[i].mat = &materials[get_material_id(meshes[i].mat->name)];
	}
	for (int i = 0, max = model->skeleton.bone_data.size(); i < max; i++) {
		bone_data.push_back(model->skeleton.bone_data[i]);
	}
	process_skeleton_instance();
}

void ModelInstance::unload_model_instance() {
	if (model != nullptr) {
		ResourceManager::get_instance()->unuse_model(model->get_directory() + "model.dae");
		unload_textures();
		materials.clear();
		meshes.clear();
		bone_data.clear();
		trans_children.clear();
		model = nullptr;
	}
}

void ModelInstance::set_move(bool move) {
	//Basically: If an animation uses the "move" flag, that indicates that the
	//trans bone movement should be applied to the player's actual position
	//instead of to the animation. 

	//For this to work, we still need to know the position of the trans bone, but
	//we don't want it to influence any of the other bones in the skeleton, or the
	//movement will be doubled. So for all bones which have the trans bone as their
	//parent, we want to set their parent matrix to be the dummy one instead of the
	//trans bone matrix. 

	//That being said, we don't want to do this every frame, so we use this function
	//to properly set the children of trans' parent matrices right at the start of an
	//animation. 
	if (this->move == move) {
		return;
	}

	for (int i = 0, max = trans_children.size(); i < max; i++) {
		if (move) {
			trans_children[i]->parent_matrix = model->dummy_matrix;
		}
		else {
			trans_children[i]->parent_matrix = &bone_data[model->get_trans_id()].anim_matrix;
		}
	}

	this->move = move;
}

void ModelInstance::set_bones(float frame, Animation* anim_kind, bool flip) {
	if (anim_kind == nullptr) {
		return reset_bones();
	}
	tpose = false;

	std::vector<AnimBone> keyframes = anim_kind->keyframes[clamp(0, floorf(frame), anim_kind->keyframes.size() - 1)];
	std::vector<AnimBone> next_keyframes = anim_kind->keyframes[clamp(0, floorf(frame + 1), anim_kind->keyframes.size() - 1)];

	if (flip) {
		for (int i = 0, max = keyframes.size(); i < max; i++) {
			keyframes[i].anim_matrix += (frame - (int)frame) * (next_keyframes[i].anim_matrix - keyframes[i].anim_matrix);
			keyframes[i].pos += (frame - (int)frame) * (next_keyframes[i].pos - keyframes[i].pos);
			keyframes[i].rot += (frame - (int)frame) * (next_keyframes[i].rot - keyframes[i].rot);


			bone_data[i].anim_matrix = *bone_data[i].parent_matrix * keyframes[i].anim_matrix;
			bone_data[bone_data[i].counterpart_id].final_matrix = model->flip_matrix * ((bone_data[i].anim_matrix * model->flip_matrix) * bone_data[i].model_flip_matrix * model->get_global_transform());

			bone_data[i].pos = keyframes[i].pos;
			bone_data[i].rot = keyframes[i].rot;

			bone_data[i].rot.x *= -1.0;
			bone_data[i].rot.w *= -1.0;

			std::swap(bone_data[i].pos.x, bone_data[i].pos.z);
			bone_data[i].pos.z *= -1.0;
		}
	}
	else {
		for (int i = 0, max = keyframes.size(); i < max; i++) {
			keyframes[i].anim_matrix += (frame - (int)frame) * (next_keyframes[i].anim_matrix - keyframes[i].anim_matrix);
			keyframes[i].pos += (frame - (int)frame) * (next_keyframes[i].pos - keyframes[i].pos);
			keyframes[i].rot += (frame - (int)frame) * (next_keyframes[i].rot - keyframes[i].rot);

			bone_data[i].anim_matrix = *bone_data[i].parent_matrix * keyframes[i].anim_matrix;
			bone_data[i].final_matrix = bone_data[i].anim_matrix * bone_data[i].model_matrix * model->get_global_transform();

			bone_data[i].pos = keyframes[i].pos;
			bone_data[i].rot = keyframes[i].rot;

			std::swap(bone_data[i].pos.x, bone_data[i].pos.z);
			bone_data[i].pos.z *= -1.0;
		}
	}
}

void ModelInstance::reset_bones() {
	for (Bone& bone : bone_data) {
		bone.anim_matrix = glm::mat4(1.0);
		bone.final_matrix = glm::mat4(1.0);
	}
	tpose = true;
}

void ModelInstance::load_textures() {
	ResourceManager* resource_manager = ResourceManager::get_instance();
	std::vector<std::string> &texture_names = model->texture_names;
	std::unordered_map<std::string, std::vector<std::pair<int, int>>>& texture_map = model->texture_map;

	for (int i = 0, max = texture_names.size(); i < max; i++) {
		unsigned int loaded_texture = resource_manager->get_texture(model->get_directory() + texture_names[i]);
		for (int i2 = 0, max2 = texture_map[texture_names[i]].size(); i2 < max2; i2++) {
			const auto& [mat_index, tex_index] = texture_map[texture_names[i]][i2];
			materials[mat_index].textures[tex_index].id = loaded_texture;
		}
	}
}

void ModelInstance::load_textures(std::string path) {
	ResourceManager* resource_manager = ResourceManager::get_instance();
	std::vector<std::string>& texture_names = model->texture_names;
	std::unordered_map<std::string, std::vector<std::pair<int, int>>>& texture_map = model->texture_map;

	texture_dir = model->get_directory() + path + "/";
	for (int i = 0, max = texture_names.size(); i < max; i++) {
		unsigned int loaded_texture = resource_manager->get_texture(texture_dir + texture_names[i]);
		for (int i2 = 0, max2 = texture_map[texture_names[i]].size(); i2 < max2; i2++) {
			const auto& [mat_index, tex_index] = texture_map[texture_names[i]][i2];
			materials[mat_index].textures[tex_index].id = loaded_texture;
		}
	}
}

void ModelInstance::unload_textures() {
	ResourceManager* resource_manager = ResourceManager::get_instance();
	std::vector<std::string>& texture_names = model->texture_names;
	std::unordered_map<std::string, std::vector<std::pair<int, int>>>& texture_map = model->texture_map;
	for (int i = 0, max = texture_names.size(); i < max; i++) {
		resource_manager->unuse_texture(texture_dir + texture_names[i]);
		for (int i2 = 0, max2 = texture_map[texture_names[i]].size(); i2 < max2; i2++) {
			const auto& [mat_index, tex_index] = texture_map[texture_names[i]][i2];
			materials[mat_index].textures[tex_index].id = 0;
		}
	}
	texture_dir = "";
}

void ModelInstance::set_mesh_visibility(std::string mesh_name, bool visibility) {
	int index = get_mesh_id(mesh_name);
	if (index == -1) {
		return;
	}
	else {
		meshes[index].visible = visibility;
	}
}

void ModelInstance::set_mesh_mat(std::string mesh_name, std::string mat_name) {
	int mesh_index = get_mesh_id(mesh_name);
	if (mesh_index == -1) {
		return;
	}
	else {
		int mat_index = get_material_id(mat_name);
		if (mat_index == -1) {
			return;
		}
		else {
			meshes[mesh_index].mat = &materials[mat_index];
		}
	}
}

int ModelInstance::get_mesh_id(std::string mesh_name) {
	if (model->mesh_map.contains(mesh_name)) {
		return model->mesh_map[mesh_name];
	}
	return -1;
}

int ModelInstance::get_material_id(std::string material_name) {
	if (model->material_map.contains(material_name)) {
		return model->material_map[material_name];
	}
	return -1;
}

int ModelInstance::get_bone_id(std::string bone_name) const {
	return model->skeleton.get_bone_id(bone_name);
}

Skeleton ModelInstance::get_skeleton() const {
	return model->skeleton;
}

bool ModelInstance::has_skeleton() {
	return model->has_skeleton();
}

bool ModelInstance::is_loaded() const {
	return model != nullptr;
}

void ModelInstance::render(Shader* shader, bool flip) {
	shader->set_active_uniform_location("bone_matrix[0]");
	if (tpose) { //If we don't have any keyframe data and we're facing left, we only apply 1 flip matrix,
		//so we need to change the culling to support it.
		if (flip) {
			glCullFace(GL_FRONT);
			for (size_t i = 0, max = bone_data.size(); i < max; i++) {
				shader->set_active_mat4(model->flip_matrix, i);
			}
		}
		else {
			glCullFace(GL_BACK);
			for (size_t i = 0, max = bone_data.size(); i < max; i++) {
				shader->set_active_mat4(bone_data[i].final_matrix, i);
			}
		}
	}
	else { 
		//But if we have keyframe data and face left, we applied 1 flip matrix to each bone to mirror
		//the transformations, then we applied another one to the entire model, so culling is always the
		//same
		glCullFace(GL_BACK);
		for (size_t i = 0, max = bone_data.size(); i < max; i++) {
			shader->set_active_mat4(bone_data[i].final_matrix, i);
		}
	}

	for (const auto& mesh : meshes) {
		if (mesh.visible) {
			mesh.render();
		}
	}
}

void ModelInstance::render_no_texture(Shader* shader, bool flip) {
	shader->set_active_uniform_location("bone_matrix[0]");
	if (tpose) {
		if (flip) {
			glCullFace(GL_FRONT);
			for (size_t i = 0, max = bone_data.size(); i < max; i++) {
				shader->set_active_mat4(model->flip_matrix, i);
			}
		}
		else {
			glCullFace(GL_BACK);
			for (size_t i = 0, max = bone_data.size(); i < max; i++) {
				shader->set_active_mat4(bone_data[i].final_matrix, i);
			}
		}
	}
	else { 
		glCullFace(GL_BACK);
		for (size_t i = 0, max = bone_data.size(); i < max; i++) {
			shader->set_active_mat4(bone_data[i].final_matrix, i);
		}
	}

	for (const auto& mesh : meshes) {
		if (mesh.visible) {
			mesh.render_no_texture();
		}
	}
}

void ModelInstance::process_skeleton_instance() {		
	for (int i = 0, max = bone_data.size(); i < max; i++) {
		if (bone_data[i].parent_id == -1) {
			bone_data[i].parent_matrix = model->dummy_matrix;
		}
		else {
			bone_data[i].parent_matrix = &bone_data[bone_data[i].parent_id].anim_matrix;
			if (bone_data[i].parent_id == model->get_trans_id()) {
				trans_children.push_back(&bone_data[i]);
			}
		}
	}
}

Mesh::Mesh() {
	visible = true;
	VAO = 0;
	VBO = 0;
	EBO = 0;
	mat = nullptr;
	shader = nullptr;
}

Mesh::Mesh(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices, Material* mat, std::string name) {
	this->vertices = vertices;
	this->indices = indices;
	this->mat = mat;
	this->name = name;

	init_gl();
}

void Mesh::init_gl() {
	if (ThreadManager::get_instance()->is_main_thread()) {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ModelVertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, tex_coords));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, tangent));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, bitangent));
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, MAX_BONE_INFLUENCE, GL_INT, sizeof(ModelVertex), (void*)offsetof(ModelVertex, bone_ids));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, weights));
		glBindVertexArray(0);
		visible = true;
	}
}

Mesh::Mesh(Mesh& other) {
	for (int i = 0, max = other.vertices.size(); i < max; i++) {
		this->vertices.push_back(other.vertices[i]);
	}
	for (int i = 0, max = other.indices.size(); i < max; i++) {
		this->indices.push_back(other.indices[i]);
	}

	this->mat = other.mat;
	this->shader = other.shader;
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	this->EBO = other.EBO;
	this->name = other.name;
	this->visible = other.visible;
}

Mesh::Mesh(const Mesh& other) {
	for (int i = 0, max = other.vertices.size(); i < max; i++) {
		this->vertices.push_back(other.vertices[i]);
	}
	for (int i = 0, max = other.indices.size(); i < max; i++) {
		this->indices.push_back(other.indices[i]);
	}

	this->mat = other.mat;
	this->shader = other.shader;
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	this->EBO = other.EBO;
	this->name = other.name;
	this->visible = other.visible;
}

Mesh Mesh::operator=(Mesh& other) {
	if (this == &other) {
		return *this;
	}

	for (int i = 0, max = other.vertices.size(); i < max; i++) {
		this->vertices.push_back(other.vertices[i]);
	}
	for (int i = 0, max = other.indices.size(); i < max; i++) {
		this->indices.push_back(other.indices[i]);
	}

	this->mat = other.mat;
	this->shader = other.shader;
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	this->EBO = other.EBO;
	this->name = other.name;
	this->visible = other.visible;

	return *this;
}

Mesh Mesh::operator=(const Mesh& other) {
	if (this == &other) {
		return *this;
	}

	for (int i = 0, max = other.vertices.size(); i < max; i++) {
		this->vertices.push_back(other.vertices[i]);
	}
	for (int i = 0, max = other.indices.size(); i < max; i++) {
		this->indices.push_back(other.indices[i]);
	}

	this->mat = other.mat;
	this->shader = other.shader;
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	this->EBO = other.EBO;
	this->name = other.name;
	this->visible = other.visible;

	return *this;
}

void Mesh::render() const {
	if (this == nullptr) return;
	for (unsigned int i = 0, max = mat->textures.size(); i < max; i++) {
		glActiveTexture(GL_TEXTURE0 + i + 1);
		glBindTexture(GL_TEXTURE_2D, mat->textures[i].id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::render_no_texture() const {
	if (this == nullptr) return;
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Material::Material(std::vector<ModelTexture> textures, std::string name) {
	this->textures = textures;
	this->name = name;
}