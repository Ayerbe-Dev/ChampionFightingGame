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

Model::Model() {}

Model::Model(std::string path) {
	load_model(path);
}

Model::~Model() {
	unload_model();
}

void Model::load_model(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_PopulateArmatureData | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

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
	std::vector<std::string> missing_bones;
	for (int i = 0; i < bones.size(); i++) {
		if (bones[i].parent_id == -1) {
			bones[i].parent_matrix = new glm::mat4(1.0);
		}
		else {
			bones[i].parent_matrix = &bones[bones[i].parent_id].anim_matrix;
		}
		if (bones[i].model_matrix == glm::mat4(1.0)) {
			missing_bones.push_back(bones[i].name);
		}
	}

//	find_missing_bones(scene->mRootNode, missing_bones);
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
}

void Model::unload_model() {
	for (int i = 0; i < bones.size(); i++) {
		if (bones[i].parent_id == -1) {
			delete bones[i].parent_matrix;
		}
	}
	for (int i = 0; i < textures_loaded.size(); i++) {
		glDeleteTextures(1, &textures_loaded[i].id);
	}
	for (int i = 0; i < meshes.size(); i++) {
		glDeleteVertexArrays(1, &meshes[i].VAO);
		glDeleteBuffers(1, &meshes[i].VBO);
		glDeleteBuffers(1, &meshes[i].EBO);
	}
}

void Model::set_bones(float frame, Animation* anim_kind) {
	if (anim_kind == nullptr) {
		return reset_bones();
	}

	std::vector<AnimBone> keyframes = anim_kind->keyframes[clamp(0, floorf(frame), anim_kind->keyframes.size() - 1)];
	std::vector<AnimBone> next_keyframes = anim_kind->keyframes[clamp(0, floorf(frame + 1), anim_kind->keyframes.size() - 1)];

	for (int i = 0, max = keyframes.size(); i < max; i++) {
		keyframes[i].anim_matrix += (frame - (int)frame) * (next_keyframes[i].anim_matrix - keyframes[i].anim_matrix);

		bones[i].anim_matrix = *bones[i].parent_matrix * keyframes[i].anim_matrix;
		bones[i].final_matrix = bones[i].anim_matrix * bones[i].model_matrix * global_transform;
	}
}

void Model::reset_bones() {
	for (Bone &bone : bones) {
		bone.anim_matrix = glm::mat4(1.0);
		bone.final_matrix = glm::mat4(1.0);
	}
}

void Model::render(Shader* shader, bool flip) {
	if (flip) {
		glCullFace(GL_FRONT);
		for (int i = 0, max = bones.size(); i < max; i++) {
			shader->set_mat4("bone_matrix[0]", flip_matrix * bones[i].final_matrix, i);
		}
	}
	else {
		glCullFace(GL_BACK);
		for (int i = 0, max = bones.size(); i < max; i++) {
			shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
		}
	}

	for (Mesh &mesh : meshes) {
		if (mesh.visible) {
			mesh.render(shader);
		}
	}
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Model::render_shadow(Shader* shader, bool flip) {
	if (flip) {
		for (int i = 0, max = bones.size(); i < max; i++) {
			shader->set_mat4("bone_matrix[0]", flip_matrix * bones[i].final_matrix, i);
		}
	}
	else {
		for (int i = 0, max = bones.size(); i < max; i++) {
			shader->set_mat4("bone_matrix[0]", bones[i].final_matrix, i);
		}
	}

	for (Mesh& mesh : meshes) {
		if (mesh.visible) {
			mesh.render_shadow(shader);
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
	for (int i = 0; i < meshes.size(); i++) {
		if (meshes[i].name == mesh_name) {
			return i;
		}
	}
	return -1;
}

int Model::get_bone_id(std::string bone_name) {
	if (bone_name == "model-armature") { //DAE refers to this bone is blender_implicit, FBX refers to it as model-armature
		return 0;
	}
	for (int i = 0; i < bones.size(); i++) {
		if (bones[i].name == bone_name) {
			return i;
		}
	}
	std::cout << "ERROR: Couldn't find " << bone_name << "\n";
	return -1;
}

void Model::load_skeleton(std::string path) {
	std::ifstream smd;
	smd.open(path);
	if (smd.fail()) {
		std::cout << "Failed to open SMD!" << "\n";
		smd.close();
		return;
	}

	int bone_id;
	std::string bone_name;
	int parent_id;
	while (smd >> bone_id) {
		smd >> bone_name >> parent_id;
		Bone new_bone;
		bone_name = Filter(bone_name, "\""); //Remove the "s from the SMD's bone names
		new_bone.name = bone_name;
		new_bone.id = bone_id;
		new_bone.parent_id = parent_id;
		bones.push_back(new_bone);
	}

	smd.close();
}
void Model::process_node(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(mesh, scene));
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
			bone.anim_rest_matrix = anim_matrix;
			bone.model_matrix = model_matrix;
			aiVector3D base_pos(0.0, 0.0, 0.0); 
			aiVector3D base_rot(0.0, 0.0, 0.0);
			aiVector3D base_scale(0.0, 0.0, 0.0);
			ai_bone->mOffsetMatrix.Decompose(base_scale, base_rot, base_pos);
			bone.pos.x = base_pos.x;
			bone.pos.y = base_pos.y;
			bone.pos.z = base_pos.z;
			bone.rot.x = base_rot.x;
			bone.rot.y = base_rot.y;
			bone.rot.z = base_rot.z;
			if (bone.rot != glm::quat(0.0, 0.0, 0.0, 0.0)) {
				bone.rot = normalize(bone.rot);
			}
			bone.scale.x = base_scale.x;
			bone.scale.y = base_scale.y;
			bone.scale.z = base_scale.z;

			bone.parent_id = this->bones[bone.id].parent_id;

			for (int i2 = 0; i2 < ai_bone->mNumWeights; i2++) {
				int index = ai_bone->mWeights[i2].mVertexId;
				for (int i3 = 0; i3 < MAX_BONE_INFLUENCE; i3++) {
					if (vertices[index].weights[i3] == 0.0) {
						vertices[index].bone_ids[i3] = bone.id;
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

//Takes a vector of bones that didn't seem to be accounted for and searches through the entire node tree until it finds them
//also rare footage of recursion being cool

//also also it doesn't work because if you iterate through the nodes this way, all mTransformations are in a vacuum and i'm messing up a calculation
//somewhere down the line when it comes to finding anim matrix
bool Model::find_missing_bones(aiNode* node, std::vector<std::string>& bone_names) {
	for (int i = 0; i < bone_names.size(); i++) {
		if (Filter(node->mName.C_Str(), "model-armature_") == bone_names[i]) {
			int index = get_bone_id(bone_names[i]);
			std::cout << bones[index].name << ": " << "\n";
			std::cout << "Old Anim Matrix: " << "\n" << glm::to_string(bones[index].anim_matrix) << "\n";

			bones[index].anim_matrix = ass_converter(node->mTransformation);

			std::cout << "New Anim Matrix: " << "\n" << glm::to_string(bones[index].anim_matrix) << "\n";
			bones[index].anim_rest_matrix = bones[index].anim_matrix;

			std::cout << "Old Model Matrix: " << "\n" << glm::to_string(bones[index].model_matrix) << "\n";
			
			aiMatrix4x4 temp_transform = node->mTransformation.Inverse();
			for (aiNode* parent = node->mParent; parent != nullptr; parent = parent->mParent) {
				temp_transform = parent->mTransformation.Inverse() * temp_transform;
			}

//			bones[index].model_matrix = ass_converter(temp_transform);

			std::cout << "New Model Matrix: " << "\n" << glm::to_string(bones[index].model_matrix) << "\n" << "\n";
			bone_names.erase(bone_names.begin() + i);
			break;
		}
	}
	if (bone_names.size() == 0) {
		return true;
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		if (find_missing_bones(node->mChildren[i], bone_names)) {
			return true;
		}
	}
	return false;
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

[[gnu::always_inline]] inline void Mesh::render(Shader* shader) {
	//These should be set on creation of the shader for optimal performance
	shader->set_int("material.diffuse", 0);
	shader->set_int("material.specular", 1);
	shader->set_int("material.shadow_map", 2);
	//
	
	// this for-loop is probably fine since it is essentially checking if the mesh has textures;
	// the issue is, this is pretty ambiguous in regard to which shaders are being used. 
	// proposal to remove for loop and thus conditionals: move render func to be a virtual func of shader and make child shader classes for each shader program
	// downside: code bulk
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}