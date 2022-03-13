#include "StageAsset.h"
#include "FighterAccessor.h"
#include "Fighter.h"

extern RenderManager g_rendermanager;

StageAsset::StageAsset() {

}

StageAsset::StageAsset(std::string asset_name, std::string resource_dir, FighterAccessor* fighter_accessor) {
	this->asset_name = asset_name;
	this->resource_dir = resource_dir;
	this->fighter_accessor = fighter_accessor;
	load_params();
	load_model_shader();
	load_anim_list();
	if (get_param_bool("has_lights")) {
		load_lights();
	}
}

void StageAsset::stage_asset_main() {

}

void StageAsset::load_model_shader() {
	scale = glm::vec3(0.05);
	has_model = get_param_bool("has_model");
	has_skeleton = get_param_bool("has_skeleton");
	if (has_model) {
		if (has_skeleton) {
			model.load_model(resource_dir + "/model/model.dae");
			shader.init("vertex_main.glsl", "fragment_main.glsl");
		}
		else {
			model.load_model_no_skeleton(resource_dir + "/model/model.dae");
			shader.init("vertex_no_anim.glsl", "fragment_main.glsl");
		}
	}
	else {
		shader.init("vertex_no_anim.glsl", "fragment_main.glsl");
	}
	g_rendermanager.link_shader(&shader);
}

void StageAsset::load_anim_list() {
	Model* model_ptr;
	if (has_model) {
		model_ptr = &model;
	}
	else {
		model_ptr = nullptr;
	}
	try {
		anim_table.load_animations_no_faf(resource_dir, model_ptr);
	}
	catch (std::runtime_error err) {
		if (err.what() == "Anim List Missing") {
			fighter_accessor->fighter[0]->player_info->crash_reason = "Stage " + asset_name + "'s resource directory was incorrectly set!";
			fighter_accessor->fighter[0]->crash_to_debug = true;
		}
		else {
			std::cout << err.what() << std::endl;
		}
	}
}

void StageAsset::load_params() {
	params.load_params(resource_dir + "/param/params.yml");
}

void StageAsset::load_lights() {

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