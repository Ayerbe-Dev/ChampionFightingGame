#pragma once
#include "utils.h"
#include <string>
#include <SDL.h>

class Stage {
public:
	string stage_id;
	string resource_dir;
	SDL_Texture* pBackgroundTexture;

	Stage();
	Stage(string stage_id);
};