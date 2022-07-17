#include "Animation.h"
#include <fstream>
#include "Model.h"
#include "Bone.h"
#include "GLM Helpers.h"

#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include <assimp/Importer.hpp>
#include "utils.h"

Animation::Animation() {
	length = 0;
	faf = 0;
}

Animation::Animation(std::string anim_kind, std::string anim_dir, Model *model) {
	this->name = anim_kind;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(anim_dir, aiProcess_Triangulate);

	if (!scene || !scene->HasAnimations()) {
			if (!scene) {
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
			}
			else {
				std::cout << "Scene has no animations but Assimp doesn't seem to see the problem \n";
			}
		return;
	}

	length = scene->mAnimations[0]->mDuration;
	std::vector<AnimBone> base_bones;

	for (int i = 0, max = model->bones.size(); i < max; i++) {
		AnimBone new_bone;
		new_bone.id = model->bones[i].id;
		new_bone.name = model->bones[i].name;
		new_bone.parent_id = model->bones[i].parent_id;
		new_bone.anim_matrix = glm::mat4(1.0);
		base_bones.push_back(new_bone);
	}

	for (int i = 0; i <= length; i++) {
		keyframes.push_back(base_bones);
	}
		
	for (int i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
		aiNodeAnim* node = scene->mAnimations[0]->mChannels[i];
		int index = model->get_bone_id(node->mNodeName.C_Str());
		if (index == -1) {
			std::cout << "Bone " << node->mNodeName.C_Str() << " not found in the model skeleton!" << "\n";
			continue;
		}
		for (int i2 = 0, frame = node->mPositionKeys[i2].mTime; i2 < node->mNumPositionKeys; frame = node->mPositionKeys[++i2].mTime) { //Load the keyframes that are actually baked
			glm::mat4 pos_mat = translate(glm::mat4(1.0), ass_converter(node->mPositionKeys[i2].mValue));
			glm::mat4 rot_mat = toMat4(ass_converter(node->mRotationKeys[i2].mValue));
			glm::mat4 scale_mat = scale(glm::mat4(1.0), ass_converter(node->mScalingKeys[i2].mValue));

			keyframes[frame][index].anim_matrix = pos_mat * rot_mat * scale_mat;
			keyframes[frame][index].keyframed = true;
		}

		int last_keyframed = 0;
		int next_keyframed = 1;

		for (int i2 = 0; i2 < length; i2++) { //Interpolate to find all of the keyframes that aren't baked
			if (keyframes[i2][index].keyframed) {
				//If we find a keyframe that was already baked, set this to our last baked keyframe, then search for the next one after this and 
				//mark that as the next baked keyframe.

				//This approach means that we won't look for the next keyframe until we've found the current one, avoiding unnecessary recalculations

				for (int i3 = i2 + 1; i3 <= length; i3++) {
					if (keyframes[i3][index].keyframed) {
						next_keyframed = i3;
						break;
					}
				}
				last_keyframed = i2;
			}
			else {
				keyframes[i2][index].anim_matrix = keyframes[last_keyframed][index].anim_matrix * ((i2 - last_keyframed) / (float)(next_keyframed - last_keyframed)) + keyframes[next_keyframed][index].anim_matrix * ((next_keyframed - i2) / (float)(next_keyframed - last_keyframed));
			}
		}
	}
	glm::vec3 decomp_other_v3;
	glm::vec4 decomp_other_v4;
	glm::vec3 scale_vec;
	for (int i = 0, max = keyframes.size(); i < max; i++) {
		for (int i2 = 0, max2 = keyframes[i].size(); i2 < max2; i2++) {
			glm::decompose(keyframes[i][i2].anim_matrix, scale_vec, keyframes[i][i2].rot, keyframes[i][i2].pos, decomp_other_v3, decomp_other_v4);
			int parent_id = model->bones[i2].parent_id;
			if (parent_id != -1) {
				keyframes[i][i2].pos = rotate(keyframes[i][i2].pos, keyframes[i][parent_id].rot);
				keyframes[i][i2].pos += keyframes[i][parent_id].pos;

				keyframes[i][i2].rot = keyframes[i][parent_id].rot * keyframes[i][i2].rot;
			}
		}
	}
}

AnimationTable::AnimationTable() {}

void AnimationTable::load_animations(std::string resource_dir, Model *model) {
	std::ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail()) {
		anim_list.close();

		throw std::runtime_error("Anim List Missing");
	}

	std::string name;
	std::string path;
	std::string faf;
	std::string move;
	for (int i = 0; anim_list >> name; i++) {
		anim_list >> path >> faf >> move;
		name = ymlChopString(name);
		path = ymlChopString(path);
		Animation anim(name, resource_dir + "/anims/" + path, model);
		anim.faf = ymlChopInt(faf);
		anim.move = ymlChopInt(move);
		animations.push_back(anim);
		anim_map[name] = i;
	}
	anim_list.close();
}

void AnimationTable::load_animations_no_faf(std::string resource_dir, Model* model) {
	std::ifstream anim_list;
	anim_list.open(resource_dir + "/anims/anim_list.yml");

	if (anim_list.fail()) {
		anim_list.close();

		throw std::runtime_error("Anim List Missing");
	}

	std::string name;
	std::string path_length_container;
	if (model == nullptr) {
		int length;
		for (int i = 0; anim_list >> name; i++) {
			anim_list >> path_length_container;
			name = ymlChopString(name);
			length = ymlChopInt(path_length_container);
			Animation anim;
			anim.name = name;
			anim.length = length;
			animations.push_back(anim);
			anim_map[name] = i;
		}
	}
	else {
		std::string path;
		for (int i = 0; anim_list >> name; i++) {
			anim_list >> path_length_container;
			name = ymlChopString(name);
			path = ymlChopString(path_length_container);
			Animation anim(name, resource_dir + "/anims/" + path, model);
			animations.push_back(anim);
			anim_map[name] = i;
		}
	}

	anim_list.close();
}

void AnimationTable::unload_animations() {
	animations.clear();
}

Animation* AnimationTable::get_anim(std::string anim_name, bool verbose) {
	std::unordered_map<std::string, int>::const_iterator iterator = anim_map.find(anim_name);
	if (iterator == anim_map.end()) {
		if (verbose) {
			std::cout << "Invalid Animation: " << anim_name << "\n";
		}
		return nullptr;
	}
	return &animations[anim_map[anim_name]];
}

	/* 

	⢀⡴⠑⡄⠀⠀⠀⠀⠀⣀⣀⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
   ⠀⠿⡀⠀⠀⠀⣀⡴⢿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠑⢄⣠⠾⠁⣀⣄⡈⠙⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⢀⡀⠁⠀⠀⠈⠙⠛⠂⠈⣿⣿⣿⣿⣿⠿⡿⢿⣆⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⢀⡾⣁⣀⠀⠴⠂⠙⣗⡀⠀⢻⣿⣿⠭⢤⣴⣦⣤⣹⠀⠀⠀⢀⢴⣶⣆ 
⠀⠀⢀⣾⣿⣿⣿⣷⣮⣽⣾⣿⣥⣴⣿⣿⡿⢂⠔⢚⡿⢿⣿⣦⣴⣾⠁⠸⣼⡿ 
⠀⢀⡞⠁⠙⠻⠿⠟⠉⠀⠛⢹⣿⣿⣿⣿⣿⣌⢤⣼⣿⣾⣿⡟⠉⠀⠀⠀⠀⠀ 
⠀⣾⣷⣶⠇⠀⠀⣤⣄⣀⡀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠉⠈⠉⠀⠀⢦⡈⢻⣿⣿⣿⣶⣶⣶⣶⣤⣽⡹⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠉⠲⣽⡻⢿⣿⣿⣿⣿⣿⣿⣷⣜⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣶⣮⣭⣽⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⣀⣀⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀ 
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⠿⠿⠿⠛⠉

	*/