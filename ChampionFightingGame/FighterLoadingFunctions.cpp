#pragma warning(disable : 4996)
#include "Fighter.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "GameManager.h"
#include <fstream>

void Fighter::super_init() {
	sound_manager->add_sound_player(id);
	effect_manager->add_effect_caster(id);
	stage = battle_object_manager->stage;

	if (id == 0) {
		pos = glm::vec3(stage->start_pos.x, FLOOR_GAMECOORD, 0);
		facing_right = true;
		internal_facing_right = true;
		facing_dir = 1.0;
	}
	else {
		pos = glm::vec3(stage->start_pos.y, FLOOR_GAMECOORD, 0);
		facing_right = false;
		internal_facing_right = false;
		facing_dir = -1.0;
	}

	load_stats();
	load_params();
	load_model_shader();
	init_boxes();
	load_anim_list();
	load_status_scripts();
	load_move_scripts();
	load_fighter_sounds();
	load_chara_sounds();
	load_fighter_effects();
	load_chara_effects();
	set_default_vars();

	change_status(FIGHTER_STATUS_WAIT, false, false);
}

void Fighter::load_fighter_sounds() {

}

void Fighter::load_fighter_effects() {

}

void Fighter::load_model_shader() {
	scale = glm::vec3(0.05 * get_local_param_float("model_scale"));
	model.load_model(resource_dir + "/model/model.dae");
	model.load_textures("c" + std::to_string(player->alt_color));
	shader.init("vertex_main.glsl", "fragment_main.glsl", "geometry_main.glsl");
	shadow_shader.init("vertex_shadow.glsl", "fragment_shadow.glsl");
	outline_shader.init("vertex_outline.glsl", "fragment_outline.glsl", "geometry_outline.glsl");
	shader.use();
	shader.set_int("shadow_map", 0);
	shader.set_int("material.diffuse", 1);
	shader.set_int("material.specular", 2);
	shader.set_float("brightness_mul", 1.0);
	shader.set_bool("has_skeleton", model.has_skeleton);
	shadow_shader.use();
	shadow_shader.set_bool("has_skeleton", model.has_skeleton);
	outline_shader.use();
	outline_shader.set_bool("has_skeleton", model.has_skeleton);

	render_manager->link_shader(&shader);
	render_manager->link_shader(&shadow_shader);
	render_manager->link_shader(&outline_shader);
}

void Fighter::load_anim_list() {
	try {
		anim_table.load_animations(resource_dir, &model);
	}
	catch (std::runtime_error err) {
		if (err.what() == "Anim List Missing") {
			GameManager::get_instance()->add_crash_log("Chara " + std::to_string(chara_kind) + "\'s animation directory was incorrectly set!");
		}
		else {
			std::cout << err.what() << "\n";
		}
	}
	std::ifstream camera_stream;
	camera_stream.open(resource_dir + "/cam_anims/anim_list.yml");
	if (camera_stream.fail()) {
		GameManager::get_instance()->add_crash_log("Chara " + std::to_string(chara_kind) + "\'s camera directory was incorrectly set!");
	}
	std::string cam_anim_name;
	while (camera_stream >> cam_anim_name) {
		render_manager->camera.load_camera_anim(cam_anim_name, resource_dir + "/cam_anims/" + cam_anim_name + ".fbx");
	}
}

void Fighter::set_default_vars() {
	base_jostle_anchor = glm::vec2(get_local_param_float("jostle_x0"), get_local_param_float("jostle_y0"));
	base_jostle_offset = glm::vec2(get_local_param_float("jostle_x1"), get_local_param_float("jostle_y1"));
	fighter_float[FIGHTER_FLOAT_HEALTH] = get_local_param_float("health");
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = true;
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_NONE;
}

void Fighter::init_boxes() {
	jostle_box.init();
	jostle_box.bind_scale(&scale);
	jostle_box.set_rgba(glm::vec4(0, 0, 0, 204));
	for (int i = 0; i < 10; i++) {
		hitboxes[i].init(this);
		hurtboxes[i].init(this);
		grabboxes[i].init(this);
	}
	blockbox.init(this);
}