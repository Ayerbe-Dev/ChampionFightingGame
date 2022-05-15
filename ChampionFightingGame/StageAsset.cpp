#include "StageAsset.h"
#include "BattleObjectManager.h"
#include "Fighter.h"
#include <fstream>

StageAsset::StageAsset() {

}

StageAsset::StageAsset(std::string asset_name, std::string resource_dir, BattleObjectManager* battle_object_manager) {
	this->asset_name = asset_name;
	this->resource_dir = resource_dir;
	this->battle_object_manager = battle_object_manager;
	load_params();
	load_model_shader();
	load_anim_list();
	has_lights = get_param_bool("has_lights");
	if (has_lights) {
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
	shader.use();
	shader.set_int("material.diffuse", 0);
	shader.set_int("material.specular", 1);
	shader.set_int("material.shadow_map", 2);
	RenderManager* render_manager = RenderManager::get_instance();
	render_manager->link_shader(&shader);
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
			battle_object_manager->fighter[0]->player_info->crash_reason = "Stage " + asset_name + "'s resource directory was incorrectly set!";
			battle_object_manager->fighter[0]->crash_to_debug = true;
		}
		else {
			std::cout << err.what() << "\n";
		}
	}
}

void StageAsset::load_params() {
	params.load_params(resource_dir + "/param/params.yml");
}

void StageAsset::load_lights() {
	std::ifstream light_stream;
	light_stream.open(resource_dir + "/param/lights.yml");
	if (light_stream.fail()) {
		std::cout << "Failed to load lights! \n";
		light_stream.close();
		return;
	}

	RenderManager* render_manager = RenderManager::get_instance();


	std::vector<glm::vec3> tmp_lights;
	//glm::vec3 light_pos;
	int light_i = 0;
	tmp_lights.resize(MAX_LIGHT_SOURCES);
	while (light_stream >> tmp_lights[light_i].x) {
		light_stream >> tmp_lights[light_i].y;
		light_stream >> tmp_lights[light_i].z;
		//printf("Light Vector %f %f %f\n", tmp_lights[light_i].x, tmp_lights[light_i].y, tmp_lights[light_i].z);
		//render_manager->add_light(Light(light_pos));
		light_i++;
	}
	
	for (unsigned int i = 0; i < MAX_LIGHT_SOURCES; i++) {
		render_manager->lights[i].position.x = tmp_lights[i].x;
		render_manager->lights[i].position.y = tmp_lights[i].y;
		render_manager->lights[i].position.z = tmp_lights[i].z;
		//printf("StageAsset.cpp %f %f %f %p\n", render_manager->lights[i].position.x, render_manager->lights[i].position.y, render_manager->lights[i].position.z, &render_manager->lights[i]);
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