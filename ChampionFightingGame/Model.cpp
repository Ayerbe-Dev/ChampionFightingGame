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
		bone_name = Filter(bone_name, "\""); //Remove the "s from the SMD's bone names
		new_bone.name = bone_name;
		new_bone.id = bone_id;
		new_bone.parent_id = parent_id;
		bones.push_back(new_bone);
	}

	smd.close();
}

void Model::load_model(string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_PopulateArmatureData);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}

	global_transform = inverse(ConvertMatrixToGLMFormat(scene->mRootNode->mTransformation));

	directory = path.substr(0, path.find_last_of('/')) + "/";
	string skeleton_path = directory + "skeleton.smd";
	load_skeleton(skeleton_path); 
	
	//Note: skeleton.smd is just an smd file stripped down to the bone list. Since bones are processed in a specific order during set_bones, I want to
	//populate the model's bone list in order of ID, and I thought this was the best way to do it. 

	process_node(scene->mRootNode, scene);
}

void Model::render(Shader* shader) {
	for (int i = 0; i < bones.size(); i++) {
		string final_mat = "bone_matrix[" + to_string(i) + "]";
		shader->set_mat4(final_mat, bones[i].final_matrix);
	}
	//1-2 ms even with the function skipping over every single bone

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].render(shader);
	}
	//1-2 ms but everything in there is necessary
}

void Model::set_bones(float frame, Animation3D *anim_kind) {
	//First, we get all of the keyframes for both the current frame and the next frame (Note: For now this assumes every frame is baked, in the future
	//I'll change this to use interpolation

	int frame_index = clamp(0, floorf(frame), anim_kind->keyframes.size() - 1); 
	int next_frame_index = clamp(0, floorf(frame + 1), anim_kind->keyframes.size() - 1);
	vector<Bone> keyframes = anim_kind->keyframes[frame_index];
	vector<Bone> next_keyframes = anim_kind->keyframes[next_frame_index];
	float decimal = (float)frame_index - frame;

	for (int i = 0; i < keyframes.size(); i++) { //Iterate through all bones
		Bone curr_frame_offsets = keyframes[i];
		Bone next_frame_offsets = next_keyframes[i];

		//Calculate the difference in offset between the current frame and the next frame, then multiply said difference by the decimal place. This will
		//allow us to interpolate between non-integer keyframes, so if our frame is 3.5, for example, a bone will be halfway between its frame 3 and 4
		//keyframes

		next_frame_offsets.anim_matrix -= curr_frame_offsets.anim_matrix;
		curr_frame_offsets.anim_matrix += decimal * next_frame_offsets.anim_matrix;

		//If we have a parent bone, we also want to add in the offsets from that parent bone. Since a bone will never be at a lower index than its
		//parent, we don't need to worry about populating the entire bone vector before making this calc, the spot we're actually looking at is going
		//to be filled by the time we look at it.

		//There might be a better way to do this but if I DON'T factor in the parent matrix, the issues in bone placement are still a lot worse than just
		//not including the parent

		mat4 parent_matrix = mat4(1.0);
		if (curr_frame_offsets.parent_id != -1) {
			parent_matrix = bones[curr_frame_offsets.parent_id].anim_matrix;
		}

		//Update our anim_matrix so child bones can use it in future iterations

		bones[i].anim_matrix = curr_frame_offsets.anim_matrix;

		//This equation is probably wrong

//		bones[i].final_matrix = global_transform * parent_matrix * bones[i].anim_matrix * bones[i].anim_rest_matrix * bones[i].model_matrix;
		bones[i].final_matrix = mat4(1.0);

		if (bones[i].model_matrix != inverse(bones[i].anim_rest_matrix)) {
			//cout << "Because that makes sense" << endl;

			//Yeah so for some reason they were equal earlier but they aren't equal now. I swear I didn't touch the model_matrix.
		}
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

	if (mesh->HasBones()) {
		for (int i = 0; i < mesh->mNumBones; i++) {
			Bone bone;
			aiBone* ai_bone = mesh->mBones[i];
			aiNode* ai_node = ai_bone->mArmature;
			mat4 anim_matrix = ConvertMatrixToGLMFormat(ai_node->mTransformation); //Bone matrix in Bone Space (rest position)
			mat4 model_matrix = inverse(anim_matrix); //Bone matrix in Model Space. I store this separately because, for whatever reason, by the time
			//I'm adjusting the skeleton, bones[i].anim_rest_matrix * inverse(bones[i].anim_rest_matrix) will not equate to mat4(1.0). Make it make sense.

			//mat4 model_matrix = ConvertMatrixToGLMFormat(ai_bone->mOffsetMatrix); I THOUGHT this would be the Bone matrix in Model Space (and 
			//therefore equivalent to inverse(anim_matrix), but it didn't seem to be usable for anything even though I've seen other tutorials use
			//it as the offset matrix

			bone.model_matrix = model_matrix;
			bone.anim_rest_matrix = anim_matrix;
			bone.anim_matrix = anim_matrix;

			//Note: These vectors are only used so that empty keyframe data can be filled in properly (mostly scale, since SMD doesn't actually support
			//that). Will be removed in the future, but it isn't the cause of the current problem.

			aiVector3D base_pos; 
			aiVector3D base_rot;
			aiVector3D base_scale;
			ai_node->mTransformation.Decompose(base_scale, base_rot, base_pos);
			bone.pos.x = base_pos.x;
			bone.pos.y = base_pos.y;
			bone.pos.z = base_pos.z;
			bone.rot.x = base_rot.x;
			bone.rot.y = base_rot.y;
			bone.rot.z = base_rot.z;
			bone.scale.x = base_scale.x;
			bone.scale.y = base_scale.y;
			bone.scale.z = base_scale.z;

			bone.name = Filter(ai_bone->mName.C_Str(), "model-armature_"); //Blender be like
			bone.id = get_bone_id(bone.name);
			bone.parent_id = this->bones[bone.id].parent_id; //this->bones is already initialized to contain the name, ID and parent ID of each bone,
			//but since we generate the rest of the info here, we need to copy over the one thing that isn't able to be derived

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

			this->bones[bone.id] = bone; //This keeps the model's bone coords up to date by grabbing that data once we can find it. Probably could be
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