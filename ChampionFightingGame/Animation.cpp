#include "Animation.h"
#include <fstream>
#include "Model.h"
#include "Bone.h"
#include "GLM Helpers.h"
#include <glm/gtx/matrix_interpolation.hpp>
#include <assimp/Importer.hpp>
#include "utils.h"

Animation::Animation() {}

Animation::Animation(std::string anim_kind, std::string anim_dir, Model *model) {
	this->name = anim_kind;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(anim_dir, aiProcess_Triangulate);

	if (!scene || !scene->HasAnimations()) {
		if (model == nullptr) {
			length = 1;
		}
		else if (!scene) {
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		}
		else {
			std::cout << "Scene has no animations but Assimp doesn't seem to see the problem \n";
		}
		return;
	}

	length = scene->mAnimations[0]->mDuration;

	if (model == nullptr) {
		return;
	}

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
		for (int i2 = 0; i2 < node->mNumPositionKeys; i2++) { //Load the keyframes that are actually baked
			glm::mat4 pos_mat = translate(glm::mat4(1.0), ass_converter(node->mPositionKeys[i2].mValue));
			glm::mat4 rot_mat = toMat4(normalize(ass_converter(node->mRotationKeys[i2].mValue)));
			glm::mat4 scale_mat = scale(glm::mat4(1.0), ass_converter(node->mScalingKeys[i2].mValue));
			keyframes[node->mPositionKeys[i2].mTime][index].anim_matrix = pos_mat * rot_mat * scale_mat;
			keyframes[node->mPositionKeys[i2].mTime][index].keyframed = true;
		}

		int last_keyframed = 0;
		int next_keyframed = 1;
		for (int i2 = 0; i2 < length; i2++) { //Interpolate to find all of the keyframes that aren't baked
			if (keyframes[i2][index].keyframed) {
				//If we find a keyframe that was already baked, set this to our last baked keyframe, then search for the next one after this and 
				//mark that as the next baked keyframe.

				//This approach means that we won't look for the next keyframe until we've found the current one, avoiding unnecessary recalculations

				last_keyframed = i2;
				for (int i3 = last_keyframed + 1; i3 <= length; i3++) {
					if (keyframes[i3][index].keyframed) {
						next_keyframed = i3;
						break;
					}
				}
			}
			else {
				float delta = ((float)i2 - last_keyframed) / (next_keyframed - last_keyframed);
				keyframes[i2][index].anim_matrix = interpolate(keyframes[last_keyframed][index].anim_matrix, keyframes[next_keyframed][index].anim_matrix, delta);
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
	for (int i = 0; anim_list >> name; i++) {
		anim_list >> path >> faf;
		name = ymlChopString(name);
		path = ymlChopString(path);
		Animation anim(name, resource_dir + "/anims/" + path, model);
		anim.faf = ymlChopInt(faf);
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
	std::string path;
	for (int i = 0; anim_list >> name; i++) {
		anim_list >> path;
		name = ymlChopString(name);
		path = ymlChopString(path);
		Animation anim(name, resource_dir + "/anims/" + path, model);
		animations.push_back(anim);
		anim_map[name] = i;
	}
	anim_list.close();
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