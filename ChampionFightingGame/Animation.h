#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>

#include "Bone.h"

class Animation {
public:
	Animation();

	void init(std::string name, std::string filename, Skeleton skeleton);

	std::string name;
	std::vector<glm::mat4> trans_matrices;
	std::vector<std::vector<AnimBone>> keyframes;
	int length;
	int faf;
	bool flag_move;
	bool flag_no_hitlag_interp;
	bool flag_unused;
};

class AnimationTable {
public:
	AnimationTable();
	AnimationTable(AnimationTable& other);
	AnimationTable(const AnimationTable& other);
	AnimationTable& operator=(AnimationTable& other);
	AnimationTable& operator=(const AnimationTable& other);
	void load_anlst(std::string resource_dir, Skeleton skeleton);
	void load_anim_single(std::string name, std::string anim_filename, int end_frame, bool flag_move, bool flag_no_hitlag_interp, Skeleton skeleton);
	void unload_animations();
	Animation* get_anim(std::string anim_name, bool verbose);
private:
	std::vector<Animation> animations;
	std::unordered_map<std::string, int> anim_map;

};