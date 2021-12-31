#pragma once
#include <SDL.h>
#include <string>
#include <vector>
#include "utils.h"
#include "Bone.h"

struct Animation {
	SDL_Texture* spritesheet;
	string name{ "default"};
	string path{ "default.png" };
	int length{ -1 };
	int faf{ -1 };
	int force_center{ 0 };
	int move_dir{ 0 };
	SDL_Rect anim_map[MAX_ANIM_LENGTH] = {};

	Animation();
};

class Animation3D {
public:
	Animation3D();
	Animation3D(string anim_kind, string anim_dir);
	string name;
	vector<vector<Bone>> keyframes;
	int length;
};

SDL_Rect getFrame(int frame, Animation* animation);
void loadAnimation(Animation* animation);