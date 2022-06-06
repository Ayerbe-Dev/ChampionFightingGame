#include "RoyFireball.h"
#include "RoyFireballConstants.h"

RoyFireball::RoyFireball() {}

RoyFireball::RoyFireball(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->projectile_kind = PROJECTILE_KIND_ROY_FIREBALL;
	projectile_name = "roy_fireball";
	resource_dir = "resource/projectile/roy_fireball";
	projectile_int.resize(PROJECTILE_ROY_FIREBALL_INT_MAX, 0);
	projectile_float.resize(PROJECTILE_ROY_FIREBALL_FLOAT_MAX, 0.0);
	projectile_flag.resize(PROJECTILE_ROY_FIREBALL_FLAG_MAX, false);
	load_params();
	load_move_scripts();
	loadRoyFireballStatusFunctions();
	super_init();
}

void RoyFireball::loadRoyFireballStatusFunctions() {
	status_script.resize(PROJECTILE_ROY_FIREBALL_STATUS_MAX, nullptr);
	enter_status_script.resize(PROJECTILE_ROY_FIREBALL_STATUS_MAX, nullptr);
	exit_status_script.resize(PROJECTILE_ROY_FIREBALL_STATUS_MAX, nullptr);

	ADD_PROJECTILE_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_HOVER, &RoyFireball::status_roy_fireball_hover);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_HOVER, &RoyFireball::enter_status_roy_fireball_hover);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_HOVER, &RoyFireball::exit_status_roy_fireball_hover);

	ADD_PROJECTILE_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED, &RoyFireball::status_roy_fireball_punched);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED, &RoyFireball::enter_status_roy_fireball_punched);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED, &RoyFireball::exit_status_roy_fireball_punched);

	ADD_PROJECTILE_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_KICKED, &RoyFireball::status_roy_fireball_kicked);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_KICKED, &RoyFireball::enter_status_roy_fireball_kicked);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_KICKED, &RoyFireball::exit_status_roy_fireball_kicked);

	ADD_PROJECTILE_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_FALL, &RoyFireball::status_roy_fireball_fall);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_FALL, &RoyFireball::enter_status_roy_fireball_fall);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_FALL, &RoyFireball::exit_status_roy_fireball_fall);

	ADD_PROJECTILE_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_GROUND, &RoyFireball::status_roy_fireball_ground);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_GROUND, &RoyFireball::enter_status_roy_fireball_ground);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROY_FIREBALL_STATUS_GROUND, &RoyFireball::exit_status_roy_fireball_ground);
}

void RoyFireball::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("hover", [this]() {
		if (is_excute_frame(0)) {
			new_effect("flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0));
		}
	});
	script("punched", [this]() {
		if (is_excute_frame(0)) {
			new_effect("flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0));
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, glm::vec2{ -50,-35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				new_hitbox(0, 0, 40, 5, 1.2, 1, glm::vec2{ -50,-35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				new_hitbox(0, 0, 50, 5, 1.2, 1, glm::vec2{ -50,-35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, glm::vec2{ -50,-35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
				new_hitbox(1, 1, 30, 5, 1.2, 1, glm::vec2{ -50,-35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
			}
		}
	});
	script("kicked", [this]() {
		if (is_excute_frame(0)) {
			new_effect("flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.8), glm::vec4(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0));
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, glm::vec2{ -50, -35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				new_hitbox(0, 0, 40, 5, 1.2, 1, glm::vec2{ -50, -35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				new_hitbox(0, 0, 50, 5, 1.2, 1, glm::vec2{ -50, -35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
			}
			if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, glm::vec2{ -50, -35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
				new_hitbox(1, 1, 30, 5, 1.2, 1, glm::vec2{ -50, -35 }, glm::vec2{ 50, 35 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, 1, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true, false, true);
			}
		}
	});
}

/*
  (                                 _
   )                               /=>
  (  +____________________/\/\___ / /|
   .''._____________'._____      / /|/\
  : () :              :\ ----\|    \ )
   '..'______________.'0|----|      \
					0_0/____/        \
						|----    /----\
					   || -\\ --|      \
					   ||   || ||\      \
						\\____// '|      \
Bang! Bang!                     .'/       |
							   .:/        |
							   :/_________|
*/

void RoyFireball::projectile_unique_main() {
	if (projectile_int[PROJECTILE_INT_HEALTH] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void RoyFireball::status_default() {
	if (battle_object_manager->fighter[owner_id]->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		projectile_int[PROJECTILE_INT_HEALTH] = 2;
	}
	change_status(PROJECTILE_ROY_FIREBALL_STATUS_HOVER);
}

void RoyFireball::status_hit() {
	if (is_anim_end) {
		active = false;
	}
}

void RoyFireball::enter_status_hit() {
	change_anim("hit");
}

void RoyFireball::status_roy_fireball_hover() {
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0 && owner->status_kind == CHARA_ROY_STATUS_SPECIAL_FIREBALL_START) {
		change_status(PROJECTILE_ROY_FIREBALL_STATUS_FALL);
	}
}

void RoyFireball::enter_status_roy_fireball_hover() {
	pos = owner->get_bone_position("HaveL", glm::vec3(35.0, 0.0, 0.0));
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("hover_active_time", params);
	change_anim("hover");
}

void RoyFireball::exit_status_roy_fireball_hover() {
	clear_effect_all();
}

void RoyFireball::status_roy_fireball_punched() {
	if (owner->fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_L) {
		pos.x += get_local_param_float("punch_move_x_speed_l", params) * facing_dir;
	}
	else if (owner->fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_M) {
		pos.x += get_local_param_float("punch_move_x_speed_m", params) * facing_dir;
	}
	else {
		pos.x += get_local_param_float("punch_move_x_speed_h", params) * facing_dir;
	}
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void RoyFireball::enter_status_roy_fireball_punched() {
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("punch_active_time", params);
	change_anim("punched");
}

void RoyFireball::exit_status_roy_fireball_punched() {
	clear_effect_all();
}

void RoyFireball::status_roy_fireball_kicked() {
	if (owner->fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_L) {
		pos.x += get_local_param_float("kick_move_x_speed_l", params) * facing_dir;
		pos.y -= get_local_param_float("kick_move_y_speed_l", params);
	}
	else if (owner->fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_M) {
		pos.x += get_local_param_float("kick_move_x_speed_m", params) * facing_dir;
		pos.y -= get_local_param_float("kick_move_y_speed_m", params);
	}
	else {
		pos.x += get_local_param_float("kick_move_x_speed_h", params) * facing_dir;
		pos.y -= get_local_param_float("kick_move_y_speed_h", params);
	}
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void RoyFireball::enter_status_roy_fireball_kicked() {
	pos = owner->get_bone_position("FootR", glm::vec3(20.0, -25.0, 0.0));
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("kick_active_time", params);
	change_anim("kicked");
}

void RoyFireball::exit_status_roy_fireball_kicked() {
	clear_effect_all();
}

void RoyFireball::status_roy_fireball_fall() {
	if (is_anim_end) {
		change_status(PROJECTILE_ROY_FIREBALL_STATUS_GROUND);
	}
}

void RoyFireball::enter_status_roy_fireball_fall() {
	change_anim("fall");
}

void RoyFireball::exit_status_roy_fireball_fall() {

}

void RoyFireball::status_roy_fireball_ground() {
	if (is_anim_end) {
		active = false;
	}
}

void RoyFireball::enter_status_roy_fireball_ground() {
	change_anim("ground");
}

void RoyFireball::exit_status_roy_fireball_ground() {

}