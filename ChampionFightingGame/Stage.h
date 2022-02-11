#pragma once
#include "utils.h"
#include <string>
#include <SDL.h>
#include "RenderManager.h"

class Stage {
public:
	int stage_kind;
	int default_music_kind;
	string resource_dir;
	GameTextureNew background;
	vec3 origin;

	Stage();
	Stage(int stage_kind, string stage_name, vec3 origin);

	int get_stage_music();
};