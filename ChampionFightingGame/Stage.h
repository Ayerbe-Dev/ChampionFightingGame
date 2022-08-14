#pragma once
#include "GameObject.h"
#include "StageScript.h"

class StageAsset;
class BattleObjectManager;

class StageInfo {
public:
	StageInfo();
	StageInfo(int stage_kind, std::string stage_name);

	int stage_kind;
	std::string resource_dir;

};

class Stage {
public:
	Stage();

	int stage_kind;

	glm::vec2 start_pos;
	glm::vec2 stage_bounds;
	glm::vec2 camera_bounds;

	std::string default_music_kind;
	std::string resource_dir;
	std::vector<StageAsset*> stage_assets;

	std::queue<ScriptFunc<Stage>> funcs;

	void load_stage(StageInfo stage_info, BattleObjectManager* battle_object_manager);
	void unload_stage();
	void process();
	void render();
	void render_shadow();
};