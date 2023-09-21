#include "RowanFireball.h"
#include "RowanFireballConstants.h"

void RowanFireball::projectile_unique_main() {
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0 && status_kind != PROJECTILE_STATUS_DEACTIVATE) {
		change_status(PROJECTILE_STATUS_DEACTIVATE);
	}
}

void RowanFireball::status_activate() {
	if (owner->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		projectile_int[PROJECTILE_INT_ATTACK_LEVEL] = 1;
		projectile_int[PROJECTILE_INT_HEALTH] = 2;
	}
	change_status(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER);
}

void RowanFireball::status_rowan_fireball_hover() {
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		if (owner->status_kind != CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH
			&& owner->status_kind != CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK) {
			change_status(PROJECTILE_STATUS_DEACTIVATE);
		}
	}
}

void RowanFireball::enter_status_rowan_fireball_hover() {
	set_pos(owner->get_bone_position("HaveL", glm::vec3(35.0, 0.0, 0.0)));
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("hover_active_time", params);
	change_anim("hover");
}

void RowanFireball::exit_status_rowan_fireball_hover() {
	clear_effect_all();
}

void RowanFireball::status_rowan_fireball_punched() {
	add_pos(glm::vec3(projectile_float[PROJECTILE_FLOAT_SPEED_X], projectile_float[PROJECTILE_FLOAT_SPEED_Y], 0));
	projectile_int[PROJECTILE_INT_ELAPSED_FRAMES]++;
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		change_status(PROJECTILE_STATUS_DEACTIVATE);
	}
}

void RowanFireball::enter_status_rowan_fireball_punched() {
	projectile_int[PROJECTILE_INT_OWNER_ENDLAG] = owner->get_frames_until_actionable();
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("punch_active_time", params);
	switch (owner->fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL]) {
		case SPECIAL_LEVEL_L: {
			projectile_float[PROJECTILE_FLOAT_SPEED_X] = get_local_param_float("punch_move_x_speed_l", params) * facing_dir;
		} break;
		case SPECIAL_LEVEL_M: {
			projectile_float[PROJECTILE_FLOAT_SPEED_X] = get_local_param_float("punch_move_x_speed_m", params) * facing_dir;
		} break;
		case SPECIAL_LEVEL_H:
		case SPECIAL_LEVEL_EX: {
			projectile_float[PROJECTILE_FLOAT_SPEED_X] = get_local_param_float("punch_move_x_speed_h", params) * facing_dir;
		} break;
	}
	projectile_float[PROJECTILE_FLOAT_SPEED_Y] = 0.0;
	change_anim("punched");
}

void RowanFireball::exit_status_rowan_fireball_punched() {
	clear_effect("flame");
}

void RowanFireball::status_rowan_fireball_kicked() {
	add_pos(glm::vec3(projectile_float[PROJECTILE_FLOAT_SPEED_X], projectile_float[PROJECTILE_FLOAT_SPEED_Y], 0));
	projectile_int[PROJECTILE_INT_ELAPSED_FRAMES]++;
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		change_status(PROJECTILE_STATUS_DEACTIVATE);
	}
}

void RowanFireball::enter_status_rowan_fireball_kicked() {
	projectile_int[PROJECTILE_INT_OWNER_ENDLAG] = owner->get_frames_until_actionable();
	set_pos(owner->get_bone_position("FootR", glm::vec3(20.0, -25.0, 0.0)));
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("kick_active_time", params);
	switch (owner->fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL]) {
	case SPECIAL_LEVEL_L: {
		projectile_float[PROJECTILE_FLOAT_SPEED_X] = get_local_param_float("kick_move_x_speed_l", params) * facing_dir;
		projectile_float[PROJECTILE_FLOAT_SPEED_Y] = -get_local_param_float("kick_move_y_speed_l", params);
	} break;
	case SPECIAL_LEVEL_M: {
		projectile_float[PROJECTILE_FLOAT_SPEED_X] = get_local_param_float("kick_move_x_speed_m", params) * facing_dir;
		projectile_float[PROJECTILE_FLOAT_SPEED_Y] = -get_local_param_float("kick_move_y_speed_m", params);
	} break;
	case SPECIAL_LEVEL_H:
	case SPECIAL_LEVEL_EX: {
		projectile_float[PROJECTILE_FLOAT_SPEED_X] = get_local_param_float("kick_move_x_speed_h", params) * facing_dir;
		projectile_float[PROJECTILE_FLOAT_SPEED_Y] = -get_local_param_float("kick_move_y_speed_h", params);
	} break;
	}
	change_anim("kicked");
}

void RowanFireball::exit_status_rowan_fireball_kicked() {
	clear_effect("flame");
}

void RowanFireball::load_projectile_unique_status_scripts() {
	status_script.resize(PROJECTILE_ROWAN_FIREBALL_STATUS_MAX, nullptr);
	enter_status_script.resize(PROJECTILE_ROWAN_FIREBALL_STATUS_MAX, nullptr);
	exit_status_script.resize(PROJECTILE_ROWAN_FIREBALL_STATUS_MAX, nullptr);

	ADD_PROJECTILE_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER, &RowanFireball::status_rowan_fireball_hover);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER, &RowanFireball::enter_status_rowan_fireball_hover);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER, &RowanFireball::exit_status_rowan_fireball_hover);

	ADD_PROJECTILE_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_PUNCHED, &RowanFireball::status_rowan_fireball_punched);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_PUNCHED, &RowanFireball::enter_status_rowan_fireball_punched);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_PUNCHED, &RowanFireball::exit_status_rowan_fireball_punched);

	ADD_PROJECTILE_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_KICKED, &RowanFireball::status_rowan_fireball_kicked);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_KICKED, &RowanFireball::enter_status_rowan_fireball_kicked);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_KICKED, &RowanFireball::exit_status_rowan_fireball_kicked);
}