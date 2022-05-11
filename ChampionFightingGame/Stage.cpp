#include "Stage.h"
#include "StageKind.h"
#include "StageAsset.h"
#include "Animation.h"
#include "BattleObjectManager.h"
#include "ParamAccessor.h"
#include <fstream>
extern RenderManager g_rendermanager;

StageInfo::StageInfo() {}

StageInfo::StageInfo(int stage_kind, std::string stage_name) {
	this->stage_kind = stage_kind;
	resource_dir = "resource/stage/" + stage_name + "/";

	default_music_kind = "Vesuvius_Theme"; //Change later
}

Stage::Stage() {}

void Stage::load_stage(StageInfo stage_info, BattleObjectManager *battle_object_manager) {
	battle_object_manager->stage = this;
	stage_kind = stage_info.stage_kind;
	resource_dir = stage_info.resource_dir;
	default_music_kind = stage_info.default_music_kind;

	std::ifstream stream;

	stream.open(resource_dir + "params.yml");
	if (stream.fail()) {
		std::cout << "Failed to open stage params!\n";
		stream.close();
		return;
	}

	stream >> stage_bounds.x >> stage_bounds.y;
	float center_point = (stage_bounds.x + stage_bounds.y) / 2.0;
	float distance = get_param_float("spawn_distance", PARAM_FIGHTER) / 2.0;
	start_pos.x = center_point - distance;
	start_pos.y = center_point + distance;

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
		stage_assets.push_back(new StageAsset(asset_name, resource_dir, battle_object_manager));
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
}

void Stage::render() {
	for (StageAsset* stage_asset : stage_assets) {
		if (stage_asset->has_model) {
			stage_asset->render();
		}
	}
}

void Stage::render_shadow()
{
	for (StageAsset* stage_asset : stage_assets) {
		if (stage_asset->has_model) {
			stage_asset->render_shadow();
		}
	}
}
