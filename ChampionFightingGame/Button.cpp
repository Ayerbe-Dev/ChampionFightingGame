#include "Button.h"

bool is_any_controller_input(SDL_GameController* controller) {
	for (int i = 1; i < 16; i++) {
		if (SDL_GameControllerGetButton(controller, (SDL_GameControllerButton)i)) {
			return true;
		}
	}
	for (int i = 1; i < 7; i++) {
		if (abs(SDL_GameControllerGetAxis(controller, (SDL_GameControllerAxis)i)) >= 13106) {
			return true;
		}
	}
	return false;
}
