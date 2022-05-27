#include "GameController.h"
#include "ParamAccessor.h"
#include "ControllerManager.h"
#include "utils.h"

GameController::GameController() {
	id = -1;
	controller = nullptr;
	stick_hold_h_timer = 0;
	stick_hold_v_timer = 0;
}

int GameController::check_controllers() {
	ControllerManager* controller_manager = ControllerManager::get_instance();
	if (controller == nullptr) {
		SDL_GameController* new_controller;
		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			if (SDL_IsGameController(i)) {
				if (controller_manager->registered_controllers[!id].id != i) {
					new_controller = SDL_GameControllerOpen(i);
					if (is_any_controller_input(new_controller)) {
						controller = new_controller;
						controller_manager->registered_controllers[id].id = i;
						controller_manager->registered_controllers[id].controller = controller;
						return GAME_CONTROLLER_UPDATE_REGISTERED;
					}
					SDL_GameControllerClose(new_controller);
				}
			}
		}
	}
	else {
		if (!SDL_GameControllerGetAttached(controller)) {
			controller_manager->registered_controllers[id].id = -1;
			controller_manager->registered_controllers[id].controller = nullptr;;
			controller = nullptr;
			return GAME_CONTROLLER_UPDATE_UNREGISTERED;
		}
	}
	return GAME_CONTROLLER_UPDATE_NONE;
}

void GameController::poll_buttons(const Uint8* keyboard_state) {
	int buffer_window = get_param_int("buffer_window", PARAM_FIGHTER);
	for (int i = 0, max = button_info.size(); i < max; i++) {
		int button_kind = button_info[i].button_kind;
		bool old_button = button_info[i].button_on;
		if (button_info[i].force_duration > 0) {
			button_info[i].button_on = true;
			button_info[i].force_duration--;
		}
		else if (controller != nullptr) {
			if (button_kind < 8) {
				if (button_kind == BUTTON_UP || button_kind == BUTTON_MENU_UP) {
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) <= -13106);
				}
				else if (button_kind == BUTTON_DOWN || button_kind == BUTTON_MENU_DOWN) {
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) >= 13106);
				}
				else if (button_kind == BUTTON_LEFT || button_kind == BUTTON_MENU_LEFT) {
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) <= -13106);
				}
				else {
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) >= 13106);
				}
			}
			else {
				button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping)
					|| SDL_GameControllerGetAxis(controller, button_info[i].c_axis) >= 13106);
			}
		}
		else {
			button_info[i].button_on = keyboard_state[button_info[i].k_mapping];
		}
		bool new_button = button_info[i].button_on;
		button_info[i].changed = (old_button != new_button);
		if (button_kind == BUTTON_MACRO_P && button_info[i].changed) {
			if (new_button) {
				for (int i = BUTTON_LP; i <= BUTTON_HP; i++) {
					bool button_already_changed = button_info[button_map[i]].changed;
					button_info[button_map[i]].changed = !button_info[button_map[i]].button_on;
					button_info[button_map[i]].button_on = true;
					if (button_info[button_map[i]].changed || button_already_changed) {
						sort_buffer(i);
						button_info[button_map[i]].buffer = buffer_window;
					}
				}
			}
		}
		if (button_kind == BUTTON_MACRO_K && button_info[i].changed) {
			if (new_button) {
				for (int i = BUTTON_LK; i <= BUTTON_HK; i++) {
					bool button_already_changed = button_info[button_map[i]].changed;
					button_info[button_map[i]].changed = !button_info[button_map[i]].button_on;
					button_info[button_map[i]].button_on = true;
					if (button_info[button_map[i]].changed || button_already_changed) {
						sort_buffer(i);
						button_info[button_map[i]].buffer = buffer_window;
					}
				}
			}
		}
		button_info[i].buffer = clamp(0, button_info[i].buffer - 1, button_info[i].buffer);
		if (button_info[i].changed && button_info[i].button_on && is_valid_buffer_button(button_info[i].button_kind)) {
			sort_buffer(button_info[i].button_kind);
			button_info[i].buffer = buffer_window;
		}
	}
}

void GameController::add_button(unsigned int button_kind, unsigned int k_mapping, SDL_GameControllerButton c_mapping) {
	Button new_button;
	new_button.button_kind = button_kind;
	new_button.k_mapping = k_mapping;
	new_button.c_mapping = c_mapping;
	button_map[button_kind] = button_info.size();
	button_info.push_back(new_button);
}

void GameController::add_button(unsigned int button_kind, unsigned int k_mapping, SDL_GameControllerAxis c_axis) {
	Button new_button;
	new_button.button_kind = button_kind;
	new_button.k_mapping = k_mapping;
	new_button.c_axis = c_axis;
	button_map[button_kind] = button_info.size();
	button_info.push_back(new_button);
}

bool GameController::check_button_on(unsigned int button_kind) {
	button_kind = button_map[button_kind];
	return button_info[button_kind].button_on;
}

bool GameController::check_button_input(unsigned int button_kind) {
	return buffer_order[0] == button_kind && button_info[button_map[button_kind]].buffer > 0;
}

bool GameController::check_button_input(unsigned int button[], int length, int min_matches) {
	if (min_matches == 0) {
		min_matches = length;
	}
	int matches = 0;
	for (int i = 0; i < length; i++) {
		unsigned int button_kind = button_map[button[i]];
		for (int i2 = 0; i2 < length; i2++) {
			if (buffer_order[i2] == button[i] && button_info[button_kind].buffer > 0) {
				matches += 1;
				break;
			}
		}
	}
	return matches >= min_matches;
}

bool GameController::check_button_trigger(unsigned int button_kind) {
	button_kind = button_map[button_kind];
	return button_info[button_kind].changed && button_info[button_kind].button_on;
}

bool GameController::check_button_release(unsigned int button_kind) {
	button_kind = button_map[button_kind];
	return button_info[button_kind].changed && (!button_info[button_kind].button_on);
}

bool GameController::horizontal_input(bool right) {
	if (right) {
		if (check_button_on(BUTTON_MENU_RIGHT)) {
			if (check_button_trigger(BUTTON_MENU_RIGHT)) {
				stick_hold_h_timer = get_param_int("stick_hold_timer", PARAM_MENU);
				return true;
			}
			else if (stick_hold_h_timer == 0) {
				stick_hold_h_timer = get_param_int("stick_hold_interval", PARAM_MENU);
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
				stick_hold_h_timer = get_param_int("stick_hold_timer", PARAM_MENU);
				return true;
			}
			else if (stick_hold_h_timer == 0) {
				stick_hold_h_timer = get_param_int("stick_hold_interval", PARAM_MENU);
				return true;
			}
			else {
				stick_hold_h_timer--;
				return false;
			}
		}
	}
}

bool GameController::vertical_input(bool down) {
	if (down) {
		if (check_button_on(BUTTON_MENU_DOWN)) {
			if (check_button_trigger(BUTTON_MENU_DOWN)) {
				stick_hold_v_timer = get_param_int("stick_hold_timer", PARAM_MENU);
				return true;
			}
			else if (stick_hold_v_timer == 0) {
				stick_hold_v_timer = get_param_int("stick_hold_interval", PARAM_MENU);
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
				stick_hold_v_timer = get_param_int("stick_hold_timer", PARAM_MENU);
				return true;
			}
			else if (stick_hold_v_timer == 0) {
				stick_hold_v_timer = get_param_int("stick_hold_interval", PARAM_MENU);
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

bool GameController::is_any_inputs() {
	if (controller != nullptr) {
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

void GameController::set_button_on(unsigned int button_kind, int duration) {
	button_kind = button_map[button_kind];
	button_info[button_kind].force_duration = duration;
}

void GameController::set_button_off(unsigned int button_kind) {
	button_kind = button_map[button_kind];
	button_info[button_kind].force_duration = 0;
}

bool GameController::is_valid_buffer_button(unsigned int button_kind) {
	return button_kind == BUTTON_LP || button_kind == BUTTON_MP || button_kind == BUTTON_HP 
		|| button_kind == BUTTON_LK || button_kind == BUTTON_MK || button_kind == BUTTON_HK;
}

void GameController::reset_buffer() {
	for (int i = 0, max = button_info.size(); i < max; i++) {
		button_info[i].buffer = 0;
	}
}

void GameController::sort_buffer(unsigned int button) {
	int button_index = 0;
	for (int i = 0; i < 6; i++) {
		if (buffer_order[i] == button) {
			if (i == 0) {
				return;
			}
			else {
				button_index = i;
				break;
			}
		}
	}
	for (int i = button_index; i > 0; i--) {
		buffer_order[i] = buffer_order[i - 1];
	}
	buffer_order[0] = button;
}