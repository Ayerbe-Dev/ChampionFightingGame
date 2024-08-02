#include "Stage.h"
#include "StageKind.h"
#include "StageAsset.h"
#include "Animation.h"
#include "ObjectManager.h"
#include "ParamAccessor.h"
#include <fstream>

StageInfo::StageInfo() {
	stage_kind = 0;
}

StageInfo::StageInfo(int stage_kind, std::string stage_name) {
	this->stage_kind = stage_kind;
	resource_dir = "resource/stage/" + stage_name + "/";
}

Stage::Stage() {
	camera_bound = 0.0f;
	stage_bound = 0.0f;
	stage_kind = 0;
	start_pos = glm::vec2(0.0);
}

void Stage::load_stage(StageInfo stage_info, ObjectManager *object_manager) {
	object_manager->stage = this;
	stage_kind = stage_info.stage_kind;
	resource_dir = stage_info.resource_dir;

	std::ifstream stream;

	stream.open(resource_dir + "params.yml");
	if (stream.fail()) {
		std::cout << "Failed to open stage params!\n";
		stream.close();
		return;
	}

	WindowManager* window_manager = WindowManager::get_instance();
	stream >> stage_bound >> default_music_kind >> window_manager->ambient_col.r 
		>> window_manager->ambient_col.g >> window_manager->ambient_col.b;
	float distance = get_global_param_float(PARAM_FIGHTER, "spawn_distance") / 2.0;
	start_pos.x = -distance;
	start_pos.y = distance;
	float max_distance = get_global_param_float(PARAM_FIGHTER, "max_distance") / 2.2;
	camera_bound = stage_bound / 20;

	default_music_kind = "resource/sound/bgm/stage/" + default_music_kind;

	stream.close();

	stream.open(resource_dir + "asset_list.yml");
	if (stream.fail()) {
		std::cout << "Failed to open asset list!\n";
		stream.close();
		return;
	}

	std::string asset_name;
	while (stream >> asset_name) {
		std::string resource_dir = this->resource_dir + "assets/" + asset_name;
		stage_assets.push_back(new StageAsset(asset_name, resource_dir, object_manager));
	}

	stream.close();
}

void Stage::unload_stage() {
	for (int i = 0; i < stage_assets.size(); i++) {
		delete stage_assets[i];
	}
}

void Stage::process() {
	for (StageAsset *stage_asset : stage_assets) {
		stage_asset->stage_asset_main();
	}
	while (!funcs.empty()) {
		funcs.front().execute(this);
		funcs.pop();
	}
}

void Stage::render() {
	for (StageAsset* stage_asset : stage_assets) {
		if (stage_asset->has_model) {
			stage_asset->render();
		}
	}
}

void Stage::render_shadow() {
	for (StageAsset* stage_asset : stage_assets) {
		if (stage_asset->has_model) {
			stage_asset->render_shadow();
		}
	}
}
