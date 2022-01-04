#include "Model.h"
using namespace glm;

Mesh::Mesh(vector<Vertex> vertices, vector<uint> indices, vector<Texture> textures, string name) {
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

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, MAX_BONE_INFLUENCE, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bone_ids));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weights));
	glBindVertexArray(0);
}

void Mesh::render(Shader *shader) {
    unsigned int diffuse_count = 1;
    unsigned int specular_count = 1;
	unsigned int normal_count = 1;
	unsigned int height_count = 1;
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse") {
			number = to_string(diffuse_count++);
		}
		else if (name == "texture_specular") {
			number = to_string(specular_count++);
		}
		else if (name == "texture_normal") {
			number = to_string(normal_count++);
		}
		else if (name == "texture_height") {
			number = to_string(height_count++);
		}

		shader->set_float(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

Model::Model(string path) {
	load_model(path);
}

int Model::get_mesh_id(string mesh_name) {
	for (int i = 0; i < meshes.size(); i++) {
		if (meshes[i].name == mesh_name) {
			return i;
		}
	}
	return -1;
}

int Model::get_bone_id(string bone_name) {
	for (int i = 0; i < bones.size(); i++) {
		if (bones[i].name == bone_name) {
			return i;
		}
	}
	return -1;
}

void Model::load_skeleton(string path) {
	ifstream smd;
	smd.open(path);
	if (smd.fail()) {
		cout << "Failed to open SMD!" << endl;
		smd.close();
		return;
	}

	int bone_id;
	string bone_name;
	int parent_id;
	while (smd >> bone_id) {
		smd >> bone_name >> parent_id;
		Bone new_bone;
		bone_name = Filter(Filter(bone_name, "\""), "\""); //Remove the "s from the SMD's bone names
		new_bone.name = bone_name;
		new_bone.id = bone_id;
		new_bone.parent_id = parent_id;
		bones.push_back(new_bone);
	}

	smd.close();
	bones_anim = bones;
}

void Model::load_model(string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/')) + "/";
	string skeleton_path = directory + "skeleton.smd";
	load_skeleton(skeleton_path);

	process_node(scene->mRootNode, scene);

	bones_anim = bones;
}

void Model::reset_bones() {
	bones_anim = bones;
}

void Model::render(Shader* shader) {
	for (int i = 0; i < bones_anim.size(); i++) {
		if (bones[i].pos == bones_anim[i].pos
		&& bones[i].rot == bones_anim[i].pos
		&& bones[i].scale == bones_anim[i].pos) {
			continue;
		}
		string final_mat = "bone_matrix[" + to_string(i) + "]";
		mat4 model_mat = mat4(1.0);
		mat4 pos_mat = translate(model_mat, bones_anim[i].pos - bones[i].pos);
		mat4 rot_mat = orientate4(bones_anim[i].rot - bones[i].rot);
		mat4 scale_mat = scale(model_mat, bones_anim[i].scale - bones[i].scale);
		model_mat *= rot_mat * pos_mat * scale_mat;
		shader->set_mat4(final_mat, model_mat);
//		shader->set_mat4(final_mat, bones_anim[i].matrix);
	}
	//1-2 ms even with the function skipping over every single bone

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].render(shader);
	}
	//1-2 ms but everything in there is necessary
}

void Model::set_bones(float frame, Animation3D *anim_kind) {
	reset_bones();
	int frame_index = clamp(0, floorf(frame), anim_kind->keyframes.size() - 1); //All keyframes for the current frame
	int next_frame_index = clamp(0, floorf(frame + 1), anim_kind->keyframes.size() - 1); //All keyframes for the next frame
	vector<Bone> keyframes = anim_kind->keyframes[frame_index];
	vector<Bone> next_keyframes = anim_kind->keyframes[next_frame_index];

	for (int i = 0; i < keyframes.size(); i++) { //Iterate through all bones
		Bone curr_frame_offsets = keyframes[i]; //Get the bone offsets for the current bone on this frame
		Bone next_frame_offsets = next_keyframes[i]; //Do the same for the next frame
		float decimal = (float)frame_index - frame; //Decimal place for the next frame

		//Calculate the difference in offset between the current frame and the next frame, then multiply said difference by the decimal place. This will
		//allow us to interpolate between non-integer keyframes, so if our frame is 3.5, for example, a bone will be halfway between its frame 3 and 4
		//keyframes

		next_frame_offsets.pos = decimal * (next_frame_offsets.pos - curr_frame_offsets.pos);
		next_frame_offsets.rot = decimal * (next_frame_offsets.rot - curr_frame_offsets.rot);
		next_frame_offsets.scale = decimal * (next_frame_offsets.scale - curr_frame_offsets.scale);
		next_frame_offsets.matrix = decimal * (next_frame_offsets.matrix - curr_frame_offsets.matrix);
		
		curr_frame_offsets.pos += next_frame_offsets.pos;
		curr_frame_offsets.rot += next_frame_offsets.rot;
		curr_frame_offsets.scale += next_frame_offsets.scale;
		curr_frame_offsets.matrix += next_frame_offsets.matrix;

		//If we have a parent bone, we also want to add in the offsets from that parent bone. Since a bone will never be at a lower index than its
		//parent, we don't need to worry about populating the entire bone vector before making this calc, the spot we're actually looking at is going
		//to be filled by the time we look at it

		if (curr_frame_offsets.parent_id != -1) {
			Bone parent_offsets = bones_anim[curr_frame_offsets.parent_id]; //Get the parent bone coords
			offset_base_bone(&bones[curr_frame_offsets.parent_id], &parent_offsets); //Subtract the base bone coords from the parent ones since we already factor them into the child
	
			curr_frame_offsets.pos += parent_offsets.pos;
			curr_frame_offsets.rot += parent_offsets.rot;
			curr_frame_offsets.scale += parent_offsets.scale;
			curr_frame_offsets.matrix = parent_offsets.matrix * curr_frame_offsets.matrix;
		}

		bones_anim[i].pos += curr_frame_offsets.pos;
		bones_anim[i].rot += curr_frame_offsets.rot;
		bones_anim[i].scale += curr_frame_offsets.scale;
		bones_anim[i].matrix *= curr_frame_offsets.matrix;
	}
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
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	vector<Bone> bones = this->bones;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vec3 vector;
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
			vec2 vec;
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
		vector<Texture> diffuseMaps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		vector<Texture> normalMaps = load_material_textures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		vector<Texture> heightMaps = load_material_textures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	string name = mesh->mName.C_Str();

	if (mesh->HasBones()) { //Check if the mesh has any bones
		for (int i = 0; i < mesh->mNumBones; i++) {
			//First, we split up the offset_matrix into its pos, rot and scale
			Bone bone;
			aiBone* ai_bone = mesh->mBones[i];
			aiVector3D base_pos;
			aiVector3D base_rot;
			aiVector3D base_scale;
			mat4 base_matrix = ConvertMatrixToGLMFormat(ai_bone->mOffsetMatrix);
			ai_bone->mOffsetMatrix.Decompose(base_scale, base_rot, base_pos);
			bone.pos.x = base_pos.x;
			bone.pos.y = base_pos.y;
			bone.pos.z = base_pos.z;
			bone.rot.x = base_rot.x;
			bone.rot.y = base_rot.y;
			bone.rot.z = base_rot.z;
			bone.scale.x = base_scale.x;
			bone.scale.y = base_scale.y;
			bone.scale.z = base_scale.z;
			bone.matrix = base_matrix;
			//For the bone's name, we just derive it like this
			bone.name = Filter(ai_bone->mName.C_Str(), "model-armature_");
			bone.id = get_bone_id(bone.name);
			bone.parent_id = this->bones[bone.id].parent_id;

			//Iterate through all of the vertices that this bone influences and set that vertex data accordingly
			for (int i2 = 0; i2 < ai_bone->mNumWeights; i2++) {
				int index = ai_bone->mWeights[i2].mVertexId; //Note that our vertex list gets updated in the exact order of the bones
				for (int i3 = 0; i3 < MAX_BONE_INFLUENCE; i3++) {
					if (vertices[index].weights[i3] == 0.0) {
						vertices[index].bone_ids[i3] = bone.id;
						vertices[index].weights[i3] = ai_bone->mWeights[i2].mWeight;
						break;
					}
				}
			}

			this->bones[bone.id] = bone; //This keeps the model's bone coords up to date by grabbing that data when we can find it. Probably could be
			//optimized since there's no check to make sure the bone is empty, but fixing that is a pretty low priority rn
		}
	}

	return Mesh(vertices, indices, textures, name);
}

vector<Texture> Model::load_material_textures(aiMaterial* mat, aiTextureType type, string typeName) {
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = loadGLTextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}