#include "Roy.h"

void Roy::chara_main() {

}

bool Roy::specific_ground_status_act() {
	if (get_special_input(SPECIAL_KIND_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}

	if (get_special_input(SPECIAL_KIND_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
		}
		else {
			return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
		}
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
	}
	return false;
}

bool Roy::specific_air_status_act() {
	return false;
}

bool Roy::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (situation_kind == FIGHTER_SITUATION_GROUND && specific_ground_status_act()) {
			return true;
		}
		else if (situation_kind == FIGHTER_SITUATION_AIR && specific_air_status_act()) {
			return true;
		}
		else {
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_LP, FIGHTER_SITUATION_GROUND)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_MP, FIGHTER_SITUATION_GROUND)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_MP, BUTTON_HP, FIGHTER_SITUATION_GROUND)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_LP, FIGHTER_SITUATION_AIR)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_MP, FIGHTER_SITUATION_AIR)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_HP, FIGHTER_SITUATION_AIR)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_MP, BUTTON_LP, FIGHTER_SITUATION_AIR)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_MP, BUTTON_HP, FIGHTER_SITUATION_AIR)) {
				return true;
			}
		}
	}
	return false;
}

void Roy::roy_status_special_fireball_start() {
	if (frame >= get_param_int_special("special_fireball_transition_frame")) {
		if (check_button_input(BUTTON_LP)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_MP)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_HP)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_LK)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_MK)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_HK)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
	}
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
}

void Roy::roy_enter_status_special_fireball_start() {
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		change_anim("special_fireball_start", 2);
	}
	else {
		change_anim("special_fireball_start", 3);
	}
}

void Roy::roy_exit_status_special_fireball_start() {

}


void Roy::roy_status_special_fireball_punch() {
	if (is_status_end()) {
		return;
	}
}

void Roy::roy_enter_status_special_fireball_punch() {
	change_anim("special_fireball_punch", 2);
}

void Roy::roy_exit_status_special_fireball_punch() {

}

void Roy::roy_status_special_fireball_kick() {
	if (is_status_end()) {
		return;
	}
}

void Roy::roy_enter_status_special_fireball_kick() {
	situation_kind = FIGHTER_SITUATION_AIR;
	change_anim("special_fireball_kick", 2);
}

void Roy::roy_exit_status_special_fireball_kick() {

}

void Roy::roy_status_special_uppercut_start() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS] || (frame >= get_param_int("special_uppercut_transition_frame", param_table) && !fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED_DURING_STATUS])) {
		change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (is_status_end()) {
		return;
	}
}

void Roy::roy_enter_status_special_uppercut_start() {
	change_anim("special_uppercut_start", 1);
}

void Roy::roy_exit_status_special_uppercut_start() {

}

void Roy::roy_status_special_uppercut() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	if (anim_kind->name == "special_uppercut") {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float_special("special_uppercut_fall_speed") * -1.0) {
				fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float_special("special_uppercut_gravity");
			}
			situation_kind = FIGHTER_SITUATION_AIR;
			add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
			if (is_anim_end) {
				change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL);
				return;
			}
		}
	}
	else {
		if (is_anim_end) {
			change_anim("special_uppercut");
		}
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, 0);
		}
	}
}

void Roy::roy_enter_status_special_uppercut() {
	if (situation_kind == FIGHTER_SITUATION_GROUND) { //Not sure if we want air dp to be a thing but if we do, this is designed to account for it
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			change_anim("special_uppercut_rise", 1, 1);
		}
		else {
			change_anim("special_uppercut_rise", 2, 1);
		}
	}
	else {
		change_anim("special_uppercut");
	}

	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_uppercut_x");
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_param_float_special("special_uppercut_init_y");
}

void Roy::roy_exit_status_special_uppercut() {

}

void Roy::roy_status_special_uppercut_fall() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float_special("special_uppercut_fall_speed") * -1.0) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float_special("special_uppercut_gravity");
	}
	add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
}
void Roy::roy_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Roy::roy_exit_status_special_uppercut_fall() {

}

void Roy::loadRoyStatusFunctions() {
	roy_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_START - FIGHTER_STATUS_MAX] = &Roy::roy_status_special_fireball_start;
	roy_enter_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_START - FIGHTER_STATUS_MAX] = &Roy::roy_enter_status_special_fireball_start;
	roy_exit_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_START - FIGHTER_STATUS_MAX] = &Roy::roy_exit_status_special_fireball_start;

	roy_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH - FIGHTER_STATUS_MAX] = &Roy::roy_status_special_fireball_punch;
	roy_enter_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH - FIGHTER_STATUS_MAX] = &Roy::roy_enter_status_special_fireball_punch;
	roy_exit_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH - FIGHTER_STATUS_MAX] = &Roy::roy_exit_status_special_fireball_punch;

	roy_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK - FIGHTER_STATUS_MAX] = &Roy::roy_status_special_fireball_kick;
	roy_enter_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK - FIGHTER_STATUS_MAX] = &Roy::roy_enter_status_special_fireball_kick;
	roy_exit_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK - FIGHTER_STATUS_MAX] = &Roy::roy_exit_status_special_fireball_kick;

	roy_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START - FIGHTER_STATUS_MAX] = &Roy::roy_status_special_uppercut_start;
	roy_enter_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START - FIGHTER_STATUS_MAX] = &Roy::roy_enter_status_special_uppercut_start;
	roy_exit_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START - FIGHTER_STATUS_MAX] = &Roy::roy_exit_status_special_uppercut_start;

	roy_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT - FIGHTER_STATUS_MAX] = &Roy::roy_status_special_uppercut;
	roy_enter_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT - FIGHTER_STATUS_MAX] = &Roy::roy_enter_status_special_uppercut;
	roy_exit_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT - FIGHTER_STATUS_MAX] = &Roy::roy_exit_status_special_uppercut;

	roy_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL - FIGHTER_STATUS_MAX] = &Roy::roy_status_special_uppercut_fall;
	roy_enter_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL - FIGHTER_STATUS_MAX] = &Roy::roy_enter_status_special_uppercut_fall;
	roy_exit_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL - FIGHTER_STATUS_MAX] = &Roy::roy_exit_status_special_uppercut_fall;
}