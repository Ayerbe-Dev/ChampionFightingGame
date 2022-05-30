#pragma warning(disable : 4996)
#include "Fighter.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "EffectManager.h"

void Fighter::super_init(int id) {
	this->id = id;
	
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
	RenderManager* render_manager = RenderManager::get_instance();
	scale = glm::vec3(0.05 * get_local_param_float("model_scale"));
	shader.init("vertex_main.glsl", "fragment_main.glsl");
	render_manager->link_shader(&shader);
	model.load_model(resource_dir + "/model/model.dae");
	model.load_textures("c" + std::to_string(player_info->alt_color));
	shader.use();
	shader.set_int("material.diffuse", 0);
	shader.set_int("material.specular", 1);
	shader.set_int("material.shadow_map", 2);

}

void Fighter::load_anim_list() {
	try {
		anim_table.load_animations(resource_dir, &model);
	}
	catch (std::runtime_error err) {
		if (err.what() == "Anim List Missing") {
			char buffer[55];
			sprintf(buffer, "Character %d's resource directory was incorrectly set!", chara_kind);
			player_info->crash_reason = buffer;

			crash_to_debug = true;
		}
		else {
			std::cout << err.what() << "\n";
		}
	}
}

void Fighter::set_default_vars() {
	base_jostle_anchor = glm::vec2(get_local_param_float("jostle_x0"), get_local_param_float("jostle_y0"));
	base_jostle_offset = glm::vec2(get_local_param_float("jostle_x1"), get_local_param_float("jostle_y1"));
	fighter_float[FIGHTER_FLOAT_HEALTH] = get_local_param_float("health");
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = true;
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
}