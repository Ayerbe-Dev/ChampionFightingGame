#include "Animation.h"
#include "Model.h"
#include "GLM Helpers.h"

#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Anlst.h"
#include "utils.h"
#include <chrono>

Animation::Animation() {
	length = 0;
	faf = 0;
	flag_move = false;
	flag_no_hitlag_interp = false;
	flag_unused = false;
}

void Animation::init(std::string name, std::string filename, Skeleton skeleton) {
	this->name = name;

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(filename, aiProcess_Triangulate);

	if (!scene || !scene->HasAnimations()) {
		if (!scene) {
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		}
		else {
			std::cout << "Scene has no animations but Assimp doesn't seem to see the problem \n";
		}
		return;
	}

	length = (int)scene->mAnimations[0]->mDuration;
	std::vector<AnimBone> base_bones;

	for (int i = 0, max = skeleton.bone_data.size(); i < max; i++) {
		AnimBone new_bone;
		new_bone.id = skeleton.bone_data[i].id;
		new_bone.name = skeleton.bone_data[i].name;
		new_bone.parent_id = skeleton.bone_data[i].parent_id;
		new_bone.anim_matrix = glm::mat4(1.0);
		base_bones.push_back(new_bone);
	}

	for (int i = 0; i <= length; i++) {
		keyframes.push_back(base_bones);
	}
		
	for (int i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
		aiNodeAnim* node = scene->mAnimations[0]->mChannels[i];
		int index = skeleton.get_bone_id(node->mNodeName.C_Str());
		if (index == -1) {
			std::cout << "Bone " << node->mNodeName.C_Str() << " not found in the model skeleton!" << "\n";
			continue;
		}
		for (unsigned i2 = 0, frame = (unsigned)node->mPositionKeys[i2].mTime; i2 < node->mNumPositionKeys; frame = (unsigned)node->mPositionKeys[++i2].mTime) { //Load the keyframes that are actually baked
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
			int parent_id = skeleton.bone_data[i2].parent_id;
			if (parent_id != -1) {
				keyframes[i][i2].pos = rotate(keyframes[i][i2].pos, keyframes[i][parent_id].rot);
				keyframes[i][i2].pos += keyframes[i][parent_id].pos;

				keyframes[i][i2].rot = keyframes[i][parent_id].rot * keyframes[i][i2].rot;
			}
		}
	}
}

AnimationTable::AnimationTable() {

}

AnimationTable::AnimationTable(AnimationTable& other) {
	animations.clear();
	anim_map.clear();
	for (int i = 0, max = other.animations.size(); i < max; i++) {
		animations.push_back(other.animations[i]);
	}
	anim_map = other.anim_map;
}

AnimationTable::AnimationTable(const AnimationTable& other) {
	animations.clear();
	anim_map.clear();
	for (int i = 0, max = other.animations.size(); i < max; i++) {
		animations.push_back(other.animations[i]);
	}
	anim_map = other.anim_map;
}

AnimationTable& AnimationTable::operator=(AnimationTable& other) {
	if (this == &other) {
		return *this;
	}
	animations.clear();
	anim_map.clear();

	for (int i = 0, max = other.animations.size(); i < max; i++) {
		animations.push_back(other.animations[i]);
	}
	anim_map = other.anim_map;

	return *this;
}

AnimationTable& AnimationTable::operator=(const AnimationTable& other) {
	if (this == &other) {
		return *this;
	}
	animations.clear();
	anim_map.clear();

	for (int i = 0, max = other.animations.size(); i < max; i++) {
		animations.push_back(other.animations[i]);
	}
	anim_map = other.anim_map;

	return *this;
}

void AnimationTable::load_anlst(std::string resource_dir, Skeleton skeleton) {
	std::ifstream anim_list;
	anim_list.open(resource_dir + "/anim_list.anlst", std::ios::binary);

	if (anim_list.fail()) {
		anim_list.close();

		throw std::runtime_error("Anim List Missing");
	}
	std::string name;
	std::string filename;
	int end_frame;
	bool flag_move;
	bool flag_no_hitlag_interp;
	bool flag_unused;
	if (skeleton) {
		for (size_t i = 0; !anim_list.eof(); i++) {
			parse_anlst_entry(anim_list, name, filename, end_frame, flag_move, flag_no_hitlag_interp, 
				flag_unused);
			if (name == "") {
				break;
			}
			Animation anim;
			if (filename != "none") { //If we're loading an animation file, we run normal code
				anim.init(name, resource_dir + "/" + filename, skeleton);
				anim.faf = end_frame;
			}
			else { //If there's no animation file, our end frame specifies the length instead of faf
				anim.name = name;
				anim.length = end_frame;
				anim.faf = -1;
			}
			anim.flag_move = flag_move;
			anim.flag_no_hitlag_interp = flag_no_hitlag_interp;
			anim.flag_unused = flag_unused;
			animations.push_back(anim);
			anim_map[name] = i;
		}
	}
	else { //And if we don't even provide a skeleton, we can assume there's no animation file either
		for (size_t i = 0; !anim_list.eof(); i++) {
			parse_anlst_entry(anim_list, name, filename, end_frame, flag_move, flag_no_hitlag_interp, 
				flag_unused);
			if (name == "") {
				break;
			}
			Animation anim;
			anim.name = name;
			anim.length = end_frame;
			anim.faf = -1;
			anim.flag_move = flag_move;
			anim.flag_no_hitlag_interp = flag_no_hitlag_interp;
			anim.flag_unused = flag_unused;
			animations.push_back(anim);
			anim_map[name] = i;
		}
	}
	anim_list.close();
}

void AnimationTable::load_anim_single(std::string name, std::string anim_filename, int end_frame, bool flag_move, bool flag_no_hitlag_interp, Skeleton skeleton) {
	Animation anim;
	if (anim_filename != "none") {
		anim.init(name, anim_filename, skeleton);
		anim.faf = end_frame;
	}
	else {
		anim.name = name;
		anim.length = end_frame;
		anim.faf = -1;
	}
	anim.flag_move = flag_move;
	anim.flag_no_hitlag_interp = flag_no_hitlag_interp;
	anim_map[name] = animations.size();
	animations.push_back(anim);
}

void AnimationTable::unload_animations() {
	animations.clear();
}

Animation* AnimationTable::get_anim(std::string anim_name, bool verbose) {
	if (!anim_map.contains(anim_name)) {
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

//What are you doing in his swamp