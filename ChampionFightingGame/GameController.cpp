#include "GameController.h"
#include "ParamAccessor.h"
#include "InputManager.h"
#include "utils.h"

GameController::GameController() {
	stick_hold_h_timer = 0;
	stick_hold_v_timer = 0;
	hold_buffer = false;
	controller_id = -1;
	player_controller_id = &controller_id;
	owns_keyboard = false;
	player_owns_keyboard = &owns_keyboard;

	reset_button_mappings();
}

void GameController::reset_button_mappings() {
	button_info.clear();
	add_button_mapping(BUTTON_UP, GLFW_KEY_W, GLFW_GAMEPAD_BUTTON_DPAD_UP);
	add_button_mapping(BUTTON_DOWN, GLFW_KEY_S, GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
	add_button_mapping(BUTTON_LEFT, GLFW_KEY_A, GLFW_GAMEPAD_BUTTON_DPAD_LEFT);
	add_button_mapping(BUTTON_RIGHT, GLFW_KEY_D, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT);
	add_button_mapping(BUTTON_LP, GLFW_KEY_Y, GLFW_GAMEPAD_BUTTON_A);
	add_button_mapping(BUTTON_MP, GLFW_KEY_U, GLFW_GAMEPAD_BUTTON_Y);
	add_button_mapping(BUTTON_HP, GLFW_KEY_I, GLFW_GAMEPAD_BUTTON_B);
	add_button_mapping(BUTTON_LK, GLFW_KEY_H, GLFW_GAMEPAD_BUTTON_LEFT_BUMPER);
	add_button_axis(BUTTON_MK, GLFW_KEY_J, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER);
	add_button_axis(BUTTON_HK, GLFW_KEY_K, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER);
	add_button(BUTTON_2L);
	add_button(BUTTON_2M);
	add_button(BUTTON_2H);
	add_button_mapping(BUTTON_3P, GLFW_KEY_O, GLFW_GAMEPAD_BUTTON_X);
	add_button_mapping(BUTTON_3K, GLFW_KEY_L, GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);
	add_button(BUTTON_6B);
	add_button_mapping(BUTTON_START, GLFW_KEY_ESCAPE, GLFW_GAMEPAD_BUTTON_START);
	add_button_mapping(BUTTON_MENU_UP, GLFW_KEY_W, GLFW_GAMEPAD_BUTTON_DPAD_UP);
	add_button_mapping(BUTTON_MENU_DOWN, GLFW_KEY_S, GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
	add_button_mapping(BUTTON_MENU_LEFT, GLFW_KEY_A, GLFW_GAMEPAD_BUTTON_DPAD_LEFT);
	add_button_mapping(BUTTON_MENU_RIGHT, GLFW_KEY_D, GLFW_GAMEPAD_BUTTON_DPAD_RIGHT);
	add_button_mapping(BUTTON_MENU_SELECT, GLFW_KEY_Y, GLFW_GAMEPAD_BUTTON_A);
	add_button_mapping(BUTTON_MENU_START, GLFW_KEY_ESCAPE, GLFW_GAMEPAD_BUTTON_START);
	add_button_mapping(BUTTON_MENU_BACK, GLFW_KEY_U, GLFW_GAMEPAD_BUTTON_B);
	add_button_axis(BUTTON_MENU_PAGE_LEFT, GLFW_KEY_H, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER);
	add_button_axis(BUTTON_MENU_PAGE_RIGHT, GLFW_KEY_L, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER);
	add_button_mapping(BUTTON_MENU_FRAME_PAUSE, GLFW_KEY_LEFT_SHIFT, GLFW_GAMEPAD_BUTTON_MAX);
	add_button_mapping(BUTTON_MENU_FRAME_ADVANCE, GLFW_KEY_LEFT_CONTROL, GLFW_GAMEPAD_BUTTON_MAX);
	add_button_mapping(BUTTON_MENU_RECORD_INPUT, GLFW_KEY_1, GLFW_GAMEPAD_BUTTON_MAX);
	add_button_mapping(BUTTON_MENU_REPLAY_INPUT, GLFW_KEY_2, GLFW_GAMEPAD_BUTTON_MAX);
	add_button_mapping(BUTTON_MENU_SWITCH_INPUT, GLFW_KEY_SPACE, GLFW_GAMEPAD_BUTTON_MAX);
}

void GameController::check_controllers() {
	InputManager* input_manager = InputManager::get_instance();
	if (owns_keyboard) return;
	if (controller_id == -1) {
		if (input_manager->get_owner(-1) == nullptr) {
			for (size_t i = 0, max = button_info.size(); i < max; i++) {
				if (input_manager->keyboard_state[button_info[i].k_mapping]) {
					owns_keyboard = true;
					input_manager->register_controller(-1, this);
					return;
				}
			}
		}
		for (int c : input_manager->available_controllers) {
			if (input_manager->get_owner(c) == nullptr) {
				GLFWgamepadstate state;
				if (is_any_controller_input(c)) {
					controller_id = c;
					input_manager->register_controller(c, this);
					return;
				}
			}
		}
	}
}

void GameController::set_owns_keyboard(bool owns_keyboard) {
	this->owns_keyboard = owns_keyboard;
}

void GameController::poll_menu() {
	std::map<int, bool> keyboard_state = InputManager::get_instance()->keyboard_state;
	if (controller_id != -1) {
		GLFWgamepadstate state;
		glfwGetGamepadState(controller_id, &state);
		for (unsigned int i = BUTTON_MENU; i < BUTTON_MENU_MAX; i++) {
			bool old_button = button_info[i].button_on;
			switch (i) {
				case (BUTTON_MENU_UP): {
					button_info[i].button_on = ((button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX
						&& state.buttons[button_info[i].c_mapping])
						|| state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] >= 0.4);
				} break;
				case (BUTTON_MENU_DOWN): {
					button_info[i].button_on = ((button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX
						&& state.buttons[button_info[i].c_mapping])
						|| state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] <= -0.4);
				} break;
				case (BUTTON_MENU_LEFT): {
					button_info[i].button_on = ((button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX
						&& state.buttons[button_info[i].c_mapping])
						|| state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] <= -0.4);
				} break;
				case (BUTTON_MENU_RIGHT): {
					button_info[i].button_on = ((button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX
						&& state.buttons[button_info[i].c_mapping])
						|| state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] >= 0.4);
				} break;
				default: {
					button_info[i].button_on = (button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX && state.buttons[button_info[i].c_mapping])
						|| (button_info[i].c_axis != GLFW_GAMEPAD_AXIS_MAX && state.axes[button_info[i].c_axis] >= 0.4);
				} break;
			}
			button_info[i].changed = button_info[i].button_on != old_button;
		}
	}
	else if (owns_keyboard) {
		for (unsigned int i = BUTTON_MENU; i < BUTTON_MENU_MAX; i++) {
			bool old_button = button_info[i].button_on;
			button_info[i].button_on = keyboard_state[button_info[i].k_mapping];
			button_info[i].changed = button_info[i].button_on != old_button;
		}
	}
}

void GameController::poll_fighter() {
	std::map<int, bool> keyboard_state = InputManager::get_instance()->keyboard_state;
	int buffer_window = get_global_param_int(PARAM_FIGHTER, "buffer_window");
	input_code = 0;
	std::vector<bool> old_button;
	for (unsigned int i = 0; i < BUTTON_MENU; i++) {
		old_button.push_back(button_info[i].button_on);
	}
	if (*player_controller_id != -1) {
		GLFWgamepadstate state;
		glfwGetGamepadState(*player_controller_id, &state);
		for (unsigned int i = 0; i < BUTTON_MENU; i++) {
			switch (i) {
				case (BUTTON_UP): {
					button_info[i].button_on = ((button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX
						&& state.buttons[button_info[i].c_mapping])
						|| state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] >= 0.4);
				} break;
				case (BUTTON_DOWN): {
					button_info[i].button_on = ((button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX
						&& state.buttons[button_info[i].c_mapping])
						|| state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y] <= -0.4);
				} break;
				case (BUTTON_LEFT): {
					button_info[i].button_on = ((button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX
						&& state.buttons[button_info[i].c_mapping])
						|| state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] <= -0.4);
				} break;
				case (BUTTON_RIGHT): {
					button_info[i].button_on = ((button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX
						&& state.buttons[button_info[i].c_mapping])
						|| state.axes[GLFW_GAMEPAD_AXIS_LEFT_X] >= 0.4);
				} break;
				default: {
					button_info[i].button_on = (button_info[i].c_mapping != GLFW_GAMEPAD_BUTTON_MAX && state.buttons[button_info[i].c_mapping])
						|| (button_info[i].c_axis != GLFW_GAMEPAD_AXIS_MAX && state.axes[button_info[i].c_axis] >= 0.4);
				} break;
			}
		}
	}
	else if (*player_owns_keyboard) {
		for (unsigned int i = 0; i < BUTTON_MENU; i++) {
			button_info[i].button_on = keyboard_state[button_info[i].k_mapping];
		}
	}
	for (unsigned int i = 0; i < BUTTON_MENU; i++) {
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

void GameController::add_button(unsigned int button_kind) {
	Button new_button;
	new_button.button_kind = button_kind;
	button_info.push_back(new_button);
}

void GameController::add_button_mapping(unsigned int button_kind, unsigned int k_mapping, unsigned int c_mapping) {
	bool menu = is_menu_button(button_kind);
	Button new_button;
	new_button.button_kind = button_kind;
	new_button.k_mapping = k_mapping;
	new_button.c_mapping = c_mapping;
	key_map[menu][k_mapping] = button_info.size();
	button_map[menu][c_mapping] = button_info.size();
	button_info.push_back(new_button);
}

void GameController::add_button_axis(unsigned int button_kind, unsigned int k_mapping, unsigned int c_axis) {
	bool menu = is_menu_button(button_kind);
	Button new_button;
	new_button.button_kind = button_kind;
	new_button.k_mapping = k_mapping;
	new_button.c_axis = c_axis;
	key_map[menu][k_mapping] = button_info.size();
	axis_map[menu][c_axis] = button_info.size();
	button_info.push_back(new_button);
}

void GameController::set_button_k_mapping(unsigned int button_kind, unsigned int k_mapping) {
	bool menu = is_menu_button(button_kind);
	Button& button = button_info[button_kind];
	if (key_map[menu].contains(k_mapping)) {
		//Basically, this checks if we already mapped this key to a button. If we did, the button that 
		//already used this key mapping will now use the key mapping that was already on this button
		button_info[key_map[menu][k_mapping]].k_mapping = button.k_mapping;
	}
	button.k_mapping = k_mapping;
}

void GameController::set_button_c_mapping(unsigned int button_kind, unsigned int c_mapping) {
	bool menu = is_menu_button(button_kind);
	Button& button = button_info[button_kind];
	if (button_map[menu].contains(c_mapping)) {
		button_info[key_map[menu][c_mapping]].c_mapping = button.c_mapping;
	}
	button.c_mapping = c_mapping;
}

void GameController::set_button_c_axis(unsigned int button_kind, unsigned int c_axis) {
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
	if (controller_id != -1) {
		return is_any_controller_input(controller_id);
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
	return owns_keyboard || controller_id != -1;
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
	std::swap(player_controller_id, other->player_controller_id);
	std::swap(player_owns_keyboard, other->player_owns_keyboard);
}

void GameController::reset_player_controller() {
	player_controller_id = &controller_id;
	player_owns_keyboard = &owns_keyboard;
}

void GameController::remove_controller() {
	GameController* other = InputManager::get_instance()->get_owner(*player_controller_id);
	if (this != other) {
		swap_player_controller(other);
	}
	controller_id = -1;
}