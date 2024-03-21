#include "Button.h"

bool is_any_controller_input(SDL_GameController* controller) {
	for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
		if (SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)i)) {
			return true;
		}
	}
	for (int i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++) {
		if (abs(SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)i)) >= 13106) {
			return true;
		}
	}
	return false;
}

bool is_menu_button(unsigned int button_kind) {
	return button_kind >= BUTTON_MENU;
}