#pragma once
#include <queue>
#include <vector>
#include "GameObject.h"
#include "WindowManager.h"
#include "Animation.h"
#include "Param.h"
#include "ScriptFunc.h"

class ObjectManager;

class StageAsset : public GameObject {
public:
	StageAsset();
	StageAsset(std::string asset_name, std::string resource_dir, ObjectManager* object_manager);
	~StageAsset();

	Stage* owner;

	std::string asset_name;

	std::vector<int> stage_int;
	std::vector<float> stage_float;
	std::vector<bool> stage_flag;

	std::vector<Light> lights;

	ParamTable params;

	std::queue<ScriptFunc<StageAsset>> funcs;

	void stage_asset_main();

	void load_model_shader();
	void load_anim_list();
	void load_params();
	void load_lights();

	bool has_model;
	bool has_lights;

	int get_param_int(std::string param);
	float get_param_float(std::string param);
	std::string get_param_string(std::string param);
	bool get_param_bool(std::string param);
};