#include "Fighter.h"

bool Fighter::check_button_on(u32 button) {
	return player_info->check_button_on(button);
}

bool Fighter::check_button_input(u32 button) {
	return player_info->check_button_input(button);
}

bool Fighter::check_button_input(u32 button[], int length, int min_matches) {
	return player_info->check_button_input(button, length, min_matches);
}

bool Fighter::check_button_trigger(u32 button) {
	return player_info->check_button_trigger(button);
}

bool Fighter::check_button_release(u32 button) {
	return player_info->check_button_release(button);
}

int Fighter::get_stick_dir() {
	if (check_button_on(BUTTON_UP) && !check_button_on(BUTTON_DOWN)) {
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT))) {
			if (check_button_on(BUTTON_RIGHT) == facing_right) {
				return 9;
			}
			else {
				return 7;
			}
		}
		else {
			return 8;
		}
	}
	else if (check_button_on(BUTTON_DOWN) && !check_button_on(BUTTON_UP)) {
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT))) {
			if (check_button_on(BUTTON_RIGHT) == facing_right) {
				return 3;
			}
			else {
				return 1;
			}
		}
		else {
			return 2;
		}
	}
	else {
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT))) {
			if (check_button_on(BUTTON_RIGHT) == facing_right) {
				return 6;
			}
			else {
				return 4;
			}
		}
		else {
			return 5;
		}
	}
}

int Fighter::get_flick_dir() {
	int stick_dir = get_stick_dir();
	if (stick_dir == prev_stick_dir) {
		return 0;
	}
	else {
		return stick_dir;
	}
}

int Fighter::get_special_input(int special_kind, u32 button, int charge_frames) {
	int button_check = 0;
	bool input_check = false;

	if (button == BUTTON_MACRO_P) {
		u32 ex_buttons[3] = { BUTTON_LP, BUTTON_MP, BUTTON_HP };
		if (check_button_input(ex_buttons, 3, 2)) {
			button_check = SPECIAL_INPUT_NORMAL;
		}
	}
	else if (button == BUTTON_MACRO_K) {
		u32 ex_buttons[3] = { BUTTON_LK, BUTTON_MK, BUTTON_HK };
		if (check_button_input(ex_buttons, 3, 2)) {
			button_check = SPECIAL_INPUT_NORMAL;
		}
	}
	else {
		if (check_button_input(button) || check_button_release(button)) {
			button_check = SPECIAL_INPUT_NORMAL;
		}
	}
	if (button_check) {
		if (special_kind == SPECIAL_KIND_236) {
			if (fighter_int[FIGHTER_INT_236_STEP] == 2 && get_stick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_236_STEP] == 3) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_236_STEP] = 0;
				fighter_int[FIGHTER_INT_236_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_214) {
			if (fighter_int[FIGHTER_INT_214_STEP] == 2 && get_stick_dir() == 4) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_214_STEP] == 3) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_214_STEP] = 0;
				fighter_int[FIGHTER_INT_214_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_623) {
			if (fighter_int[FIGHTER_INT_623_STEP] == 2 && get_stick_dir() == 3) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_623_STEP] == 3) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_623_STEP] = 0;
				fighter_int[FIGHTER_INT_623_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_41236) {
			if (fighter_int[FIGHTER_INT_41236_STEP] == 4 && get_stick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_41236_STEP] == 5) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_41236_STEP] = 0;
				fighter_int[FIGHTER_INT_41236_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_63214) {
			if (fighter_int[FIGHTER_INT_63214_STEP] == 4 && get_stick_dir() == 4) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_63214_STEP] == 5) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_63214_STEP] = 0;
				fighter_int[FIGHTER_INT_63214_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_236236) {
			if (fighter_int[FIGHTER_INT_236236_STEP] == 5 && get_stick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_236236_STEP] == 6) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_236236_STEP] = 0;
				fighter_int[FIGHTER_INT_236236_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_214214) {
			if (fighter_int[FIGHTER_INT_214214_STEP] == 5 && get_stick_dir() == 4) {
				button_check = SPECIAL_INPUT_JUST;
				input_check = true;
			}
			else if (fighter_int[FIGHTER_INT_214214_STEP] == 6) {
				input_check = true;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_214_STEP] = 0;
				fighter_int[FIGHTER_INT_214_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_CHARGE_DOWN) {
			input_check = (fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] >= charge_frames && get_stick_dir() == 8);
			if (input_check && get_flick_dir() == 8) {
				button_check = SPECIAL_INPUT_JUST;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
				fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
				fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER] = 0;
			}
		}
		else if (special_kind == SPECIAL_KIND_CHARGE_BACK) {
			input_check = (fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] >= charge_frames && get_stick_dir() == 6);
			if (input_check && get_flick_dir() == 6) {
				button_check = SPECIAL_INPUT_JUST;
			}
			if (input_check) {
				fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
				fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER] = 0;
			}
		}
	}
	if (input_check) {
		return button_check;
	}
	else {
		return SPECIAL_INPUT_NONE;
	}
}

bool Fighter::get_normal_cancel(int attack_kind, u32 button, int situation_kind, int stick) {
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == attack_kind && check_button_input(button) && situation_kind == this->situation_kind) {
		int prev_attack_kind = fighter_int[FIGHTER_INT_ATTACK_KIND];
		if (button == BUTTON_LP) {
			if (stick < 4 && situation_kind == FIGHTER_SITUATION_GROUND) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CLP;
			}
			else {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LP;
			}
		}
		if (button == BUTTON_MP) {
			if (stick < 4 && situation_kind == FIGHTER_SITUATION_GROUND) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CMP;
			}
			else {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MP;
			}
		}
		if (button == BUTTON_HP) {
			if (stick < 4 && situation_kind == FIGHTER_SITUATION_GROUND) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CHP;
			}
			else {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HP;
			}
		}
		if (button == BUTTON_LK) {
			if (stick < 4 && situation_kind == FIGHTER_SITUATION_GROUND) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CLK;
			}
			else {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LK;
			}
		}
		if (button == BUTTON_MK) {
			if (stick < 4 && situation_kind == FIGHTER_SITUATION_GROUND) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CMK;
			}
			else {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MK;
			}
		}
		if (button == BUTTON_HK) {
			if (stick < 4 && situation_kind == FIGHTER_SITUATION_GROUND) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CHK;
			}
			else {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HK;
			}
		}
		fighter_flag[FIGHTER_FLAG_SELF_CANCEL] = (fighter_int[FIGHTER_INT_ATTACK_KIND] == prev_attack_kind);
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] <= BUFFER_WINDOW) {
			if (situation_kind == FIGHTER_SITUATION_GROUND) {
				return change_status_after_hitlag(FIGHTER_STATUS_ATTACK, true, false);
			}
			else {
				return change_status_after_hitlag(FIGHTER_STATUS_ATTACK_AIR, true, false);
			}
		}
	}

	return false;
}

int Fighter::try_ex(bool punch) {
	u32 no_heavy_ex_buttons[2];
	no_heavy_ex_buttons[0] = punch ? BUTTON_LP : BUTTON_LK;
	no_heavy_ex_buttons[1] = punch ? BUTTON_MP : BUTTON_MK;
	if (fighter_float[FIGHTER_FLOAT_SUPER_METER] >= EX_METER_SIZE / (EX_METER_BARS / 2)) {
		fighter_float[FIGHTER_FLOAT_SUPER_METER] -= EX_METER_SIZE / (EX_METER_BARS / 2);
		return SPECIAL_LEVEL_EX;
	}
	else if (check_button_input(no_heavy_ex_buttons, 2, 2)) {
		return SPECIAL_LEVEL_M;
	}
	else {
		return SPECIAL_LEVEL_H;
	}
}