#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>

class Model;
struct AnimBone;

class Animation {
public:
	Animation();
	Animation(std::string anim_kind, std::string anim_dir, Model* model);

	std::string name;
	std::vector<std::vector<AnimBone>> keyframes;
	int length;
	int faf;
	bool move = false;
};

class AnimationTable {
public:
	AnimationTable();
	void load_animations(std::string resource_dir, Model* model);
	void load_animations_no_faf(std::string resource_dir, Model* model);
	Animation* get_anim(std::string anim_name, bool verbose);
private:
	std::vector<Animation> animations;
	std::unordered_map<std::string, int> anim_map;
};