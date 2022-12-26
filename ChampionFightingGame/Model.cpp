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

#include "utils.h"

Model::Model() {
	move = false;
	tpose = false;
	global_transform = glm::mat4(1.0);
	dummy_matrix = nullptr;
	dummy_vec = nullptr;
	dummy_quat = nullptr;
	has_skeleton = false;
	flip_matrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	trans_id = -1;
}

Model::Model(std::string path) {
	load_model(path);
}

Model::Model(Model& other) {
	this->global_transform = other.global_transform;
	this->dummy_matrix = other.dummy_matrix;
	this->dummy_quat = other.dummy_quat;
	this->dummy_vec = other.dummy_vec;
	this->directory = other.directory;
	this->flip_matrix = other.flip_matrix;

	for (int i = 0, max = other.texture_names.size(); i < max; i++) {
		this->texture_names.push_back(other.texture_names[i]);
	}
	this->texture_map = other.texture_map;

	this->materials = other.materials;
	this->material_map = other.material_map;

	for (int i = 0, max = other.meshes.size(); i < max; i++) {
		this->meshes.push_back(other.meshes[i]);
		meshes[i].mat = &materials[get_material_id(meshes[i].mat->name)];
	}
	this->mesh_map = other.mesh_map;

	for (int i = 0, max = other.bones.size(); i < max; i++) {
		this->bones.push_back(other.bones[i]);
	}
	this->bone_map = other.bone_map;
	this->has_skeleton = other.has_skeleton;

	if (this->has_skeleton) {
		this->post_process_skeleton();
	}
}

Model::Model(const Model& other) { 
	this->global_transform = other.global_transform;
	this->dummy_matrix = other.dummy_matrix;
	this->dummy_quat = other.dummy_quat;
	this->dummy_vec = other.dummy_vec;
	this->directory = other.directory;
	this->flip_matrix = other.flip_matrix;

	for (int i = 0, max = other.texture_names.size(); i < max; i++) {
		this->texture_names.push_back(other.texture_names[i]);
	}
	this->texture_map = other.texture_map;

	for (int i = 0, max = other.materials.size(); i < max; i++) {
		this->materials.push_back(other.materials[i]);
	}
	this->material_map = other.material_map;

	for (int i = 0, max = other.meshes.size(); i < max; i++) {
		this->meshes.push_back(other.meshes[i]);
		meshes[i].mat = &materials[get_material_id(meshes[i].mat->name)];
	}
	this->mesh_map = other.mesh_map;

	for (int i = 0, max = other.bones.size(); i < max; i++) {
		this->bones.push_back(other.bones[i]);
	}
	this->bone_map = other.bone_map;
	this->has_skeleton = other.has_skeleton;

	if (this->has_skeleton) {
		this->post_process_skeleton();
	}
}

Model& Model::operator=(Model& other) {
	if (this == &other) {
		return *this;
	}

	this->global_transform = other.global_transform;
	this->dummy_matrix = other.dummy_matrix;
	this->dummy_quat = other.dummy_quat;
	this->dummy_vec = other.dummy_vec;
	this->directory = other.directory;
	this->flip_matrix = other.flip_matrix;
	
	for (int i = 0, max = other.texture_names.size(); i < max; i++) {
		this->texture_names.push_back(other.texture_names[i]);
	}
	this->texture_map = other.texture_map;

	for (int i = 0, max = other.materials.size(); i < max; i++) {
		this->materials.push_back(other.materials[i]);
	}
	this->material_map = other.material_map;

	for (int i = 0, max = other.meshes.size(); i < max; i++) {
		this->meshes.push_back(other.meshes[i]);
		meshes[i].mat = &materials[get_material_id(meshes[i].mat->name)];
	}
	this->mesh_map = other.mesh_map;

	for (int i = 0, max = other.bones.size(); i < max; i++) {
		this->bones.push_back(other.bones[i]);
	}
	this->bone_map = other.bone_map;
	this->has_skeleton = other.has_skeleton;

	if (this->has_skeleton) {
		this->post_process_skeleton();
	}
	return *this;
}

Model& Model::operator=(const Model& other) {
	if (this == &other) {
		return *this;
	}

	this->global_transform = other.global_transform;
	this->dummy_matrix = other.dummy_matrix;
	this->dummy_quat = other.dummy_quat;
	this->dummy_vec = other.dummy_vec;
	this->directory = other.directory;
	this->flip_matrix = other.flip_matrix;

	for (int i = 0, max = other.texture_names.size(); i < max; i++) {
		this->texture_names.push_back(other.texture_names[i]);
	}
	this->texture_map = other.texture_map;

	for (int i = 0, max = other.materials.size(); i < max; i++) {
		this->materials.push_back(other.materials[i]);
	}
	this->material_map = other.material_map;

	for (int i = 0, max = other.meshes.size(); i < max; i++) {
		this->meshes.push_back(other.meshes[i]);
		meshes[i].mat = &materials[get_material_id(meshes[i].mat->name)];
	}
	this->mesh_map = other.mesh_map;

	for (int i = 0, max = other.bones.size(); i < max; i++) {
		this->bones.push_back(other.bones[i]);
	}
	this->bone_map = other.bone_map;
	this->has_skeleton = other.has_skeleton;

	if (this->has_skeleton) {
		this->post_process_skeleton();
	}
	return *this;
}

void Model::init(std::string path) {
	move = false;
	tpose = false;
	dummy_matrix = new glm::mat4(1.0);
	dummy_vec = new glm::vec3(0.0);
	dummy_quat = new glm::quat(1.0, 0.0, 0.0, 0.0);
	flip_matrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		return;
	}

	global_transform = ass_converter(scene->mRootNode->mTransformation.Inverse());

	directory = path.substr(0, path.find_last_of('/')) + "/";
	std::string skeleton_path = directory + "skeleton.smd";
	has_skeleton = load_skeleton(skeleton_path);

	process_start(scene);
	if (has_skeleton) {
		process_skeleton(scene->mRootNode);
		post_process_skeleton();
	}
}

void Model::destroy() {
	delete dummy_matrix;
	delete dummy_vec;
	delete dummy_quat;
	for (auto& mesh : meshes)	{
		glDeleteVertexArrays(1, &mesh.VAO);
		glDeleteBuffers(1, &mesh.VBO);
		glDeleteBuffers(1, &mesh.EBO);
	}
	unload_texture_resources();
}

void Model::load_model(std::string path) {
	*this = ResourceManager::get_instance()->use_model(path);
}

void Model::unload_model() {
	ResourceManager::get_instance()->unuse_model(directory + "model.dae");
}

//Used if there's only one possible texture set (I.E. the stage).
void Model::load_textures() {
	ResourceManager* resource_manager = ResourceManager::get_instance();
	for (int i = 0, max = texture_names.size(); i < max; i++) {
		unsigned int loaded_texture = resource_manager->use_texture(directory + texture_names[i]);
		for (int i2 = 0, max2 = texture_map[texture_names[i]].size(); i2 < max2; i2++) {
			const auto& [mat_index, tex_index] = texture_map[texture_names[i]][i2];
			materials[mat_index].textures[tex_index].id = loaded_texture;
		}
	}
}

//Used for specifying sub-directories from the Model folder (I.E. different textures per alt but the
//same model data)
void Model::load_textures(std::string texture_dir) {
	ResourceManager* resource_manager = ResourceManager::get_instance();
	for (int i = 0, max = texture_names.size(); i < max; i++) {
		unsigned int loaded_texture = resource_manager->use_texture(directory + texture_dir + "/" + texture_names[i]);
		for (int i2 = 0, max2 = texture_map[texture_names[i]].size(); i2 < max2; i2++) {
			const auto& [mat_index, tex_index] = texture_map[texture_names[i]][i2];
			materials[mat_index].textures[tex_index].id = loaded_texture;
		}
	}
}

void Model::unload_textures() {
	ResourceManager* resource_manager = ResourceManager::get_instance();
	for (int i = 0, max = texture_names.size(); i < max; i++) {
		resource_manager->unuse_texture(texture_names[i]);
		for (int i2 = 0, max2 = texture_map[texture_names[i]].size(); i2 < max2; i2++) {
			const auto& [mat_index, tex_index] = texture_map[texture_names[i]][i2];
			materials[mat_index].textures[tex_index].id = 0;
		}
	}
}

void Model::unload_texture_resources() {
	ResourceManager* resource_manager = ResourceManager::get_instance();
	for (int i = 0, max = texture_names.size(); i < max; i++) {
		resource_manager->unload_texture(texture_names[i]);
		for (int i2 = 0, max2 = texture_map[texture_names[i]].size(); i2 < max2; i2++) {
			const auto& [mat_index, tex_index] = texture_map[texture_names[i]][i2];
			materials[mat_index].textures[tex_index].id = 0;
		}
	}
}

void Model::set_move(bool move) {
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
			trans_children[i]->parent_matrix = dummy_matrix;
		}
		else {
			trans_children[i]->parent_matrix = &bones[trans_id].anim_matrix;
		}
	}

	this->move = move;
}

void Model::set_bones(float frame, Animation* anim_kind, bool flip) {
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


			bones[i].anim_matrix = *bones[i].parent_matrix * keyframes[i].anim_matrix;
			bones[bones[i].counterpart_id].final_matrix = flip_matrix * ((bones[i].anim_matrix * flip_matrix) * bones[i].model_flip_matrix * global_transform);

			bones[i].pos = keyframes[i].pos;
			bones[i].rot = keyframes[i].rot;
			
			bones[i].rot.x *= -1.0;
			bones[i].rot.w *= -1.0;

			std::swap(bones[i].pos.x, bones[i].pos.z);
			bones[i].pos.z *= -1.0;
		}
	}
	else {
		for (int i = 0, max = keyframes.size(); i < max; i++) {
			keyframes[i].anim_matrix += (frame - (int)frame) * (next_keyframes[i].anim_matrix - keyframes[i].anim_matrix);
			keyframes[i].pos += (frame - (int)frame) * (next_keyframes[i].pos - keyframes[i].pos);
			keyframes[i].rot += (frame - (int)frame) * (next_keyframes[i].rot - keyframes[i].rot);

			bones[i].anim_matrix = *bones[i].parent_matrix * keyframes[i].anim_matrix;
			bones[i].final_matrix = bones[i].anim_matrix * bones[i].model_matrix * global_transform;

			bones[i].pos = keyframes[i].pos;
			bones[i].rot = keyframes[i].rot;

			std::swap(bones[i].pos.x, bones[i].pos.z);
			bones[i].pos.z *= -1.0;
		}
	}
}

void Model::reset_bones() {
	for (Bone& bone : bones) {
		bone.anim_matrix = glm::mat4(1.0);
		bone.final_matrix = glm::mat4(1.0);
	}
	tpose = true;
}

void Model::render(Shader* shader, bool flip) {
	if (tpose) { //If we don't have any keyframe data and we're facing left, we only apply 1 flip matrix,
		//so we need to change the culling to support it.
		if (flip) {
			glCullFace(GL_FRONT);
			for (size_t i = 0, max = bones.size(); i < max; i++) {
				shader->set_mat4("bone_matrix[0]", flip_matrix, i);
			}
		}
		else {
			glCullFace(GL_BACK);
			for (size_t i = 0, max = bones.size(); i < max; i++) {
				shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
			}
		}
	}
	else { //But if we have keyframe data and face left, we applied 1 flip matrix to each bone to mirror
		//the transformations, then we applied another one to the entire model, so culling is always the same
		glCullFace(GL_BACK);
		for (size_t i = 0, max = bones.size(); i < max; i++) {
			shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
		}
	}

	for (const auto& mesh : meshes) {
		if (mesh.visible) {
			mesh.render();
		}
	}
}

void Model::render_no_texture(Shader* shader, bool flip) {
	if (tpose) {
		if (flip) {
			for (size_t i = 0, max = bones.size(); i < max; i++) {
				shader->set_mat4("bone_matrix[0]", flip_matrix, i);
			}
		}
		else {
			for (size_t i = 0, max = bones.size(); i < max; i++) {
				shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
			}
		}
	}
	else {
		for (size_t i = 0, max = bones.size(); i < max; i++) {
			shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
		}
	}

	for (const auto& mesh: meshes) {
		if (mesh.visible) {
			mesh.render_no_texture();
		}
	}
}

void Model::set_mesh_visibility(std::string mesh_name, bool visibility) {
	int index = get_mesh_id(mesh_name);
	if (index == -1) {
		return;
	}
	else {
		meshes[index].visible = visibility;
	}
}

void Model::set_mesh_mat(std::string mesh_name, std::string mat_name) {
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

int Model::get_mesh_id(std::string mesh_name) {
	if (mesh_map.contains(mesh_name)) {
		return mesh_map[mesh_name];
	}
	return -1;
}

int Model::get_material_id(std::string material_name) {
	if (material_map.contains(material_name)) {
		return material_map[material_name];
	}
	return -1;
}

int Model::get_bone_id(std::string bone_name, bool verbose) {
	if (bone_name == "model-armature") {
		if (bone_map.contains("blender_implicit")) {
			return 0;
		}
		else {
			return -1;
		}
	}
	if (!bone_map.contains(bone_name)) {
		if (verbose) {
			std::cout << "ERROR: Couldn't find " << bone_name << "\n";
		}
		return -1;
	}
	return bone_map[bone_name];
}

bool Model::load_skeleton(std::string path) {
	std::ifstream smd;
	smd.open(path);
	if (smd.fail()) {
		smd.close();
		return false;
	}

	Bone new_bone;
	int bone_id;
	std::string bone_name;
	int parent_id;
	int counterpart_id;
	while (smd >> bone_id) {
		smd >> bone_name >> parent_id >> counterpart_id;
		bone_name = Filter(bone_name, "\""); //Remove the "s from the SMD's bone names
		new_bone.name = bone_name;
		new_bone.id = bone_id;
		new_bone.parent_id = parent_id;
		new_bone.counterpart_id = counterpart_id;
		bones.push_back(new_bone);
		bone_map[bone_name] = bone_id;
	}

	smd.close();
	return true;
}

void Model::process_start(const aiScene* scene) {
	//convert contents of mMaterials[] into materials, updating texture_map
	//load_texture_data handles updates to texture_names
	for (unsigned int mat_index = 0; mat_index < scene->mNumMaterials; mat_index++) {
		aiMaterial* material = scene->mMaterials[mat_index];

		std::vector<ModelTexture> textures;

		std::vector<ModelTexture> diffuseMaps = load_texture_data(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<ModelTexture> specularMaps = load_texture_data(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<ModelTexture> normalMaps = load_texture_data(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<ModelTexture> heightMaps = load_texture_data(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		materials.emplace_back(textures, material->GetName().C_Str());
		material_map[materials[mat_index].name] = mat_index;

		//for each texture in the current material, add ordered pair to that texture to texture_map
		for (int tex = 0, max = materials[mat_index].textures.size(); tex < max; tex++) {
			texture_map[materials[mat_index].textures[tex].filename].push_back({ mat_index, tex });
		}
	}
	process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene) {
	for (int i = 0, max = node->mNumMeshes; i < max; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		int index = meshes.size();
		meshes.push_back(process_mesh(mesh));
		mesh_map[meshes[index].name] = index;
	}
	for (unsigned int i = 0, max = node->mNumChildren; i < max; i++) {
		process_node(node->mChildren[i], scene);
	}
}

Mesh Model::process_mesh(aiMesh* mesh) {
	std::vector<ModelVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<ModelTexture> textures;
	std::vector<Bone> bones = this->bones;

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
		if (bones.size() == 0) {
			std::cout << "ERROR: Model at " << directory << " has bones but skeleton.smd was not found!\n";
			return Mesh(vertices, indices, &materials[mesh->mMaterialIndex], name);
		}

		for (int i = 0; i < mesh->mNumBones; i++) {
			aiBone* ai_bone = mesh->mBones[i];
			int bone_id = get_bone_id(Filter(ai_bone->mName.C_Str(), "model-armature_"), false);

			if (bone_id == -1) {
				std::cout << "ERROR: skeleton.smd at " << directory << " does not match the skeleton for this model!" << "\n";
				continue;
			}

			for (int i2 = 0; i2 < ai_bone->mNumWeights; i2++) {
				int index = ai_bone->mWeights[i2].mVertexId;
				int weight_index = ++vertices[index].num_weights;
				vertices[index].bone_ids[weight_index] = bone_id;
				vertices[index].f_bone_ids[weight_index] = this->bones[bone_id].counterpart_id;
				vertices[index].weights[weight_index] = ai_bone->mWeights[i2].mWeight;
			}
		}
	}

	return Mesh(vertices, indices, &materials[mesh->mMaterialIndex], name);
}
std::vector<ModelTexture> Model::load_texture_data(aiMaterial* mat, aiTextureType type, std::string type_name) {
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

void Model::process_skeleton(aiNode* root) {
	for (int i = 0, max = bones.size(); i < max; i++) {
		aiNode* node;
		if (root->mName.C_Str() == "model-armature_" + bones[i].name) {
			node = root;
		}
		else {
			node = root->FindNode(("model-armature_" + bones[i].name).c_str());
		}
		if (node == nullptr) {
			continue;
		}
		bones[i].anim_matrix = ass_converter(node->mTransformation); //Anim Matrix is the transformation factoring parents
		bones[i].bind_matrix = bones[i].anim_matrix; //Bind Matrix will be in a vacuum
		
		//The Anim Matrix is necessary for calculating the Model Matrix, but we want the local transforms
		//stored somewhere while post-processing the skeleton
	}
	for (int i = 0, max = bones.size(); i < max; i++) {
		if (bones[i].parent_id != -1) {
			bones[i].anim_matrix = bones[bones[i].parent_id].anim_matrix * bones[i].anim_matrix;
		}
		bones[i].model_matrix = glm::inverse(global_transform * bones[i].anim_matrix);
	}
}

void Model::post_process_skeleton() { //Reads through the skeleton, figures out which bone is our
	//base translation bone, figures out all of its children, and sets up the parent and flip matrices
	trans_id = get_bone_id("Trans");
	glm::vec3 decomp_other_v3;
	glm::vec4 decomp_other_v4;
	glm::vec3 scale_vec;

	for (int i = 0, max = bones.size(); i < max; i++) {
		glm::decompose(bones[i].bind_matrix, scale_vec, bones[i].base_rot, bones[i].base_pos, decomp_other_v3, decomp_other_v4);

		if (bones[i].parent_id == -1) {
			bones[i].parent_matrix = dummy_matrix;
		}
		else {
			if (bones[i].parent_id == trans_id) {
				trans_children.push_back(&bones[i]);
			}
			bones[i].parent_matrix = &bones[bones[i].parent_id].anim_matrix;

			bones[i].model_flip_matrix = bones[bones[i].counterpart_id].model_matrix;

			bones[i].base_pos = rotate(bones[i].base_pos, bones[bones[i].parent_id].base_rot);
			bones[i].base_pos += bones[bones[i].parent_id].base_pos;

			bones[i].base_rot = bones[bones[i].parent_id].base_rot * bones[i].base_rot;
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

	init();
}

void Mesh::init() {
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
	for (unsigned int i = 0, max = mat->textures.size(); i < max; i++) {
		glActiveTexture(GL_TEXTURE0 + i + 1);
		glBindTexture(GL_TEXTURE_2D, mat->textures[i].id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::render_no_texture() const {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Material::Material(std::vector<ModelTexture> textures, std::string name) {
	this->textures = textures;
	this->name = name;
}