#include "Roy.h"
#include "RoyFireball.h"

void Roy::chara_main() {

}

bool Roy::specific_ground_status_act() {
	if (!projectiles[0]->active) {
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
			return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
			return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
			return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
			return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
		}
	}

	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MACRO_K) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_SLIDE);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_LK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_SLIDE);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_SLIDE);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_HK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_SLIDE);
	}

	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
		}
		else {
			return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
		}
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
	}
	return false;
}

bool Roy::specific_air_status_act() {
	return false;
}

bool Roy::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS] || fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED_DURING_STATUS]) {
		if (is_enable_cancel(CANCEL_CAT_CONTACT, CANCEL_KIND_236P)) {
			if (!projectiles[0]->active) {
				if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
					fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
					return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
				}
				if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
					fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
					return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
				}
				if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
					fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
					return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
				}
				if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
					fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
					return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
				}
			}
		}
		if (is_enable_cancel(CANCEL_CAT_CONTACT, CANCEL_KIND_214K)) {
			if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MACRO_K) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
				return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_SLIDE);
			}
			if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_LK) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
				return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_SLIDE);
			}
			if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MK) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
				return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_SLIDE);
			}
			if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_HK) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
				return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_SLIDE);
			}
		}
		if (is_enable_cancel(CANCEL_CAT_CONTACT, CANCEL_KIND_623P)) {
			if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
				if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
					return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
				}
				else {
					return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
				}
			}
			if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
				return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
			}
			if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
				return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
			}
			if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
				return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
			}
		}

		if (normal_cancel(ATTACK_KIND_LP, FIGHTER_SITUATION_GROUND)) {
			return true;
		}
		if (normal_cancel(ATTACK_KIND_MP, FIGHTER_SITUATION_GROUND)) {
			return true;
		}
		if (normal_cancel(ATTACK_KIND_HP, FIGHTER_SITUATION_GROUND)) {
			return true;
		}
		if (normal_cancel(ATTACK_KIND_LP, FIGHTER_SITUATION_AIR)) {
			return true;
		}
		if (normal_cancel(ATTACK_KIND_MP, FIGHTER_SITUATION_AIR)) {
			return true;
		}
		if (normal_cancel(ATTACK_KIND_HP, FIGHTER_SITUATION_AIR)) {
			return true;
		}
	}
	return false;
}

void Roy::roy_status_special_fireball_start() {
	if (frame >= get_param_int_special("special_fireball_transition_frame")) {
		if (check_button_input(BUTTON_LP)) {
			fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_MP)) {
			fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_HP)) {
			fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_LK)) {
			fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_MK)) {
			fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_HK)) {
			fighter_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
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
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_236;
	change_anim("special_fireball_start", 1.0);	
}

void Roy::roy_exit_status_special_fireball_start() {

}


void Roy::roy_status_special_fireball_punch() {
	if (is_status_end()) {
		return;
	}
}

void Roy::roy_enter_status_special_fireball_punch() {
	change_anim("special_fireball_punch", 1.0);
}

void Roy::roy_exit_status_special_fireball_punch() {

}

void Roy::roy_status_special_fireball_kick() {
	if (is_status_end()) {
		return;
	}
}

void Roy::roy_enter_status_special_fireball_kick() {
	change_anim("special_fireball_kick", 1.0);
}

void Roy::roy_exit_status_special_fireball_kick() {

}

void Roy::roy_status_special_slide() {
	if (is_status_end()) {
		return;
	}
	if (frame >= get_local_param_int("special_slide_move_start_frame", params) && frame < get_local_param_int("special_slide_move_stop_frame", params)) {
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, 0.0);
	}
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		if (frame >= get_local_param_int("special_slide_transition_frame", params)) {
			change_status(CHARA_ROY_STATUS_SPECIAL_SLIDE_FOLLOWUP);
		}
	}
}

void Roy::roy_enter_status_special_slide() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_214;
	change_anim("special_slide");
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_slide_move_x");
}

void Roy::roy_exit_status_special_slide() {

}

void Roy::roy_status_special_slide_followup() {
	if (is_status_end()) {
		return;
	}
}

void Roy::roy_enter_status_special_slide_followup() {
	change_anim("special_slide_followup");
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Roy::roy_exit_status_special_slide_followup() {

}

void Roy::roy_status_special_uppercut_start() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS] || (frame >= get_local_param_int("special_uppercut_transition_frame", params) && !fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED_DURING_STATUS])) {
		change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (is_status_end()) {
		return;
	}
}

void Roy::roy_enter_status_special_uppercut_start() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_623;
	change_anim("special_uppercut_start", 1);
}

void Roy::roy_exit_status_special_uppercut_start() {

}

void Roy::roy_status_special_uppercut() {
	if (is_anim_end) {
		change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL);
		return;
	}
	if (frame > 4.0) {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
			situation_kind = FIGHTER_SITUATION_AIR;
			add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
			if (pos.y <= FLOOR_GAMECOORD) {
				change_status(FIGHTER_STATUS_LANDING);
				return;
			}
		}
	}
	else {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, 0);
		}
	}
}

void Roy::roy_enter_status_special_uppercut() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_623;
	if (situation_kind == FIGHTER_SITUATION_GROUND) { //Not sure if we want air dp to be a thing but if we do, this is designed to account for it
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			change_anim("special_uppercut", 1.0, 1.0);
		}
		else {
			change_anim("special_uppercut", 1.0, 1.0);
		}
	}
	else {
		change_anim("special_uppercut", 1.0, 4.0);
	}

	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_uppercut_x");
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_param_float_special("special_uppercut_init_y");
}

void Roy::roy_exit_status_special_uppercut() {

}

void Roy::roy_status_special_uppercut_fall() {
	if (check_landing()) {
		return;
	}
	apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
	add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
}

void Roy::roy_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
}

void Roy::roy_exit_status_special_uppercut_fall() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Roy::loadRoyStatusFunctions() {
	status_script.resize(CHARA_ROY_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ROY_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ROY_STATUS_MAX, nullptr);

	ADD_FIGHTER_STATUS(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START, &Roy::roy_status_special_fireball_start);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START, &Roy::roy_enter_status_special_fireball_start);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START, &Roy::roy_exit_status_special_fireball_start);

	ADD_FIGHTER_STATUS(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH, &Roy::roy_status_special_fireball_punch);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH, &Roy::roy_enter_status_special_fireball_punch);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH, &Roy::roy_exit_status_special_fireball_punch);

	ADD_FIGHTER_STATUS(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK, &Roy::roy_status_special_fireball_kick);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK, &Roy::roy_enter_status_special_fireball_kick);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK, &Roy::roy_exit_status_special_fireball_kick);

	ADD_FIGHTER_STATUS(CHARA_ROY_STATUS_SPECIAL_SLIDE, &Roy::roy_status_special_slide);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROY_STATUS_SPECIAL_SLIDE, &Roy::roy_enter_status_special_slide);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROY_STATUS_SPECIAL_SLIDE, &Roy::roy_exit_status_special_slide);

	ADD_FIGHTER_STATUS(CHARA_ROY_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Roy::roy_status_special_slide_followup);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROY_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Roy::roy_enter_status_special_slide_followup);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROY_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Roy::roy_exit_status_special_slide_followup);

	ADD_FIGHTER_STATUS(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START, &Roy::roy_status_special_uppercut_start);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START, &Roy::roy_enter_status_special_uppercut_start);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START, &Roy::roy_exit_status_special_uppercut_start);

	ADD_FIGHTER_STATUS(CHARA_ROY_STATUS_SPECIAL_UPPERCUT, &Roy::roy_status_special_uppercut);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROY_STATUS_SPECIAL_UPPERCUT, &Roy::roy_enter_status_special_uppercut);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROY_STATUS_SPECIAL_UPPERCUT, &Roy::roy_exit_status_special_uppercut);

	ADD_FIGHTER_STATUS(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL, &Roy::roy_status_special_uppercut_fall);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL, &Roy::roy_enter_status_special_uppercut_fall);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL, &Roy::roy_exit_status_special_uppercut_fall);
}