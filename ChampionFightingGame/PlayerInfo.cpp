#include "PlayerInfo.h"

PlayerInfo::PlayerInfo() {}

PlayerInfo::PlayerInfo(int id) {
	this->id = id;
	SDL_GameController *new_controller;

	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			new_controller = SDL_GameControllerOpen(i);
			cout << "Controller " << i << " is open" << endl;
			if (SDL_GameControllerGetAttached(new_controller)) {
				cout << "Controller " << i << " is attached" << endl;
				this->controller = new_controller;
				SDL_GameControllerClose(new_controller);
			}
		}
	}
	set_default_buttons(id);
}

bool PlayerInfo::check_button_on(u32 button) {
	return button_info[button].button_on;
}

bool PlayerInfo::check_button_input(u32 button) {
	return button_info[button].buffer > 0 && last_buffered_button == button;
}

bool PlayerInfo::check_button_trigger(u32 button) {
	return  button_info[button].button_on && button_info[button].changed;
}

bool PlayerInfo::check_button_release(u32 button)
{
	return !button_info[button].button_on && button_info[button].changed;
}

void PlayerInfo::update_buttons(const Uint8* keyboard_state) {
	for (int i = 0; i < BUTTON_MAX; i++) {
		bool old_button = button_info[i].button_on;
		if (controller != NULL) {
			if (i < 8) {
				if (i == 0 || i == 4) { //Up
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) <= -13106);
				}
				else if (i == 1 || i == 5) { //Down
					button_info[i].button_on = (SDL_GameControllerGetButton(controller, button_info[i].c_mapping) || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) >= 13106);
				}
				else if (i == 2 || i == 6) { //Left
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
		button_info[i].buffer = clamp(0, button_info[i].buffer - 1, button_info[i].buffer);
		if (button_info[i].changed && button_info[i].button_on && is_valid_buffer_button(i)) {
			last_buffered_button = i;
			button_info[i].buffer = BUFFER_WINDOW;
		}
	}
}

bool PlayerInfo::is_valid_buffer_button(u32 button) {
	if (button == BUTTON_LP || button == BUTTON_MP || button == BUTTON_HP || button == BUTTON_LK || button == BUTTON_MK || button == BUTTON_HK) {
		return true;
	}
	else {
		return false;
	}
}

void PlayerInfo::set_default_buttons(int id) {
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
	button_info[BUTTON_DOWN].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
	button_info[BUTTON_RIGHT].c_mapping = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
	button_info[BUTTON_LP].c_mapping = SDL_CONTROLLER_BUTTON_B;
	button_info[BUTTON_MP].c_mapping = SDL_CONTROLLER_BUTTON_Y;
	button_info[BUTTON_HP].c_mapping = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
	button_info[BUTTON_LK].c_mapping = SDL_CONTROLLER_BUTTON_A;
	button_info[BUTTON_MK].c_mapping = SDL_CONTROLLER_BUTTON_X;
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