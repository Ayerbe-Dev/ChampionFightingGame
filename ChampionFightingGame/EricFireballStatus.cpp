#include "EricFireball.h"
#include "EricFireballConstants.h"

void EricFireball::unique_process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) {
	if (status_kind == PROJECTILE_ERIC_FIREBALL_STATUS_HOVER) {
		object_int[PROJECTILE_INT_HEALTH]++;
	}
}

void EricFireball::status_move() {
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
		add_pos(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
		object_int[PROJECTILE_INT_ELAPSED_FRAMES]++;
	}
}

void EricFireball::enter_status_move() {
	object_int[PROJECTILE_INT_OWNER_ENDLAG] = owner->get_frames_until_actionable();
	object_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("punch_active_time");
	object_flag[PROJECTILE_FLAG_DESPAWN_ON_OOB] = true;
	change_anim("punched");
	switch (owner->object_int[CHARA_ERIC_INT_FIREBALL_LEVEL]) {
		case SPECIAL_LEVEL_L: {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("punch_move_x_speed_l") * facing_dir;
		} break;
		case SPECIAL_LEVEL_M: {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("punch_move_x_speed_m") * facing_dir;
		} break;
		case SPECIAL_LEVEL_H:
		case SPECIAL_LEVEL_EX: {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("punch_move_x_speed_h") * facing_dir;
		} break;
	}
	switch (owner->object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
		case SPECIAL_LEVEL_L: {
			change_script("punched_l");
		} break;
		case SPECIAL_LEVEL_M: {
			change_script("punched_m");
		} break;
		case SPECIAL_LEVEL_H: {
			change_script("punched_h");

		} break;
		case SPECIAL_LEVEL_EX: {
			change_script("punched_ex");
		} break;
	}
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 0.0;
}

void EricFireball::exit_status_move() {
	clear_effect("flame");
}

void EricFireball::status_eric_fireball_hover() {
	if (object_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		if (owner->status_kind != CHARA_ERIC_STATUS_SPECIAL_FIREBALL_PUNCH
			&& owner->status_kind != CHARA_ERIC_STATUS_SPECIAL_FIREBALL_KICK) {
			if (get_alpha() == 0) {
				deactivate();
			}
			else {
				add_alpha(-25);
			}
		}
	}
}

void EricFireball::enter_status_eric_fireball_hover() {
	set_pos(owner->get_bone_position("HaveL", glm::vec3(35.0, 0.0, 0.0)));
	object_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("hover_active_time");
	change_anim("hover");
	switch (owner->object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
		case (SPECIAL_LEVEL_EX): {
			change_script("hover_ex");
		} break;
		case (SPECIAL_LEVEL_H): {
			change_script("hover_h");
		} break;
		case (SPECIAL_LEVEL_M): {
			change_script("hover_m");
		} break;
		default:
		case (SPECIAL_LEVEL_L): {
			change_script("hover_l");
		} break;
	}
}

void EricFireball::exit_status_eric_fireball_hover() {
	clear_effect_all();
}

void EricFireball::load_projectile_unique_status_scripts() {
	SET_STATUS_FUNC(PROJECTILE_ERIC_FIREBALL_STATUS_HOVER, &EricFireball::status_eric_fireball_hover);
	SET_ENTRY_STATUS_FUNC(PROJECTILE_ERIC_FIREBALL_STATUS_HOVER, &EricFireball::enter_status_eric_fireball_hover);
	SET_EXIT_STATUS_FUNC(PROJECTILE_ERIC_FIREBALL_STATUS_HOVER, &EricFireball::exit_status_eric_fireball_hover);
}