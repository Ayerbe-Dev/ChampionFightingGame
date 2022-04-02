#pragma once
#include <vector>
#include "RenderObject.h"
#include "RenderManager.h"
#include "Animation.h"
#include "Param.h"

class BattleObjectManager;

class StageAsset : public RenderObject {
public:
	StageAsset();
	StageAsset(std::string asset_name, std::string resource_dir, BattleObjectManager* battle_object_manager);

	BattleObjectManager* battle_object_manager;

	AnimationTable anim_table;
	Animation* anim_kind;

	std::string asset_name;
	std::string resource_dir;

	std::vector<int> stage_int;
	std::vector<float> stage_float;
	std::vector<bool> stage_flag;

	float frame;
	float rate;
	bool is_anim_end = false;

	ParamTable params;

	void stage_asset_main();

	void load_model_shader();
	void load_anim_list();
	void load_params();
	void load_lights();

	bool has_model;
	bool has_skeleton;
	bool has_lights;

	int get_param_int(std::string param);
	float get_param_float(std::string param);
	std::string get_param_string(std::string param);
	bool get_param_bool(std::string param);
};