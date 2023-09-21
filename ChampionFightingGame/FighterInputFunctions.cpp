#include "Fighter.h"

bool Fighter::check_button_on(unsigned int button) {
	return player->controller.check_button_on(button);
}

bool Fighter::check_button_input(unsigned int button) {
	return player->controller.check_button_input(button);
}

bool Fighter::check_button_input(unsigned int button[], int length, int min_matches) {
	return player->controller.check_button_input(button, length, min_matches);
}

bool Fighter::check_button_trigger(unsigned int button) {
	return player->controller.check_button_trigger(button);
}

bool Fighter::check_button_release(unsigned int button) {
	return player->controller.check_button_release(button);
}

unsigned int Fighter::get_stick_dir(bool internal_dir) {
	bool right = internal_facing_right;
	if (!internal_dir) {
		right = facing_right;
	}

	if (check_button_on(BUTTON_UP) && !check_button_on(BUTTON_DOWN)) {
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT))) {
			if (check_button_on(BUTTON_RIGHT) == right) {
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
			if (check_button_on(BUTTON_RIGHT) == right) {
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
			if (check_button_on(BUTTON_RIGHT) == right) {
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

unsigned int Fighter::get_stick_dir_no_lr() {
	if (check_button_on(BUTTON_UP) && !check_button_on(BUTTON_DOWN)) {
		if ((check_button_on(BUTTON_RIGHT) || check_button_on(BUTTON_LEFT)) && !(check_button_on(BUTTON_RIGHT) && check_button_on(BUTTON_LEFT))) {
			if (check_button_on(BUTTON_RIGHT)) {
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
			if (check_button_on(BUTTON_RIGHT)) {
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
			if (check_button_on(BUTTON_RIGHT)) {
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

unsigned int Fighter::get_flick_dir(bool internal_dir) {
	int stick_dir = get_stick_dir(internal_dir);
	if (stick_dir == prev_stick_dir) {
		return 0;
	}
	else {
		return stick_dir;
	}
}

bool Fighter::get_attack_input(int attack_kind, unsigned int button, int stick_dir) {
	if (attack_kind >= ATTACK_KIND_OTHER) {
		if (attack_kind == ATTACK_KIND_OTHER) {
			return check_button_input(button) && get_stick_dir() == stick_dir;
		}
		return false;
	}

	bool button_check;
	bool stick_check;
	
	button_check = check_button_input((attack_kind % 6) + 8);
	stick_check = (attack_kind >= 6) == (get_stick_dir() < 4); 
	return button_check && (stick_check || situation_kind != FIGHTER_SITUATION_GROUND);
}

int Fighter::get_special_input(int special_kind, unsigned int button, int charge_frames) {
	if (special_kind < ATTACK_KIND_SPECIAL_236) {
		return false;
	}

	int button_check = 0;
	bool input_check = false;

	if (button == BUTTON_MACRO_P) {
		unsigned int ex_buttons[3] = { BUTTON_LP, BUTTON_MP, BUTTON_HP };
		if (check_button_input(ex_buttons, 3, 2)) {
			button_check = SPECIAL_INPUT_NORMAL;
		}
	}
	else if (button == BUTTON_MACRO_K) {
		unsigned int ex_buttons[3] = { BUTTON_LK, BUTTON_MK, BUTTON_HK };
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
		switch (special_kind) {
			case (ATTACK_KIND_SPECIAL_236): {
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
			} break;
			case (ATTACK_KIND_SPECIAL_214): {
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
			} break;
			case (ATTACK_KIND_SPECIAL_623): {
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
			} break;
			case (ATTACK_KIND_SPECIAL_41236): {
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
			} break;
			case (ATTACK_KIND_SPECIAL_63214): {
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
			} break;
			case (ATTACK_KIND_SPECIAL_632): {
				if (fighter_int[FIGHTER_INT_632_STEP] == 2 && get_stick_dir() == 2) {
					button_check = SPECIAL_INPUT_JUST;
					input_check = true;
				}
				else if (fighter_int[FIGHTER_INT_632_STEP] == 3) {
					input_check = true;
				}
				if (input_check) {
					fighter_int[FIGHTER_INT_632_STEP] = 0;
					fighter_int[FIGHTER_INT_632_TIMER] = 0;
				}
			} break;
			case (ATTACK_KIND_SPECIAL_22): {
				if (fighter_int[FIGHTER_INT_22_STEP] == 2 && get_stick_dir() == 2) {
					button_check = SPECIAL_INPUT_JUST;
					input_check = true;
				}
				else if (fighter_int[FIGHTER_INT_22_STEP] == 3) {
					input_check = true;
				}
				if (input_check) {
					fighter_int[FIGHTER_INT_22_STEP] = 0;
					fighter_int[FIGHTER_INT_22_TIMER] = 0;
				}
			} break;
			case (ATTACK_KIND_SPECIAL_28): {
				input_check = (fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] >= charge_frames && get_stick_dir() > 6);
				if (input_check && get_flick_dir() > 6) {
					button_check = SPECIAL_INPUT_JUST;
				}
				if (input_check) {
					fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
					fighter_int[FIGHTER_INT_DOWN_CHARGE_FRAMES] = 0;
					fighter_int[FIGHTER_INT_DOWN_CHARGE_TIMER] = 0;
				}
			} break;
			case (ATTACK_KIND_SPECIAL_46): {
				input_check = (fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] >= charge_frames && get_stick_dir() == 6);
				if (input_check && get_flick_dir() == 6) {
					button_check = SPECIAL_INPUT_JUST;
				}
				if (input_check) {
					fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
					fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER] = 0;
				}
			} break;
			case (ATTACK_KIND_SUPER_236236): {
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
					fighter_int[FIGHTER_INT_236_STEP] = 0;
					fighter_int[FIGHTER_INT_236_TIMER] = 0;
				}
			} break;
			case(ATTACK_KIND_SUPER_214214): {
				if (fighter_int[FIGHTER_INT_214214_STEP] == 5 && get_stick_dir() == 4) {
					button_check = SPECIAL_INPUT_JUST;
					input_check = true;
				}
				else if (fighter_int[FIGHTER_INT_214214_STEP] == 6) {
					input_check = true;
				}
				if (input_check) {
					fighter_int[FIGHTER_INT_214214_STEP] = 0;
					fighter_int[FIGHTER_INT_214214_TIMER] = 0;
					fighter_int[FIGHTER_INT_214_STEP] = 0;
					fighter_int[FIGHTER_INT_214_TIMER] = 0;
				}
			} break;
			case (ATTACK_KIND_SUPER_4646): {
				if (fighter_int[FIGHTER_INT_4646_STEP] == 3 && get_stick_dir() == 6) {
					button_check = SPECIAL_INPUT_JUST;
					input_check = true;
				}
				else if (fighter_int[FIGHTER_INT_4646_STEP] == 4) {
					input_check = true;
				}
				if (input_check) {
					fighter_int[FIGHTER_INT_4646_STEP] = 0;
					fighter_int[FIGHTER_INT_4646_TIMER] = 0;
				}
			} break;
			default: {

			} break;
		}
	}
	if (input_check) {
		return button_check;
	}
	else {
		return SPECIAL_INPUT_NONE;
	}
}

bool Fighter::attack_cancel(int attack_kind, unsigned int button, int stick) {
	if (get_attack_input(attack_kind, button, stick)) {
		if (is_enable_cancel(attack_kind)) {
			if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] <= get_param_int(PARAM_FIGHTER, "buffer_window")) {
				fighter_flag[FIGHTER_FLAG_SELF_CANCEL] = (fighter_int[FIGHTER_INT_ATTACK_KIND] == attack_kind);
				fighter_int[FIGHTER_INT_ATTACK_KIND] = attack_kind;
				if (situation_kind == FIGHTER_SITUATION_GROUND) {
					return change_status_after_hitlag(FIGHTER_STATUS_ATTACK, true, false);
				}
				else {
					return change_status_after_hitlag(FIGHTER_STATUS_ATTACK_AIR, true, false);
				}
			}
		}
		else {
			fighter_int[FIGHTER_INT_PRE_ENABLE_ATTACK_KIND] = attack_kind;
			fighter_flag[FIGHTER_FLAG_SELF_CANCEL] = (fighter_int[FIGHTER_INT_ATTACK_KIND] == fighter_int[FIGHTER_INT_PRE_ENABLE_ATTACK_KIND]);
			if (situation_kind == FIGHTER_SITUATION_GROUND) {
				return buffer_status_pre_enable_cancel(FIGHTER_STATUS_ATTACK, attack_kind, true, false);
			}
			else {
				return buffer_status_pre_enable_cancel(FIGHTER_STATUS_ATTACK_AIR, attack_kind, true, false);
			}
		}
	}
	return false;
}

int Fighter::try_ex(bool punch) {
	unsigned int no_heavy_ex_buttons[2];
	no_heavy_ex_buttons[0] = punch ? BUTTON_LP : BUTTON_LK;
	no_heavy_ex_buttons[1] = punch ? BUTTON_MP : BUTTON_MK;
	if (has_meter(2)) {
		return SPECIAL_LEVEL_EX;
	}
	else if (check_button_input(no_heavy_ex_buttons, 2, 2)) {
		return SPECIAL_LEVEL_M;
	}
	else {
		return SPECIAL_LEVEL_H;
	}
}