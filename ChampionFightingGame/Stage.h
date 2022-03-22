#pragma once
#include "RenderObject.h"

class StageAsset;
class BattleObjectManager;

class StageInfo {
public:
	StageInfo();
	StageInfo(int stage_kind, std::string stage_name);

	int stage_kind;
	int default_music_kind;
	std::string resource_dir;

	int get_stage_music();
};

class Stage {
public:
	int stage_kind;
	int default_music_kind;
	std::string resource_dir;
	std::vector<StageAsset*> stage_assets;

	Stage();

	void load_stage(StageInfo stage_info, BattleObjectManager* battle_object_manager);
	void unload_stage();
	void process();
	void render();
};