#include "Stage.h"
#include "StageKind.h"
#include "StageAsset.h"
#include "Music.h"
#include "Animation.h"
#include "FighterAccessor.h"
#include <fstream>
extern RenderManager g_rendermanager;

StageInfo::StageInfo() {}

StageInfo::StageInfo(int stage_kind, std::string stage_name) {
	this->stage_kind = stage_kind;
	resource_dir = "resource/stage/" + stage_name + "/";

	default_music_kind = get_stage_music();
}

int StageInfo::get_stage_music() {
	switch (stage_kind) {
		case (STAGE_KIND_TRAINING_OLD):
		{
			return MUSIC_KIND_ATLAS_STAGE;
		} break;
		default:
		{
			return MUSIC_KIND_TRAINING_STAGE;
		} break;
	}
}

Stage::Stage() {}

void Stage::load_stage(StageInfo stage_info, FighterAccessor *fighter_accessor) {
	stage_kind = stage_info.stage_kind;
	resource_dir = stage_info.resource_dir;
	default_music_kind = stage_info.default_music_kind;

	std::ifstream asset_list;
	asset_list.open(resource_dir + "asset_list.yml");
	if (asset_list.fail()) {
		std::cout << "Failed to open asset list!" << "\n";
		asset_list.close();
		return;
	}

	std::string asset_name;
	while (asset_list >> asset_name) {
		std::string resource_dir = this->resource_dir + "assets/" + asset_name;
		stage_assets.push_back(new StageAsset(asset_name, resource_dir, fighter_accessor));
	}

	asset_list.close();
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