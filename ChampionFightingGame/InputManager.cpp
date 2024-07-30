#include "InputManager.h"
#include <iostream>

InputManager::InputManager() {
	controller_map[nullptr] = nullptr;
	keyboard_state = SDL_GetKeyboardState(nullptr);
	input_char = 0;
	backspace_time = 0;
}

void InputManager::register_controller(SDL_GameController* sdl_controller, GameController* controller) {
	controller_map[sdl_controller] = controller;
}

void InputManager::unregister_controller(SDL_GameController* sdl_controller) {
	controller_map.erase(sdl_controller);
}

GameController* InputManager::get_owner(SDL_GameController* sdl_controller) {
	return controller_map[sdl_controller];
}

bool InputManager::check_backspace() {
	if (keyboard_state[SDL_SCANCODE_BACKSPACE]) {
		switch (backspace_time) {
			case 0: {
				backspace_time = 30;
				return true;
			} break;
			case 1: {
				backspace_time = 2;
				return true;
			} break;
			default: {
				backspace_time--;
			} break;
		}
	}
	else {
		backspace_time = 0;
	}
	return false;
}

InputManager* InputManager::instance = nullptr;
InputManager* InputManager::get_instance() {
	if (instance == nullptr) {
		instance = new InputManager;
	}
	return instance;
}

void InputManager::destroy_instance() {
	for (auto controller : controller_map) {
		if (controller.first != nullptr && controller.second != nullptr) {
			SDL_GameControllerClose(controller.first);
		}
	}
	if (instance != nullptr) {
		delete instance;
	}
}