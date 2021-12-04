#include "PlayerInfo.h"
extern int registered_controllers[4];

PlayerInfo::PlayerInfo() {
}

PlayerInfo::PlayerInfo(int id) {
	this->id = id;
	chara_kind = CHARA_KIND_MAX;
	stage = Stage(STAGE_KIND_TRAINING_OLD, "training_room_old"); //Todo: Overwrite this value while on the stage select
	update_controller();
	set_default_button_mappings(id);
}

void PlayerInfo::update_controller() {
	SDL_GameController* new_controller;
	for (int i = 0; i < SDL_NumJoysticks(); ++i) { //Check all plugged in controllers
		if (SDL_IsGameController(i)) {
			bool unregistered = registered_controllers[!id] != i; //Make sure that the other player isn't already using it
			if (unregistered && controller == NULL) { //Only map a controller if we don't already have one
				new_controller = SDL_GameControllerOpen(i); 
				if (is_any_controller_input(new_controller)) {
					/*
					If we don't have a controller yet, we found a controller that the opponent isn't using, AND that controller is
					making inputs, register it to the first player that asked
					*/
					controller = new_controller;
					registered_controllers[id] = i;
				}
			}
		}
	}
	if (controller != NULL && !SDL_GameControllerGetAttached(controller)) {
		controller = NULL;
	}
}

void PlayerInfo::update_buttons(const Uint8* keyboard_state) {
	update_controller();
	for (int i = 0; i < BUTTON_MAX; i++) {
		bool old_button = button_info[i].button_on;
		if (controller != NULL) {
			if (i < 8) {
				if (i == BUTTON_UP || i == BUTTON_MENU_UP) { //Up
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) <= -13106);
				}
				else if (i == BUTTON_DOWN || i == BUTTON_MENU_DOWN) { //Down
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) >= 13106);
				}
				else if (i == BUTTON_LEFT || i == BUTTON_MENU_LEFT) { //Left
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) <= -13106);
				}
				else { //Right
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) >= 13106);
				}
			}
			else {
				button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping)
					|| SDL_GameControllerGetAxis(controller, button_info[i].axis) >= 13106);
			}
		}
		else {
			button_info[i].button_on = keyboard_state[button_info[i].mapping];
		}
		bool new_button = button_info[i].button_on;
		button_info[i].changed = (old_button != new_button);
		if (i == BUTTON_MACRO_P && button_info[i].changed) {
			if (new_button) {
				for (int i = BUTTON_LP; i < BUTTON_HP + 1; i++) {
					bool button_already_changed = button_info[i].changed;
					button_info[i].changed = !button_info[i].button_on;
					button_info[i].button_on = true;
					if (button_info[i].changed || button_already_changed) {
						move_to_front(buffer_order, i);
						button_info[i].buffer = BUFFER_WINDOW;
					}
				}
			}
		}
		if (i == BUTTON_MACRO_K && button_info[i].changed) {
			if (new_button) {
				for (int i = BUTTON_LK; i < BUTTON_HK + 1; i++) {
					bool button_already_changed = button_info[i].changed;
					button_info[i].changed = !button_info[i].button_on;
					button_info[i].button_on = true;
					if (button_info[i].changed || button_already_changed) {
						move_to_front(buffer_order, i);
						button_info[i].buffer = BUFFER_WINDOW;
					}
				}
			}
		}
		button_info[i].buffer = clamp(0, button_info[i].buffer - 1, button_info[i].buffer);
		if (button_info[i].changed && button_info[i].button_on && is_valid_buffer_button(i)) {
			move_to_front(buffer_order, i);
			button_info[i].buffer = BUFFER_WINDOW;
		}
	}
}

void PlayerInfo::set_default_button_mappings(int id) {
	if (id == 0) {
		button_info[BUTTON_UP].mapping = SDL_SCANCODE_W;
		button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_A;
		button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_S;
		button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_D;
		button_info[BUTTON_LP].mapping = SDL_SCANCODE_Y;
		button_info[BUTTON_MP].mapping = SDL_SCANCODE_U;
		button_info[BUTTON_HP].mapping = SDL_SCANCODE_I;
		button_info[BUTTON_LK].mapping = SDL_SCANCODE_H;
		button_info[BUTTON_MK].mapping = SDL_SCANCODE_J;
		button_info[BUTTON_HK].mapping = SDL_SCANCODE_K;
		button_info[BUTTON_MACRO_P].mapping = SDL_SCANCODE_O;
		button_info[BUTTON_MACRO_K].mapping = SDL_SCANCODE_L;
		button_info[BUTTON_START].mapping = SDL_SCANCODE_SPACE;

		button_info[BUTTON_MENU_UP].mapping = SDL_SCANCODE_W;
		button_info[BUTTON_MENU_LEFT].mapping = SDL_SCANCODE_A;
		button_info[BUTTON_MENU_DOWN].mapping = SDL_SCANCODE_S;
		button_info[BUTTON_MENU_RIGHT].mapping = SDL_SCANCODE_D;
		button_info[BUTTON_MENU_SELECT].mapping = SDL_SCANCODE_Y;
		button_info[BUTTON_MENU_BACK].mapping = SDL_SCANCODE_U;
		button_info[BUTTON_MENU_START].mapping = SDL_SCANCODE_SPACE;
	}
	else {
		button_info[BUTTON_UP].mapping = SDL_SCANCODE_UP;
		button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_DOWN;
		button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_LEFT;
		button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_RIGHT;
		button_info[BUTTON_LP].mapping = SDL_SCANCODE_Z;
		button_info[BUTTON_MP].mapping = SDL_SCANCODE_X;
		button_info[BUTTON_HP].mapping = SDL_SCANCODE_C;
		button_info[BUTTON_LK].mapping = SDL_SCANCODE_V;
		button_info[BUTTON_MK].mapping = SDL_SCANCODE_B;
		button_info[BUTTON_HK].mapping = SDL_SCANCODE_N;
		button_info[BUTTON_MACRO_P].mapping = SDL_SCANCODE_M;
		button_info[BUTTON_MACRO_K].mapping = SDL_SCANCODE_COMMA;
		button_info[BUTTON_START].mapping = SDL_SCANCODE_RETURN;

		button_info[BUTTON_MENU_UP].mapping = SDL_SCANCODE_UP;
		button_info[BUTTON_MENU_LEFT].mapping = SDL_SCANCODE_LEFT;
		button_info[BUTTON_MENU_DOWN].mapping = SDL_SCANCODE_DOWN;
		button_info[BUTTON_MENU_RIGHT].mapping = SDL_SCANCODE_RIGHT;
		button_info[BUTTON_MENU_SELECT].mapping = SDL_SCANCODE_Z;
		button_info[BUTTON_MENU_BACK].mapping = SDL_SCANCODE_X;
		button_info[BUTTON_MENU_START].mapping = SDL_SCANCODE_RETURN;
	}

	button_info[BUTTON_UP].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_UP;
	button_info[BUTTON_LEFT].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	button_info[BUTTON_DOWN].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	button_info[BUTTON_RIGHT].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	button_info[BUTTON_LP].c_mapping = SDL_CONTROLLER_BUTTON_A;
	button_info[BUTTON_MP].c_mapping = SDL_CONTROLLER_BUTTON_X;
	button_info[BUTTON_HP].c_mapping = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	button_info[BUTTON_LK].c_mapping = SDL_CONTROLLER_BUTTON_B;
	button_info[BUTTON_MK].c_mapping = SDL_CONTROLLER_BUTTON_Y;
	button_info[BUTTON_HK].c_mapping = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
	button_info[BUTTON_MACRO_P].axis = SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
	button_info[BUTTON_MACRO_K].axis = SDL_CONTROLLER_AXIS_TRIGGERLEFT;
	button_info[BUTTON_START].c_mapping = SDL_CONTROLLER_BUTTON_START;

	button_info[BUTTON_MENU_UP].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_UP;
	button_info[BUTTON_MENU_LEFT].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
	button_info[BUTTON_MENU_DOWN].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	button_info[BUTTON_MENU_RIGHT].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	button_info[BUTTON_MENU_SELECT].c_mapping = SDL_CONTROLLER_BUTTON_A;
	button_info[BUTTON_MENU_BACK].c_mapping = SDL_CONTROLLER_BUTTON_B;
	button_info[BUTTON_MENU_START].c_mapping = SDL_CONTROLLER_BUTTON_START;
}

bool PlayerInfo::check_button_on(u32 button) {
	return button_info[button].button_on;
}

bool PlayerInfo::check_button_input(u32 button) {
	return buffer_order[0] == button && button_info[button].buffer > 0;
}

bool PlayerInfo::check_button_input(u32 button[], int length, int min_matches) {
	if (min_matches == 0) {
		min_matches = length;
	}
	int matches = 0;
	for (int i = 0; i < length; i++) {
		for (int o = 0; o < length; o++) {
			if (buffer_order[o] == button[i] && button_info[button[i]].buffer > 0) {
				matches += 1;
				break;
			}
		}
	}
	return matches >= min_matches;
}

bool PlayerInfo::check_button_trigger(u32 button) {
	return  button_info[button].button_on && button_info[button].changed;
}

bool PlayerInfo::check_button_release(u32 button) {
	return button_info[button].changed && (!button_info[button].button_on);
}

void PlayerInfo::move_to_front(u32 buttons[6], u32 button) {
	int button_index = 0;
	for (int i = 0; i < 6; i++) {
		if (buttons[i] == button) {
			if (i == 0) {
				return;
			}
			button_index = i;
			break;
		}
	}
	for (int i = button_index; i > 0; i--) {
		buttons[i] = buttons[i - 1];
	}
	buttons[0] = button;
}

bool PlayerInfo::is_valid_buffer_button(u32 button) {
	if (button == BUTTON_LP || button == BUTTON_MP || button == BUTTON_HP || button == BUTTON_LK || button == BUTTON_MK || button == BUTTON_HK) {
		return true;
	}
	else {
		return false;
	}
}

bool PlayerInfo::horizontal_input(bool right) {
	if (right) {
		if (check_button_on(BUTTON_MENU_RIGHT)) {
			if (check_button_trigger(BUTTON_MENU_RIGHT)) {
				stick_hold_h_timer = MENU_STICK_HOLD_TIMER;
				return true;
			}
			else if (stick_hold_h_timer == 0) {
				stick_hold_h_timer = MENU_STICK_HOLD_INTERVAL;
				return true;
			}
			else {
				stick_hold_h_timer--;
				return false;
			}
		}
	}
	else {
		if (check_button_on(BUTTON_MENU_LEFT)) {
			if (check_button_trigger(BUTTON_MENU_LEFT)) {
				stick_hold_h_timer = MENU_STICK_HOLD_TIMER;
				return true;
			}
			else if (stick_hold_h_timer == 0) {
				stick_hold_h_timer = MENU_STICK_HOLD_INTERVAL;
				return true;
			}
			else {
				stick_hold_h_timer--;
				return false;
			}
		}
	}
}

bool PlayerInfo::vertical_input(bool down) {
	if (down) {
		if (check_button_on(BUTTON_MENU_DOWN)) {
			if (check_button_trigger(BUTTON_MENU_DOWN)) {
				stick_hold_v_timer = MENU_STICK_HOLD_TIMER;
				return true;
			}
			else if (stick_hold_v_timer == 0) {
				stick_hold_v_timer = MENU_STICK_HOLD_INTERVAL;
				return true;
			}
			else {
				stick_hold_v_timer--;
				return false;
			}
		}
		else {
			return false;
		}
	}
	else {
		if (check_button_on(BUTTON_MENU_UP)) {
			if (check_button_trigger(BUTTON_MENU_UP)) {
				stick_hold_v_timer = MENU_STICK_HOLD_TIMER;
				return true;
			}
			else if (stick_hold_v_timer == 0) {
				stick_hold_v_timer = MENU_STICK_HOLD_INTERVAL;
				return true;
			}
			else {
				stick_hold_v_timer--;
				return false;
			}
		}
		else {
			return false;
		}
	}
}

bool PlayerInfo::is_any_inputs() {
	if (controller != NULL) {
		return is_any_controller_input(controller);
	}
	else {
		for (int i = 0; i < BUTTON_MAX; i++) {
			if (check_button_trigger(i)) {
				return true;
			}
		}
		return false;
	}
}