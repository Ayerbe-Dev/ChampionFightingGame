#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "utils.h"
#include "Bone.h"
#include <assimp/Importer.hpp>

class Model;

class Animation {
public:
	Animation();
	Animation(string anim_kind, string anim_dir, Model* model);
	string name;
	vector<vector<Bone>> keyframes;
	int length;
	int faf;
};