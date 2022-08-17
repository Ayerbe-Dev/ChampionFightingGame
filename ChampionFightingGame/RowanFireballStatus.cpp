#include "RowanFireball.h"
#include "RowanFireballConstants.h"

void RowanFireball::projectile_unique_main() {
	if (projectile_int[PROJECTILE_INT_HEALTH] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void RowanFireball::status_default() {
	if (owner->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		projectile_int[PROJECTILE_INT_HEALTH] = 2;
	}
	change_status(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER);
}

void RowanFireball::status_hit() {
	if (is_anim_end) {
		active = false;
	}
}

void RowanFireball::enter_status_hit() {
	change_anim("hit");
}

void RowanFireball::status_rowan_fireball_hover() {
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		if (owner->status_kind != CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH
			&& owner->status_kind != CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK) {
			change_status(PROJECTILE_ROWAN_FIREBALL_STATUS_FALL);
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
	if (owner->fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_L) {
		add_pos(get_local_param_float("punch_move_x_speed_l", params) * facing_dir, 0);
	}
	else if (owner->fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_M) {
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

void RowanFireball::enter_status_rowan_fireball_punched() {
	projectile_int[PROJECTILE_INT_OWNER_ENDLAG] = owner->get_frames_until_actionable();
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("punch_active_time", params);
	change_anim("punched");
}

void RowanFireball::exit_status_rowan_fireball_punched() {
	clear_effect_all();
}

void RowanFireball::status_rowan_fireball_kicked() {
	if (owner->fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_L) {

		add_pos(get_local_param_float("kick_move_x_speed_l", params) * facing_dir, get_local_param_float("kick_move_y_speed_l", params) * -1);
	}
	else if (owner->fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_M) {
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

void RowanFireball::enter_status_rowan_fireball_kicked() {
	projectile_int[PROJECTILE_INT_OWNER_ENDLAG] = owner->get_frames_until_actionable();
	set_pos(owner->get_bone_position("FootR", glm::vec3(20.0, -25.0, 0.0)));
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_local_param_int("kick_active_time", params);
	change_anim("kicked");
}

void RowanFireball::exit_status_rowan_fireball_kicked() {
	clear_effect_all();
}

void RowanFireball::status_rowan_fireball_fall() {
	if (is_anim_end) {
		change_status(PROJECTILE_ROWAN_FIREBALL_STATUS_GROUND);
	}
}

void RowanFireball::enter_status_rowan_fireball_fall() {
	change_anim("fall");
}

void RowanFireball::exit_status_rowan_fireball_fall() {

}

void RowanFireball::status_rowan_fireball_ground() {
	if (is_anim_end) {
		active = false;
	}
}

void RowanFireball::enter_status_rowan_fireball_ground() {
	change_anim("ground");
}

void RowanFireball::exit_status_rowan_fireball_ground() {

}

void RowanFireball::loadRowanFireballStatusFunctions() {
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

	ADD_PROJECTILE_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_FALL, &RowanFireball::status_rowan_fireball_fall);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_FALL, &RowanFireball::enter_status_rowan_fireball_fall);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_FALL, &RowanFireball::exit_status_rowan_fireball_fall);

	ADD_PROJECTILE_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_GROUND, &RowanFireball::status_rowan_fireball_ground);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_GROUND, &RowanFireball::enter_status_rowan_fireball_ground);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_ROWAN_FIREBALL_STATUS_GROUND, &RowanFireball::exit_status_rowan_fireball_ground);
}