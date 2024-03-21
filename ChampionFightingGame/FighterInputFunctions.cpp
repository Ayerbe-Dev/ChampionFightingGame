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
	if ((buffer_code & BUTTON_UP_BIT) && !(buffer_code & BUTTON_DOWN_BIT)) {
		if (((buffer_code & BUTTON_LEFT_BIT) || (buffer_code & BUTTON_RIGHT_BIT)) && !((buffer_code & BUTTON_LEFT_BIT) && (buffer_code & BUTTON_RIGHT_BIT))) {
			if ((bool)(buffer_code & BUTTON_RIGHT_BIT) == right) {
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
	else if (!(buffer_code & BUTTON_UP_BIT) && (buffer_code & BUTTON_DOWN_BIT)) {
		if (((buffer_code & BUTTON_LEFT_BIT) || (buffer_code & BUTTON_RIGHT_BIT)) && !((buffer_code & BUTTON_LEFT_BIT) && (buffer_code & BUTTON_RIGHT_BIT))) {
			if ((bool)(buffer_code & BUTTON_RIGHT_BIT) == right) {
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
		if (((buffer_code & BUTTON_LEFT_BIT) || (buffer_code & BUTTON_RIGHT_BIT)) && !((buffer_code & BUTTON_LEFT_BIT) && (buffer_code & BUTTON_RIGHT_BIT))) {
			if ((bool)(buffer_code & BUTTON_RIGHT_BIT) == right) {
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