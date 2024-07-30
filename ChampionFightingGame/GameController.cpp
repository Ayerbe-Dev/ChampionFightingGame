#include "GameController.h"
#include "ParamAccessor.h"
#include "InputManager.h"
#include "utils.h"
#include "SDL/SDL_gamecontroller.h"

GameController::GameController() {
	stick_hold_h_timer = 0;
	stick_hold_v_timer = 0;
	hold_buffer = false;
	controller = nullptr;
	player_controller = &controller;
	owns_keyboard = false;
	player_owns_keyboard = &owns_keyboard;

	reset_button_mappings();
}

void GameController::reset_button_mappings() {
	button_info.clear();
	add_button_mapping(BUTTON_UP, SDL_SCANCODE_W, SDL_CONTROLLER_BUTTON_DPAD_UP);
	add_button_mapping(BUTTON_DOWN, SDL_SCANCODE_S, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	add_button_mapping(BUTTON_LEFT, SDL_SCANCODE_A, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	add_button_mapping(BUTTON_RIGHT, SDL_SCANCODE_D, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	add_button_mapping(BUTTON_LP, SDL_SCANCODE_Y, SDL_CONTROLLER_BUTTON_A);
	add_button_mapping(BUTTON_MP, SDL_SCANCODE_U, SDL_CONTROLLER_BUTTON_Y);
	add_button_mapping(BUTTON_HP, SDL_SCANCODE_I, SDL_CONTROLLER_BUTTON_B);
	add_button_mapping(BUTTON_LK, SDL_SCANCODE_H, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	add_button_mapping(BUTTON_MK, SDL_SCANCODE_J, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	add_button_mapping(BUTTON_HK, SDL_SCANCODE_K, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	add_button_mapping(BUTTON_2L);
	add_button_mapping(BUTTON_2M);
	add_button_mapping(BUTTON_2H);
	add_button_mapping(BUTTON_3P, SDL_SCANCODE_O, SDL_CONTROLLER_BUTTON_X);
	add_button_mapping(BUTTON_3K, SDL_SCANCODE_L, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	add_button_mapping(BUTTON_6B);
	add_button_mapping(BUTTON_START, SDL_SCANCODE_ESCAPE, SDL_CONTROLLER_BUTTON_START);
	add_button_mapping(BUTTON_MENU_UP, SDL_SCANCODE_W, SDL_CONTROLLER_BUTTON_DPAD_UP);
	add_button_mapping(BUTTON_MENU_DOWN, SDL_SCANCODE_S, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	add_button_mapping(BUTTON_MENU_LEFT, SDL_SCANCODE_A, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	add_button_mapping(BUTTON_MENU_RIGHT, SDL_SCANCODE_D, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	add_button_mapping(BUTTON_MENU_SELECT, SDL_SCANCODE_Y, SDL_CONTROLLER_BUTTON_A);
	add_button_mapping(BUTTON_MENU_START, SDL_SCANCODE_ESCAPE, SDL_CONTROLLER_BUTTON_START);
	add_button_mapping(BUTTON_MENU_BACK, SDL_SCANCODE_U, SDL_CONTROLLER_BUTTON_B);
	add_button_mapping(BUTTON_MENU_PAGE_LEFT, SDL_SCANCODE_H, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	add_button_mapping(BUTTON_MENU_PAGE_RIGHT, SDL_SCANCODE_L, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	add_button_mapping(BUTTON_MENU_FRAME_PAUSE, SDL_SCANCODE_LSHIFT, SDL_CONTROLLER_BUTTON_INVALID);
	add_button_mapping(BUTTON_MENU_FRAME_ADVANCE, SDL_SCANCODE_LCTRL, SDL_CONTROLLER_BUTTON_INVALID);
	add_button_mapping(BUTTON_MENU_RECORD_INPUT, SDL_SCANCODE_1, SDL_CONTROLLER_BUTTON_INVALID);
	add_button_mapping(BUTTON_MENU_REPLAY_INPUT, SDL_SCANCODE_2, SDL_CONTROLLER_BUTTON_INVALID);
	add_button_mapping(BUTTON_MENU_SWITCH_INPUT, SDL_SCANCODE_SPACE, SDL_CONTROLLER_BUTTON_INVALID);
}

void GameController::check_controllers() {
	InputManager* input_manager = InputManager::get_instance();
	if (owns_keyboard) return;
	if (controller == nullptr) {
		if (input_manager->get_owner(nullptr) == nullptr) {
			for (size_t i = 0, max = button_info.size(); i < max; i++) {
				if (input_manager->keyboard_state[button_info[i].k_mapping]) {
					owns_keyboard = true;
					input_manager->register_controller(nullptr, this);
					return;
				}
			}
		}
		SDL_GameController* new_controller;

		for (int i = 0, max = SDL_NumJoysticks(); i < max; i++) {
			if (SDL_IsGameController(i)) {
				new_controller = SDL_GameControllerOpen(i);
				if (input_manager->get_owner(new_controller) == nullptr) {
					if (is_any_controller_input(new_controller)) {
						controller = new_controller;
						input_manager->register_controller(new_controller, this);
						return;
					}
					SDL_GameControllerClose(new_controller);
				}

			}
		}
	}
	else {
		if (!SDL_GameControllerGetAttached(controller)) {
			input_manager->unregister_controller(controller);
			controller = nullptr;
		}
	}
}

void GameController::set_owns_keyboard(bool owns_keyboard) {
	this->owns_keyboard = owns_keyboard;
}

void GameController::poll_menu() {
	const Uint8* keyboard_state = InputManager::get_instance()->keyboard_state;
	for (unsigned int i = BUTTON_MENU; i < BUTTON_MENU_MAX; i++) {
		bool old_button = button_info[i].button_on;
		if (controller != nullptr) {
			switch (i) {
				case (BUTTON_MENU_UP): {
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) <= -13106);
				} break;
				case (BUTTON_MENU_DOWN): {
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) >= 13106);
				} break;
				case (BUTTON_MENU_LEFT): {
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) <= -13106);
				} break;
				case (BUTTON_MENU_RIGHT): {
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) >= 13106);
				} break;
				default: {
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping)
						|| SDL_GameControllerGetAxis(controller, button_info[i].c_axis) >= 13106);
				} break;
			}
		}
		else if (owns_keyboard) {
			button_info[i].button_on = keyboard_state[button_info[i].k_mapping];
		}
		button_info[i].changed = button_info[i].button_on != old_button;
	}
}

void GameController::poll_fighter() {
	const Uint8* keyboard_state = InputManager::get_instance()->keyboard_state;
	int buffer_window = get_global_param_int(PARAM_FIGHTER, "buffer_window");
	input_code = 0;
	std::vector<bool> old_button;
	for (unsigned int i = 0; i < BUTTON_MENU; i++) {
		old_button.push_back(button_info[i].button_on);
		if (*player_controller != nullptr) {
			switch (i) {
				case (BUTTON_UP): {
					button_info[i].button_on = (SDL_GameControllerGetButton(*player_controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) <= -13106);
				} break;
				case (BUTTON_DOWN): {
					button_info[i].button_on = (SDL_GameControllerGetButton(*player_controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) >= 13106);
				} break;
				case (BUTTON_LEFT): {
					button_info[i].button_on = (SDL_GameControllerGetButton(*player_controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) <= -13106);
				} break;
				case (BUTTON_RIGHT): {
					button_info[i].button_on = (SDL_GameControllerGetButton(*player_controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) >= 13106);
				} break;
				default: {
					button_info[i].button_on = (SDL_GameControllerGetButton(*player_controller, button_info[i].c_mapping)
						|| SDL_GameControllerGetAxis(*player_controller, button_info[i].c_axis) >= 13106);
				} break;
			}
		}
		else if (*player_owns_keyboard) {
			button_info[i].button_on = keyboard_state[button_info[i].k_mapping];
		}
		if (button_info[i].button_on) {
			switch (i) {
				default: {
					input_code |= (1 << i);
				} break;
				case (BUTTON_2L): {
					for (int i = BUTTON_LP; i <= BUTTON_LK; i += 3) {
						button_info[i].button_on = true;
						input_code |= (1 << i);
					}
				} break;
				case (BUTTON_2M): {
					for (int i = BUTTON_MP; i <= BUTTON_MK; i += 3) {
						button_info[i].button_on = true;
						input_code |= (1 << i);
					}
				} break;
				case (BUTTON_2H): {
					for (int i = BUTTON_HP; i <= BUTTON_HK; i += 3) {
						button_info[i].button_on = true;
						input_code |= (1 << i);
					}
				} break;
				case (BUTTON_3P): {
					for (int i = BUTTON_LP; i <= BUTTON_HP; i++) {
						button_info[i].button_on = true;
						input_code |= (1 << i);
					}
				} break;
				case (BUTTON_3K): {
					for (int i = BUTTON_LK; i <= BUTTON_HK; i++) {
						button_info[i].button_on = true;
					}
				} break;
				case (BUTTON_6B): {
					for (int i = BUTTON_LP; i <= BUTTON_HK; i++) {
						button_info[i].button_on = true;
						input_code |= (1 << i);
					}
				} break;
				case (BUTTON_START): {

				} break;
			}
		}
	}
	bool any_new_buttons = false;
	unsigned short buffer_code_buttons = 0;
	unsigned short new_buffer_code_buttons = 0;
	for (unsigned int i = 0; i < BUTTON_MENU; i++) {
		button_info[i].changed = button_info[i].button_on != old_button[i];
	}
	for (int i = BUTTON_LP; i <= BUTTON_HK; i++) {
		if (buffer_lockout_code != 0 && (buffer_lockout_code & (1 << i)) && !button_info[i].button_on) {
			button_info[i].buffer = 0;
		}
		if (!hold_buffer) {
			button_info[i].buffer = clamp(0, button_info[i].buffer - 1, button_info[i].buffer);
		}
		if (button_info[i].changed && button_info[i].button_on) {
			button_info[i].buffer = buffer_window;
			any_new_buttons = true;
		}
		if (button_info[i].buffer) {
			buffer_code_buttons |= (1 << i);
			if (button_info[i].button_on) {
				new_buffer_code_buttons |= (1 << i);
			}
		}
	}
	if (any_new_buttons) {
		unsigned short buffer_code_stick = 0;
		if (button_info[BUTTON_LEFT].button_on) {
			buffer_code_stick |= BUTTON_LEFT_BIT;
		}
		if (button_info[BUTTON_RIGHT].button_on) {
			buffer_code_stick |= BUTTON_RIGHT_BIT;
		}
		if (button_info[BUTTON_UP].button_on) {
			buffer_code_stick |= BUTTON_UP_BIT;
		}
		if (button_info[BUTTON_DOWN].button_on) {
			buffer_code_stick |= BUTTON_DOWN_BIT;
		}
		buffer_code = new_buffer_code_buttons + buffer_code_stick;
	}
	else {
		unsigned short buffer_code_stick = buffer_code & BUTTON_STICK_BIT;
		buffer_code = buffer_code_buttons + buffer_code_stick;
	}

	buffer_lockout_code &= buffer_code;
}

void GameController::poll_from_input_code(unsigned short input_code) {
	int buffer_window = get_global_param_int(PARAM_FIGHTER, "buffer_window");
	bool any_new_buttons = false;
	unsigned short buffer_code_buttons = 0;
	unsigned short new_buffer_code_buttons = 0;
	for (int i = 0; i <= BUTTON_HK; i++) { //CPUs obviously can't pause, and they don't use the macros
		bool old_button = button_info[i].button_on;
		button_info[i].button_on = input_code & (1 << i);
		button_info[i].changed = button_info[i].button_on != old_button;
	}
	for (int i = BUTTON_LP; i <= BUTTON_HK; i++) {
		if (buffer_lockout_code != 0 && (buffer_lockout_code & (1 << i)) && !button_info[i].button_on) {
			button_info[i].buffer = 0;
		}
		if (!hold_buffer) {
			button_info[i].buffer = clamp(0, button_info[i].buffer - 1, button_info[i].buffer);
		}
		if (button_info[i].changed && button_info[i].button_on) {
			button_info[i].buffer = buffer_window;
			any_new_buttons = true;
		}
		if (button_info[i].buffer) {
			buffer_code_buttons |= (1 << i);
			if (button_info[i].button_on) {
				new_buffer_code_buttons |= (1 << i);
			}
		}
	}
	if (any_new_buttons) {
		unsigned short buffer_code_stick = 0;
		if (button_info[BUTTON_LEFT].button_on) {
			buffer_code_stick |= BUTTON_LEFT_BIT;
		}
		if (button_info[BUTTON_RIGHT].button_on) {
			buffer_code_stick |= BUTTON_RIGHT_BIT;
		}
		if (button_info[BUTTON_UP].button_on) {
			buffer_code_stick |= BUTTON_UP_BIT;
		}
		if (button_info[BUTTON_DOWN].button_on) {
			buffer_code_stick |= BUTTON_DOWN_BIT;
		}
		buffer_code = new_buffer_code_buttons + buffer_code_stick;
	}
	else {
		unsigned short buffer_code_stick = buffer_code & BUTTON_STICK_BIT;
		buffer_code = buffer_code_buttons + buffer_code_stick;
	}

	buffer_lockout_code &= buffer_code;
}

void GameController::add_button_mapping(unsigned int button_kind, unsigned int k_mapping, SDL_GameControllerButton c_mapping) {
	bool menu = is_menu_button(button_kind);
	Button new_button;
	new_button.button_kind = button_kind;
	new_button.k_mapping = k_mapping;
	new_button.c_mapping = c_mapping;
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
	key_map[menu][k_mapping] = button_info.size();
	axis_map[menu][c_axis] = button_info.size();
	button_info.push_back(new_button);
}

void GameController::add_button_mapping(unsigned int button_kind) {
	Button new_button;
	new_button.button_kind = button_kind;
	button_info.push_back(new_button);
}

void GameController::set_button_mapping(unsigned int button_kind, unsigned int k_mapping) {
	bool menu = is_menu_button(button_kind);
	Button& button = button_info[button_kind];
	if (key_map[menu].contains(k_mapping)) {
		//Basically, this checks if we already mapped this key to a button. If we did, the button that 
		//already used this key mapping will now use the key mapping that was already on this button
		button_info[key_map[menu][k_mapping]].k_mapping = button.k_mapping;
	}
	button.k_mapping = k_mapping;
}

void GameController::set_button_mapping(unsigned int button_kind, SDL_GameControllerButton c_mapping) {
	bool menu = is_menu_button(button_kind);
	Button& button = button_info[button_kind];
	if (controller_map[menu].contains(c_mapping)) {
		button_info[key_map[menu][c_mapping]].c_mapping = button.c_mapping;
	}
	button.c_mapping = c_mapping;
}

void GameController::set_button_mapping(unsigned int button_kind, SDL_GameControllerAxis c_axis) {
	bool menu = is_menu_button(button_kind);
	Button& button = button_info[button_kind];
	if (axis_map[menu].find(c_axis) != axis_map[menu].end()) {
		button_info[key_map[menu][c_axis]].c_axis = button.c_axis;
	}
	button.c_axis = c_axis;
}

bool GameController::check_button_on(unsigned int button_kind) {
	return button_info[button_kind].button_on;
}

bool GameController::check_button_input(unsigned int button_kind) {
	return buffer_code & (1 << button_kind);
}

bool GameController::check_button_input(unsigned int button[], int length, int min_matches) {
	if (min_matches == 0) {
		min_matches = length;
	}
	int matches = 0;
	for (int i = 0; i < length; i++) {
		if (buffer_code & (1 << button[i])) {
			matches += 1;
		}
	}
	return matches >= min_matches;
}

bool GameController::check_button_trigger(unsigned int button_kind) {
	return button_info[button_kind].changed && button_info[button_kind].button_on;
}

bool GameController::check_button_release(unsigned int button_kind) {
	return button_info[button_kind].changed && (!button_info[button_kind].button_on);
}

bool GameController::horizontal_input(bool right) {
	if (right) {
		if (check_button_on(BUTTON_MENU_RIGHT)) {
			if (check_button_trigger(BUTTON_MENU_RIGHT)) {
				stick_hold_h_timer = get_global_param_int(PARAM_MENU, "stick_hold_timer");
				return true;
			}
			else if (stick_hold_h_timer == 0) {
				stick_hold_h_timer = get_global_param_int(PARAM_MENU, "stick_hold_interval");
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
				stick_hold_h_timer = get_global_param_int(PARAM_MENU, "stick_hold_timer");
				return true;
			}
			else if (stick_hold_h_timer == 0) {
				stick_hold_h_timer = get_global_param_int(PARAM_MENU, "stick_hold_interval");
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
				stick_hold_v_timer = get_global_param_int(PARAM_MENU, "stick_hold_timer");
				return true;
			}
			else if (stick_hold_v_timer == 0) {
				stick_hold_v_timer = get_global_param_int(PARAM_MENU, "stick_hold_interval");
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
				stick_hold_v_timer = get_global_param_int(PARAM_MENU, "stick_hold_timer");
				return true;
			}
			else if (stick_hold_v_timer == 0) {
				stick_hold_v_timer = get_global_param_int(PARAM_MENU, "stick_hold_interval");
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
		for (int i = 0; i < BUTTON_MENU_MAX; i++) {
			if (check_button_trigger(i)) {
				return true;
			}
		}
		return false;
	}
}

bool GameController::has_any_controller() {
	return owns_keyboard || controller;
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

unsigned short GameController::get_buffer_code() {
	return buffer_code;
}

unsigned short GameController::get_buffer_lockout_code() {
	return buffer_lockout_code;
}

unsigned short GameController::get_input_code() {
	return input_code;
}

void GameController::set_buffer_lockout_code(short buffer_lockout_code) {
	this->buffer_lockout_code = buffer_lockout_code;
}

void GameController::set_stick_hold_timer(int h, int v) {
	this->stick_hold_h_timer = h;
	this->stick_hold_v_timer = v;
}

void GameController::set_hold_buffer(bool hold_buffer) {
	this->hold_buffer = hold_buffer;
}

void GameController::swap_player_controller(GameController* other) {
	std::swap(player_controller, other->player_controller);
	std::swap(player_owns_keyboard, other->player_owns_keyboard);
}

void GameController::reset_player_controller() {
	player_controller = &controller;
	player_owns_keyboard = &owns_keyboard;
}