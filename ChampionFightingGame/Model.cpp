#include "Model.h"
#include <glm/gtx/string_cast.hpp>
#include <fstream>

#include "Bone.h"
#include "Animation.h"
#include "Shader.h"

#include "GLM Helpers.h"
#include "GLEW Helpers.h"
#include "RenderManager.h"

#include "utils.h"

Model::Model() {
	loaded = false;
}

Model::Model(std::string path) {
	loaded = false;
	load_model(path);
}

Model::~Model() {
	if (loaded) {
		unload_model();
	}
}

void Model::load_model(std::string path) {
	move = false;
	dummy_matrix = new glm::mat4(1.0);

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_PopulateArmatureData | aiProcess_Triangulate | aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		return;
	}

	global_transform = ass_converter(scene->mRootNode->mTransformation.Inverse());

	directory = path.substr(0, path.find_last_of('/')) + "/";
	std::string skeleton_path = directory + "skeleton.smd";
	load_skeleton(skeleton_path); 
	
	//Note: skeleton.smd is just an smd file stripped down to the bone list. Since bones are processed in a specific order during set_bones, I want to
	//populate the model's bone list in order of ID, and I thought this was the best way to do it. 

	process_node(scene->mRootNode, scene);
	post_process_skeleton();
	loaded = true;
}

void Model::load_model_no_skeleton(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_PopulateArmatureData | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		return;
	}

	directory = path.substr(0, path.find_last_of('/')) + "/";

	process_node(scene->mRootNode, scene);
	loaded = true;
}

void Model::unload_model() {
	if (loaded) {
		delete dummy_matrix;
		for (int i = 0; i < textures_loaded.size(); i++) {
			glDeleteTextures(1, &textures_loaded[i].id);
		}
		for (int i = 0; i < meshes.size(); i++) {
			glDeleteVertexArrays(1, &meshes[i].VAO);
			glDeleteBuffers(1, &meshes[i].VBO);
			glDeleteBuffers(1, &meshes[i].EBO);
		}
	}
	loaded = false;
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

			bones[i].anim_matrix = *bones[i].parent_matrix * keyframes[i].anim_matrix;
			bones[bones[i].counterpart_id].final_matrix = flip_matrix * ((bones[i].anim_matrix* flip_matrix)* bones[i].model_flip_matrix* global_transform);
		} 
	}
	else {
		for (int i = 0, max = keyframes.size(); i < max; i++) {
			keyframes[i].anim_matrix += (frame - (int)frame) * (next_keyframes[i].anim_matrix - keyframes[i].anim_matrix);

			bones[i].anim_matrix = *bones[i].parent_matrix * keyframes[i].anim_matrix;
			bones[i].final_matrix = bones[i].anim_matrix * bones[i].model_matrix * global_transform;
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
			for (int i = 0, max = bones.size(); i < max; i++) {
				shader->set_mat4("bone_matrix[0]", flip_matrix, i);
			}
		}
		else {
			glCullFace(GL_BACK);
			for (int i = 0, max = bones.size(); i < max; i++) {
				shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
			}
		}
	}
	else { //But if we have keyframe data and face left, we applied 1 flip matrix to each bone to mirror
		//the transformations, then we applied another one to the entire model, so culling is always the same
		glCullFace(GL_BACK);
		for (int i = 0, max = bones.size(); i < max; i++) {
			shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
		}
	}

	for (Mesh &mesh : meshes) {
		if (mesh.visible) {
			mesh.render();
		}
	}
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Model::render_shadow(Shader* shader, bool flip) {
	if (flip) {
		for (int i = 0, max = bones.size(); i < max; i++) {
			shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
		}
	}
	else {
		for (int i = 0, max = bones.size(); i < max; i++) {
			shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
		}
	}

	for (Mesh& mesh : meshes) {
		if (mesh.visible) {
			mesh.render_shadow();
		}
	}
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
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

int Model::get_mesh_id(std::string mesh_name) {
	std::unordered_map<std::string, int>::const_iterator iterator = mesh_map.find(mesh_name);
	if (iterator == mesh_map.end()) {
		return -1;
	}
	return mesh_map[mesh_name];
}

int Model::get_bone_id(std::string bone_name, bool verbose) {
	if (bone_name == "model-armature") {
		return 0;
	}
	std::unordered_map<std::string, int>::const_iterator iterator = bone_map.find(bone_name);
	if (iterator == bone_map.end()) {
		if (verbose) {
			std::cout << "ERROR: Couldn't find " << bone_name << "\n";
		}
		return -1;
	}
	return bone_map[bone_name];
}

void Model::load_skeleton(std::string path) {
	std::ifstream smd;
	smd.open(path);
	if (smd.fail()) {
		std::cout << "Failed to open SMD!" << "\n";
		smd.close();
		return;
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
}
void Model::process_node(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(mesh, scene));
		mesh_map[meshes[i].name] = i;
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		process_node(node->mChildren[i], scene);
	}
}
Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
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

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<ModelTexture> diffuseMaps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<ModelTexture> specularMaps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<ModelTexture> normalMaps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		std::vector<ModelTexture> heightMaps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	std::string name = mesh->mName.C_Str();

	if (mesh->HasBones()) {
		for (int i = 0; i < mesh->mNumBones; i++) {
			Bone bone;
			aiBone* ai_bone = mesh->mBones[i];
			aiNode* ai_node = ai_bone->mArmature;
			glm::mat4 model_matrix = ass_converter(ai_bone->mOffsetMatrix);
			glm::mat4 anim_matrix = ass_converter(ai_node->mTransformation);

			bone.name = Filter(ai_bone->mName.C_Str(), "model-armature_");

			bone.id = get_bone_id(bone.name);
			if (bone.id == -1) {
				std::cout << "ERROR: skeleton.smd at " << directory << " does not match the skeleton for this model!" << "\n";
			}

			bone.anim_matrix = anim_matrix;
			bone.model_matrix = model_matrix;
			bone.parent_id = this->bones[bone.id].parent_id;
			bone.counterpart_id = this->bones[bone.id].counterpart_id;

			for (int i2 = 0; i2 < ai_bone->mNumWeights; i2++) {
				int index = ai_bone->mWeights[i2].mVertexId;
				for (int i3 = 0; i3 < MAX_BONE_INFLUENCE; i3++) {
					if (vertices[index].weights[i3] == 0.0) {
						vertices[index].bone_ids[i3] = bone.id;
						vertices[index].f_bone_ids[i3] = bone.counterpart_id;
						vertices[index].weights[i3] = ai_bone->mWeights[i2].mWeight;
						break;
					}
				}
			}

			this->bones[bone.id] = bone;
			
		}
	}

	return Mesh(vertices, indices, textures, name);
}
std::vector<ModelTexture> Model::load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name) {
	std::vector<ModelTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int i2 = 0; i2 < textures_loaded.size(); i2++) {
			if (std::strcmp(textures_loaded[i2].path.data(), str.C_Str()) == 0) {
				ModelTexture texture = textures_loaded[i2];
				texture.type_string = type_name + std::to_string(i + 1);
				textures.push_back(texture);
				skip = true;
				break;
			}
		}
		if (!skip) {
			ModelTexture texture;
			texture.id = loadGLTextureFromFile(str.C_Str(), directory);
			texture.path = str.C_Str();
			texture.type_string = type_name + std::to_string(i + 1);
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

void Model::post_process_skeleton() { //Reads through the skeleton, figures out which bone is our
	//base translation bone, figures out all of its children, and sets up the parent and flip matrices
	trans_id = get_bone_id("Trans");
	for (int i = 0, max = bones.size(); i < max; i++) {
		if (bones[i].parent_id == -1) {
			bones[i].parent_matrix = dummy_matrix;
		}
		else {
			if (bones[i].parent_id == trans_id) {
				trans_children.push_back(&bones[i]);
			}
			bones[i].parent_matrix = &bones[bones[i].parent_id].anim_matrix;
		}
		bones[i].model_flip_matrix = bones[bones[i].counterpart_id].model_matrix;
	}
}

Mesh::Mesh(std::vector<ModelVertex> vertices, std::vector<unsigned int> indices, std::vector<ModelTexture> textures, std::string name) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
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
}

FORCE_INLINE void Mesh::render() {
	for (unsigned int i = 0, max = textures.size(); i < max; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

FORCE_INLINE void Mesh::render_shadow() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}