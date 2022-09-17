#include "Rowan.h"
#include "RowanFireball.h"

void Rowan::chara_main() {

}

bool Rowan::specific_ground_status_act() {
	if (!projectiles[0]->active) {
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
		}
	}

	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MACRO_K) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_LK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_HK) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
	}

	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START);
		}
		else {
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
		}
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
	}
	if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
	}
	return false;
}

bool Rowan::specific_air_status_act() {
	return false;
}

bool Rowan::specific_status_attack() {
	if (is_enable_cancel(CANCEL_KIND_236P)) {
		if (!projectiles[0]->active) {
			if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
				return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
			}
			if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
				return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
			}
			if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
				return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
			}
			if (get_special_input(ATTACK_KIND_SPECIAL_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
				fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
				return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START);
			}
		}
	}
	if (is_enable_cancel(CANCEL_KIND_214K)) {
		if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MACRO_K) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_LK) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_MK) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_214, BUTTON_HK) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_SLIDE);
		}
	}
	if (is_enable_cancel(CANCEL_KIND_623P)) {
		if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = try_ex(true);
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START);
			}
			else {
				return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
			}
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
		}
		if (get_special_input(ATTACK_KIND_SPECIAL_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
			fighter_int[FIGHTER_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
			return change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
		}
	}
	return false;
}

void Rowan::rowan_status_special_fireball_start() {
	if (frame >= get_param_int_special("special_fireball_transition_frame")) {
		if (check_button_input(BUTTON_LP)) {
			fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_MP)) {
			fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_HP)) {
			fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_LK)) {
			fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_MK)) {
			fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_HK)) {
			fighter_int[CHARA_ROWAN_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
	}
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
}

void Rowan::rowan_enter_status_special_fireball_start() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_236;
	change_anim("special_fireball_start", 1.0);	
}

void Rowan::rowan_exit_status_special_fireball_start() {

}


void Rowan::rowan_status_special_fireball_punch() {
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_fireball_punch() {
	change_anim("special_fireball_punch", 1.0);
}

void Rowan::rowan_exit_status_special_fireball_punch() {

}

void Rowan::rowan_status_special_fireball_kick() {
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_fireball_kick() {
	change_anim("special_fireball_kick", 1.0);
}

void Rowan::rowan_exit_status_special_fireball_kick() {

}

void Rowan::rowan_status_special_slide() {
	if (is_status_end()) {
		return;
	}
	if (frame >= get_local_param_int("special_slide_move_start_frame", params) && frame < get_local_param_int("special_slide_move_stop_frame", params)) {
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, 0.0);
	}
	if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		if (frame >= get_local_param_int("special_slide_transition_frame", params)) {
			change_status(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP);
		}
	}
}

void Rowan::rowan_enter_status_special_slide() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_214;
	change_anim("special_slide");
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_slide_move_x");
}

void Rowan::rowan_exit_status_special_slide() {

}

void Rowan::rowan_status_special_slide_followup() {
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_slide_followup() {
	change_anim("special_slide_followup");
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Rowan::rowan_exit_status_special_slide_followup() {

}

void Rowan::rowan_status_special_uppercut_start() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED] || (frame >= get_local_param_int("special_uppercut_transition_frame", params) && !fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED])) {
		change_status_after_hitlag(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (is_status_end()) {
		return;
	}
}

void Rowan::rowan_enter_status_special_uppercut_start() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_SPECIAL_623;
	change_anim("special_uppercut_start", 1);
}

void Rowan::rowan_exit_status_special_uppercut_start() {

}

void Rowan::rowan_status_special_uppercut() {
	if (is_anim_end) {
		change_status(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL);
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

void Rowan::rowan_enter_status_special_uppercut() {
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

void Rowan::rowan_exit_status_special_uppercut() {

}

void Rowan::rowan_status_special_uppercut_fall() {
	if (check_landing()) {
		return;
	}
	apply_gravity(get_param_float_special("special_uppercut_gravity"), get_param_float_special("special_uppercut_fall_speed"));
	add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
}

void Rowan::rowan_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
}

void Rowan::rowan_exit_status_special_uppercut_fall() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Rowan::load_chara_status_scripts() {
	status_script.resize(CHARA_ROWAN_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ROWAN_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ROWAN_STATUS_MAX, nullptr);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, &Rowan::rowan_status_special_fireball_start);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, &Rowan::rowan_enter_status_special_fireball_start);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, &Rowan::rowan_exit_status_special_fireball_start);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH, &Rowan::rowan_status_special_fireball_punch);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH, &Rowan::rowan_enter_status_special_fireball_punch);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH, &Rowan::rowan_exit_status_special_fireball_punch);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK, &Rowan::rowan_status_special_fireball_kick);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK, &Rowan::rowan_enter_status_special_fireball_kick);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK, &Rowan::rowan_exit_status_special_fireball_kick);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, &Rowan::rowan_status_special_slide);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, &Rowan::rowan_enter_status_special_slide);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE, &Rowan::rowan_exit_status_special_slide);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Rowan::rowan_status_special_slide_followup);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Rowan::rowan_enter_status_special_slide_followup);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP, &Rowan::rowan_exit_status_special_slide_followup);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, &Rowan::rowan_status_special_uppercut_start);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, &Rowan::rowan_enter_status_special_uppercut_start);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, &Rowan::rowan_exit_status_special_uppercut_start);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, &Rowan::rowan_status_special_uppercut);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, &Rowan::rowan_enter_status_special_uppercut);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT, &Rowan::rowan_exit_status_special_uppercut);

	ADD_FIGHTER_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL, &Rowan::rowan_status_special_uppercut_fall);
	ADD_FIGHTER_ENTRY_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL, &Rowan::rowan_enter_status_special_uppercut_fall);
	ADD_FIGHTER_EXIT_STATUS(CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL, &Rowan::rowan_exit_status_special_uppercut_fall);
}