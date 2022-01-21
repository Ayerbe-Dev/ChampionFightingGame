#pragma once
#include "utils.h"
#include <string>
#include <SDL2/SDL.h>

class Stage {
public:
	int stage_kind;
	int default_music_kind;
	string resource_dir;
	SDL_Texture* pBackgroundTexture;

	Stage();
	Stage(int stage_kind, string stage_name);

	int get_stage_music();
};