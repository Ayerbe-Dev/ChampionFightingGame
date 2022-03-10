#pragma once
#include "GameTexture.h"
#include "RenderObject.h"
#include "RenderManager.h"

class Stage : public RenderObject {
public:
	int stage_kind;
	int default_music_kind;
	std::string resource_dir;
	GameTexture background;

	Stage();
	Stage(int stage_kind, std::string stage_name);

	int get_stage_music();
};