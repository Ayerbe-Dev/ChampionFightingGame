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

unsigned int Fighter::get_buffer_stick_dir(bool internal_dir) {
	short buffer_code = player->controller.get_buffer_code();
	bool right = internal_facing_right;
	if (!internal_dir) {
		right = facing_right;
	}
	if ((buffer_code & BUFFER_STICK_U) && !(buffer_code & BUFFER_STICK_D)) {
		if (((buffer_code & BUFFER_STICK_L) || (buffer_code & BUFFER_STICK_R)) && !((buffer_code & BUFFER_STICK_L) && (buffer_code & BUFFER_STICK_R))) {
			if ((bool)(buffer_code & BUFFER_STICK_R) == right) {
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
	else if (!(buffer_code & BUFFER_STICK_U) && (buffer_code & BUFFER_STICK_D)) {
		if (((buffer_code & BUFFER_STICK_L) || (buffer_code & BUFFER_STICK_R)) && !((buffer_code & BUFFER_STICK_L) && (buffer_code & BUFFER_STICK_R))) {
			if ((bool)(buffer_code & BUFFER_STICK_R) == right) {
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
		if (((buffer_code & BUFFER_STICK_L) || (buffer_code & BUFFER_STICK_R)) && !((buffer_code & BUFFER_STICK_L) && (buffer_code & BUFFER_STICK_R))) {
			if ((bool)(buffer_code & BUFFER_STICK_R) == right) {
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

bool Fighter::get_attack_input(int attack_kind, unsigned int button, int stick_dir) {
	if (attack_kind >= ATTACK_KIND_OTHER) {
		if (attack_kind == ATTACK_KIND_OTHER) {
			if (check_button_input(button) && get_buffer_stick_dir() == stick_dir) {
				if (is_actionable() && check_button_trigger(button)) {
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
				}
				return true;
			}
		}
		return false;
	}


	bool button_check;
	bool stick_check;
	
	button_check = check_button_input((attack_kind % 6) + 8);
	stick_check = (attack_kind >= 6) == (get_buffer_stick_dir() < 4); 
	if (button_check && (stick_check || situation_kind != FIGHTER_SITUATION_GROUND)) {
		if (is_actionable() && check_button_trigger((attack_kind % 6) + 8)) {
			fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
		}
		return true;
	}
	return false;
}

bool Fighter::get_special_input(int special_kind, unsigned int button, int charge_frames) {
	if (special_kind < ATTACK_KIND_SPECIAL_236) {
		return false;
	}

	int button_check = 0;
	bool input_check = false;

	if (button == BUTTON_3P) {
		unsigned int ex_buttons[3] = { BUTTON_LP, BUTTON_MP, BUTTON_HP };
		if (check_button_input(ex_buttons, 3, 2)) {
			button_check = true;
		}
	}
	else if (button == BUTTON_3K) {
		unsigned int ex_buttons[3] = { BUTTON_LK, BUTTON_MK, BUTTON_HK };
		if (check_button_input(ex_buttons, 3, 2)) {
			button_check = true;
		}
	}
	else {
		if (check_button_input(button) || check_button_release(button)) {
			button_check = true;
		}
	}
	if (button_check) {
		switch (special_kind) {
			case (ATTACK_KIND_SPECIAL_236): {
				if (fighter_int[FIGHTER_INT_236_STEP] == 2 && get_stick_dir() == 6) {
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
				}
				if (input_check) {
					fighter_int[FIGHTER_INT_BACK_CHARGE_FRAMES] = 0;
					fighter_int[FIGHTER_INT_BACK_CHARGE_TIMER] = 0;
				}
			} break;
			case (ATTACK_KIND_SUPER_236236): {
				if (fighter_int[FIGHTER_INT_236236_STEP] == 5 && get_stick_dir() == 6) {
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
					fighter_flag[FIGHTER_FLAG_JUST_FRAME] = true;
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
	return input_check && button_check;
}

bool Fighter::attack_cancel(int attack_kind, unsigned int button, int stick) {
	//A buffered special cancel cannot be overwritten by a buffered normal cancel
	if (fighter_int[FIGHTER_INT_BUFFER_ATTACK_KIND] > ATTACK_KIND_OTHER) {
		return false;
	}
	if (get_attack_input(attack_kind, button, stick)) {
		fighter_flag[FIGHTER_FLAG_SELF_CANCEL] = (fighter_int[FIGHTER_INT_ATTACK_KIND] == attack_kind);
		fighter_int[FIGHTER_INT_BUFFER_ATTACK_KIND] = attack_kind;
		
		if (situation_kind == FIGHTER_SITUATION_GROUND) {
			if (attack_kind == ATTACK_KIND_OTHER) {
				fighter_int[FIGHTER_INT_BUFFER_ATTACK_OTHER_INFO] = get_attack_other_kind_ground(button, stick);
			}
			return buffer_change_status(FIGHTER_STATUS_ATTACK, attack_kind, true, false);
		}
		else {
			if (attack_kind == ATTACK_KIND_OTHER) {
				fighter_int[FIGHTER_INT_BUFFER_ATTACK_OTHER_INFO] = get_attack_other_kind_air(button, stick);
			}
			return buffer_change_status(FIGHTER_STATUS_ATTACK_AIR, attack_kind, true, false);
		}
	}
	return false;
}

bool Fighter::special_cancel(int special_kind, unsigned int status_kind, unsigned int button, int charge_frames) {
	//ATTACK_KIND_SPECIAL_236 and CANCEL_KIND_236P are both 13, so we can calculate the cancel kind by
	//doing this:
	unsigned int cancel_kind = (special_kind - ATTACK_KIND_SPECIAL_236) * 2 + ATTACK_KIND_SPECIAL_236;
	int special_level = SPECIAL_LEVEL_MAX;
	switch (button) { //From there, we add 1 to the cancel kind if it was a kick. We also do some
		//fallthrough shenanigans to calc the special level.

		//Note that this entire system would immediately fall apart if ATTACK_KIND_SPECIAL_236 did not
		//equal CANCEL_KIND_236P.
		case (BUTTON_LK): {
			cancel_kind++;
			[[fallthrough]];
			case (BUTTON_LP): {
				special_level = SPECIAL_LEVEL_L;
			}
		} break;
		case (BUTTON_MK): {
			cancel_kind++;
			[[fallthrough]];
			case (BUTTON_MP): {
				special_level = SPECIAL_LEVEL_M;
			}
		} break;

		case (BUTTON_HK): {
			cancel_kind++;
			[[fallthrough]];
			case (BUTTON_HP): {
				special_level = SPECIAL_LEVEL_H;
			}
		} break;
		case (BUTTON_3K): {
			cancel_kind++;
			[[fallthrough]];
			case (BUTTON_3P): {
				if (!can_spend_ex(200.0)) return false;
				special_level = SPECIAL_LEVEL_EX;
			}
		} break;

	}
	if (get_special_input(special_kind, button, charge_frames)) {
		fighter_int[FIGHTER_INT_BUFFER_ATTACK_KIND] = special_kind;
		fighter_int[FIGHTER_INT_BUFFER_ATTACK_OTHER_INFO] = special_level;
		return buffer_change_status(status_kind, cancel_kind);
	}
	return false;
}

int Fighter::try_ex(bool punch) {
	unsigned int no_heavy_ex_buttons[2];
	no_heavy_ex_buttons[0] = punch ? BUTTON_LP : BUTTON_LK;
	no_heavy_ex_buttons[1] = punch ? BUTTON_MP : BUTTON_MK;
	if (can_spend_ex(200.0)) {
		return SPECIAL_LEVEL_EX;
	}
	else if (check_button_input(no_heavy_ex_buttons, 2, 2)) {
		return SPECIAL_LEVEL_M;
	}
	else {
		return SPECIAL_LEVEL_H;
	}
}

unsigned int Fighter::get_attack_other_kind_ground(unsigned int button, unsigned int stick) {
	if (!attack_other_map_ground.contains(button)) {
		return 0;
	}
	if (!attack_other_map_ground[button].contains(stick)) {
		return 0;
	}
	return attack_other_map_ground[button][stick];
}

unsigned int Fighter::get_attack_other_kind_air(unsigned int button, unsigned int stick) {
	if (!attack_other_map_air.contains(button)) {
		return 0;
	}
	if (!attack_other_map_air[button].contains(stick)) {
		return 0;
	}
	return attack_other_map_air[button][stick];
}