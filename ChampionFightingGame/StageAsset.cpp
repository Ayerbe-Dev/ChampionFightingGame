#include "StageAsset.h"
#include "ObjectManager.h"
#include "Fighter.h"
#include <fstream>
#include "GameManager.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "ShaderManager.h"

StageAsset::StageAsset() {
	has_lights = false;
	has_model = false;
	owner = nullptr;
}

StageAsset::StageAsset(std::string asset_name, std::string resource_dir, ObjectManager* object_manager) {
	this->asset_name = asset_name;
	this->resource_dir = resource_dir;
	this->object_manager = object_manager;
	owner = object_manager->stage;

	load_params();
	load_model_shader();
	load_anim_list();
	has_lights = get_param_bool("has_lights");
	if (has_lights) {
		load_lights();
	}
}

StageAsset::~StageAsset() {
	model.unload_model_instance();
}

void StageAsset::stage_asset_main() {
	//Execute the stage script, use it to populate funcs

	while (!funcs.empty()) {
		funcs.front().execute(this);
		funcs.pop();
	}
}

void StageAsset::load_model_shader() {
	set_scale(glm::vec3(get_param_float("model_scale")));
	has_model = get_param_bool("has_model");
	if (has_model) {
		model.load_model_instance(resource_dir + "/model/model.dae");
		model.load_textures();
		init_shader();
	}
}

void StageAsset::load_anim_list() {
	try {
		if (has_model) {
			anim_table.load_anlst(resource_dir + "/anims", model.get_skeleton());
		}
		else {
			anim_table.load_anlst(resource_dir + "/anims", Skeleton());
		}
	}
	catch (std::runtime_error err) {
		if (err.what() == "Anim List Missing") {
			GameManager::get_instance()->add_crash_log("Stage " + asset_name + "'s resource directory was incorrectly set!");
		}
		else {
			std::cout << err.what() << "\n";
		}
	}
}

void StageAsset::load_params() {
	params.load_params(resource_dir + "/param/params.prmlst");
}

void StageAsset::load_lights() {
	std::ifstream light_stream;
	light_stream.open(resource_dir + "/param/lights.yml");
	if (light_stream.fail()) {
		std::cout << "Failed to load lights! \n";
		light_stream.close();
		return;
	}

	lights.reserve(MAX_LIGHT_SOURCES);

	WindowManager* window_manager = WindowManager::get_instance();

	//WindowManager now gets pointers to the lights, so we'll store them
	//in the StageAssets. This is also better because it means we can move the lights around within the
	//stage scripts and have it affect the WindowManager.

	glm::vec3 light_pos; 
	glm::vec3 light_col;
	float brightness;
	while (light_stream >> light_pos.x) {
		light_stream >> light_pos.y >> light_pos.z >> light_col.x >> light_col.y >> light_col.z >> brightness;
		lights.push_back(window_manager->add_light(light_pos, light_col, brightness));
	}
	
	light_stream.close();
}

int StageAsset::get_param_int(std::string param) {
	return params.get_param_int(param);
}

float StageAsset::get_param_float(std::string param) {
	return params.get_param_float(param);
}

std::string StageAsset::get_param_string(std::string param) {
	return params.get_param_string(param);
}

bool StageAsset::get_param_bool(std::string param) {
	return params.get_param_bool(param);
}