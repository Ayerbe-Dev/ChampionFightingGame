#include "RoyFireball.h"
#include "RoyFireballConstants.h"

void RoyFireball::projectile_unique_main() {
	if (projectile_int[PROJECTILE_INT_HEALTH] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void RoyFireball::status_default() {
	if (owner->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
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
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		if (owner->status_kind != CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH
			&& owner->status_kind != CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK) {
			change_status(PROJECTILE_ROY_FIREBALL_STATUS_FALL);
		}
	}
}

void RoyFireball::enter_status_roy_fireball_hover() {
	set_pos(owner->get_bone_position("HaveL", glm::vec3(35.0, 0.0, 0.0)));
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("hover_active_time", params);
	change_anim("hover");
}

void RoyFireball::exit_status_roy_fireball_hover() {
	clear_effect_all();
}

void RoyFireball::status_roy_fireball_punched() {
	if (owner->fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_L) {
		add_pos(get_local_param_float("punch_move_x_speed_l", params) * facing_dir, 0);
	}
	else if (owner->fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_M) {
		add_pos(get_local_param_float("punch_move_x_speed_m", params) * facing_dir, 0);
	}
	else {
		add_pos(get_local_param_float("punch_move_x_speed_h", params) * facing_dir, 0);
	}
	projectile_int[PROJECTILE_INT_ELAPSED_FRAMES]++;
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void RoyFireball::enter_status_roy_fireball_punched() {
	projectile_int[PROJECTILE_INT_OWNER_ENDLAG] = owner->get_frames_until_actionable();
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("punch_active_time", params);
	change_anim("punched");
}

void RoyFireball::exit_status_roy_fireball_punched() {
	clear_effect_all();
}

void RoyFireball::status_roy_fireball_kicked() {
	if (owner->fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_L) {

		add_pos(get_local_param_float("kick_move_x_speed_l", params) * facing_dir, get_local_param_float("kick_move_y_speed_l", params) * -1);
	}
	else if (owner->fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_M) {
		add_pos(get_local_param_float("kick_move_x_speed_m", params) * facing_dir, get_local_param_float("kick_move_y_speed_m", params) * -1);
	}
	else {
		add_pos(get_local_param_float("kick_move_x_speed_h", params) * facing_dir, get_local_param_float("kick_move_y_speed_h", params) * -1);
	}
	projectile_int[PROJECTILE_INT_ELAPSED_FRAMES]++;
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void RoyFireball::enter_status_roy_fireball_kicked() {
	projectile_int[PROJECTILE_INT_OWNER_ENDLAG] = owner->get_frames_until_actionable();
	set_pos(owner->get_bone_position("FootR", glm::vec3(20.0, -25.0, 0.0)));
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