#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>

class Model;
struct Bone;

class Animation {
public:
	Animation();
	Animation(std::string anim_kind, std::string anim_dir, Model* model);
	std::string name;
	std::vector<std::vector<Bone>> keyframes;
	int length;
	int faf;
};

class AnimationTable {
public:
	AnimationTable();
	void load_fighter_animations(std::string resource_dir, Model* model);
	void load_projectile_animations(std::string resource_dir, Model* model);
	Animation* get_anim(std::string anim_name, bool verbose);
private:
	std::vector<Animation> animations;
	std::unordered_map<std::string, int> anim_map;
};