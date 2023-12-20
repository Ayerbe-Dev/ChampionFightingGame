#include "GameController.h"
#include "ParamAccessor.h"
#include "ControllerManager.h"
#include "utils.h"

GameController::GameController() {
	id = -1;
	controller = nullptr;
	stick_hold_h_timer = 0;
	stick_hold_v_timer = 0;
	add_buffer_button(BUTTON_LP, BUFFER_LP);
	add_buffer_button(BUTTON_MP, BUFFER_MP);
	add_buffer_button(BUTTON_HP, BUFFER_HP);
	add_buffer_button(BUTTON_LK, BUFFER_LK);
	add_buffer_button(BUTTON_MK, BUFFER_MK);
	add_buffer_button(BUTTON_HK, BUFFER_HK);
	hold_buffer = false;
}

int GameController::check_controllers() {
	ControllerManager* controller_manager = ControllerManager::get_instance();
	if (controller == nullptr) {
		SDL_GameController* new_controller;
		for (int i = 0; i < SDL_NumJoysticks(); i++) {
			if (SDL_IsGameController(i) && controller_manager->registered_controllers[!id].id != i) {
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
	else {
		if (!SDL_GameControllerGetAttached(controller)) {
			controller_manager->registered_controllers[id].id = -1;
			controller_manager->registered_controllers[id].controller = nullptr;
			controller = nullptr;
			return GAME_CONTROLLER_UPDATE_UNREGISTERED;
		}
	}
	return GAME_CONTROLLER_UPDATE_NONE;
}

void GameController::poll_buttons() {
	const Uint8* keyboard_state = ControllerManager::get_instance()->keyboard_state;
	int buffer_window = get_param_int(PARAM_FIGHTER, "buffer_window");
	std::vector<bool> old_button;
	for (size_t i = 0, max = button_info.size(); i < max; i++) {
		int button_kind = button_info[i].button_kind;
		old_button.push_back(button_info[i].button_on);
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
		if (button_kind == BUTTON_2L && button_info[i].button_on) {
			for (int i = BUTTON_LP; i <= BUTTON_LK; i += 3) {
				button_info[button_map[i]].button_on = true;
			}
		}
		if (button_kind == BUTTON_2M && button_info[i].button_on) {
			for (int i = BUTTON_MP; i <= BUTTON_MK; i += 3) {
				button_info[button_map[i]].button_on = true;
			}
		}
		if (button_kind == BUTTON_2H && button_info[i].button_on) {
			for (int i = BUTTON_HP; i <= BUTTON_HK; i += 3) {
				button_info[button_map[i]].button_on = true;
			}
		}
		if (button_kind == BUTTON_3P && button_info[i].button_on) {
			for (int i = BUTTON_LP; i <= BUTTON_HP; i++) {
				button_info[button_map[i]].button_on = true;
			}
		}
		if (button_kind == BUTTON_3K && button_info[i].button_on) {
			for (int i = BUTTON_LK; i <= BUTTON_HK; i++) {
				button_info[button_map[i]].button_on = true;
			}
		}
	}
	bool any_new_buttons = false;
	short buffer_code_buttons = 0;
	short new_buffer_code_buttons = 0;
	for (size_t i = 0, max = button_info.size(); i < max; i++) {
		button_info[i].changed = button_info[i].button_on != old_button[i];
		if (is_valid_buffer_button(button_info[i].button_kind)) {
			if (!hold_buffer) {
				button_info[i].buffer = clamp(0, button_info[i].buffer - 1, button_info[i].buffer);
			}
			if (button_info[i].changed && button_info[i].button_on) {
				button_info[i].buffer = buffer_window;
				any_new_buttons = true;
			}
			if (button_info[i].buffer) {
				buffer_code_buttons |= buffer_buttons[button_info[i].button_kind];
				if (button_info[i].button_on) {
					new_buffer_code_buttons |= buffer_buttons[button_info[i].button_kind];
				}
			}
		}
	}
	if (any_new_buttons) {
		short buffer_code_stick = 0;
		if (button_info[button_map[BUTTON_LEFT]].button_on) {
			buffer_code_stick |= BUFFER_STICK_L;
		}
		if (button_info[button_map[BUTTON_RIGHT]].button_on) {
			buffer_code_stick |= BUFFER_STICK_R;
		}
		if (button_info[button_map[BUTTON_UP]].button_on) {
			buffer_code_stick |= BUFFER_STICK_U;
		}
		if (button_info[button_map[BUTTON_DOWN]].button_on) {
			buffer_code_stick |= BUFFER_STICK_D;
		}
		buffer_code = new_buffer_code_buttons + buffer_code_stick;
	}
	else {
		short buffer_code_stick = (buffer_code >> 6) << 6;
		buffer_code = buffer_code_buttons + buffer_code_stick;
	}
}

void GameController::add_button_mapping(unsigned int button_kind, unsigned int k_mapping, SDL_GameControllerButton c_mapping) {
	bool menu = is_menu_button(button_kind);
	Button new_button;
	new_button.button_kind = button_kind;
	new_button.k_mapping = k_mapping;
	new_button.c_mapping = c_mapping;
	button_map[button_kind] = button_info.size();
	key_map[menu][k_mapping] = button_info.size();
	controller_map[menu][c_mapping] = button_info.size();
	button_info.push_back(new_button);
}

void GameController::add_button_mapping(unsigned int button_kind, unsigned int k_mapping, SDL_GameControllerAxis c_axis) {
	bool menu = is_menu_button(button_kind);
	Button new_button;
	new_button.button_kind = button_kind;
	new_button.k_mapping = k_mapping;
	new_button.c_axis = c_axis;
	button_map[button_kind] = button_info.size();
	key_map[menu][k_mapping] = button_info.size();
	axis_map[menu][c_axis] = button_info.size();
	button_info.push_back(new_button);
}

void GameController::add_button_mapping(unsigned int button_kind) {
	Button new_button;
	new_button.button_kind = button_kind;
	button_map[button_kind] = button_info.size();
	button_info.push_back(new_button);
}

void GameController::set_button_mapping(unsigned int button_kind, unsigned int k_mapping) {
	bool menu = is_menu_button(button_kind);
	Button& button = button_info[button_map[button_kind]];
	if (key_map[menu].find(k_mapping) != key_map[menu].end()) {
		//Basically, this checks if we already mapped this key to a button. If we did, the button that 
		//already used this key mapping will now use the key mapping that was already on this button
		button_info[key_map[menu][k_mapping]].k_mapping = button.k_mapping;
	}
	button.k_mapping = k_mapping;
}

void GameController::set_button_mapping(unsigned int button_kind, SDL_GameControllerButton c_mapping) {
	bool menu = is_menu_button(button_kind);
	Button& button = button_info[button_map[button_kind]];
	if (controller_map[menu].find(c_mapping) != controller_map[menu].end()) {
		button_info[key_map[menu][c_mapping]].c_mapping = button.c_mapping;
	}
	button.c_mapping = c_mapping;
}

void GameController::set_button_mapping(unsigned int button_kind, SDL_GameControllerAxis c_axis) {
	bool menu = is_menu_button(button_kind);
	Button& button = button_info[button_map[button_kind]];
	if (axis_map[menu].find(c_axis) != axis_map[menu].end()) {
		button_info[key_map[menu][c_axis]].c_axis = button.c_axis;
	}
	button.c_axis = c_axis;
}

bool GameController::check_button_on(unsigned int button_kind) {
	button_kind = button_map[button_kind];
	return button_info[button_kind].button_on;
}

bool GameController::check_button_input(unsigned int button_kind) {
	return buffer_code & buffer_buttons[button_kind];
}

bool GameController::check_button_input(unsigned int button[], int length, int min_matches) {
	if (min_matches == 0) {
		min_matches = length;
	}
	int matches = 0;
	for (int i = 0; i < length; i++) {
		if (buffer_code & buffer_buttons[button[i]]) {
			matches += 1;
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
				stick_hold_h_timer = get_param_int(PARAM_MENU, "stick_hold_timer");
				return true;
			}
			else if (stick_hold_h_timer == 0) {
				stick_hold_h_timer = get_param_int(PARAM_MENU, "stick_hold_interval");
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
				stick_hold_h_timer = get_param_int(PARAM_MENU, "stick_hold_timer");
				return true;
			}
			else if (stick_hold_h_timer == 0) {
				stick_hold_h_timer = get_param_int(PARAM_MENU, "stick_hold_interval");
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
				stick_hold_v_timer = get_param_int(PARAM_MENU, "stick_hold_timer");
				return true;
			}
			else if (stick_hold_v_timer == 0) {
				stick_hold_v_timer = get_param_int(PARAM_MENU, "stick_hold_interval");
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
				stick_hold_v_timer = get_param_int(PARAM_MENU, "stick_hold_timer");
				return true;
			}
			else if (stick_hold_v_timer == 0) {
				stick_hold_v_timer = get_param_int(PARAM_MENU, "stick_hold_interval");
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

void GameController::add_buffer_button(unsigned int button_kind, unsigned int buffer_kind) {
	buffer_buttons[button_kind] = buffer_kind;
}

bool GameController::is_valid_buffer_button(unsigned int button_kind) {
	return buffer_buttons.contains(button_kind);
}

void GameController::reset_all_buttons() {
	for (int i = 0, max = button_info.size(); i < max; i++) {
		button_info[i].button_on = false;
		button_info[i].buffer = 0;
		button_info[i].changed = false;
	}
	buffer_code = 0;
}

void GameController::reset_buffer() {
	for (int i = 0, max = button_info.size(); i < max; i++) {
		button_info[i].buffer = 0;
	}
	buffer_code = 0;
}

short GameController::get_buffer_code() {
	return buffer_code;
}

void GameController::set_id(int id) {
	this->id = id;
}

void GameController::set_stick_hold_timer(int h, int v) {
	this->stick_hold_h_timer = h;
	this->stick_hold_v_timer = v;
}

void GameController::set_hold_buffer(bool hold_buffer) {
	this->hold_buffer = hold_buffer;
}