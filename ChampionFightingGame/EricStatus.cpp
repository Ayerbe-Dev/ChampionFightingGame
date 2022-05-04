#include "Eric.h"

void Eric::chara_main() {

}

bool Eric::specific_ground_status_act() {
	if (get_special_input(SPECIAL_KIND_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		return change_status_after_hitlag(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START);
	}
	return false;
}

bool Eric::specific_air_status_act() {
	return false;
}

bool Eric::specific_status_attack() {
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

void Eric::enter_status_dash() {
	change_anim("dash_f");
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
}

void Eric::exit_status_dash() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
}

void Eric::enter_status_dashb() {
	change_anim("dash_b");
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
}

void Eric::exit_status_dashb() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
}

void Eric::eric_status_special_uppercut_start() {
	if (frame >= get_local_param_int("special_uppercut_transition_frame", params) && !fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED_DURING_STATUS]) {
		change_status(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	if (is_actionable() && common_ground_status_act()) {
		return;
	}
}

void Eric::eric_enter_status_special_uppercut_start() {
	change_anim("special_uppercut_start", 1);
}

void Eric::eric_exit_status_special_uppercut_start() {

}

void Eric::eric_status_special_uppercut() {
	if (anim_kind->name == "special_uppercut") {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float_special("special_uppercut_fall_speed") * -1.0) {
				fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float_special("special_uppercut_gravity");
			}
			situation_kind = FIGHTER_SITUATION_AIR;
			add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
			if (is_anim_end) {
				change_status(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL);
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

void Eric::eric_enter_status_special_uppercut() {
	if (situation_kind == FIGHTER_SITUATION_GROUND) {
		change_anim("special_uppercut_rise", 2);
	}
	else {
		change_anim("special_uppercut");
	}

	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_uppercut_x");
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_param_float_special("special_uppercut_init_y");
}

void Eric::eric_exit_status_special_uppercut() {

}

void Eric::eric_status_special_uppercut_fall() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float_special("special_uppercut_fall_speed") * -1.0) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float_special("special_uppercut_gravity");
	}
	add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
}
void Eric::eric_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Eric::eric_exit_status_special_uppercut_fall() {

}

void Eric::loadEricStatusFunctions() {
	status_script.resize(CHARA_ERIC_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ERIC_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ERIC_STATUS_MAX, nullptr);
	
	ADD_FIGHTER_STATUS(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START, &Eric::eric_status_special_uppercut_start);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START, &Eric::eric_enter_status_special_uppercut_start);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START, &Eric::eric_exit_status_special_uppercut_start);

	ADD_FIGHTER_STATUS(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT, &Eric::eric_enter_status_special_uppercut);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT , &Eric::eric_enter_status_special_uppercut);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT , &Eric::eric_exit_status_special_uppercut);

	ADD_FIGHTER_STATUS(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL , &Eric::eric_status_special_uppercut_fall);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL , &Eric::eric_enter_status_special_uppercut_fall);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL , &Eric::eric_exit_status_special_uppercut_fall);
}