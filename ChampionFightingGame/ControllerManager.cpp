#include "ControllerManager.h"

ControllerManager::ControllerManager() {
	for (int i = 0; i < 2; i++) {
		registered_controllers[i].controller = nullptr;
		registered_controllers[i].id = -1;
	}
	keyboard_state = SDL_GetKeyboardState(nullptr);
}

ControllerManager* ControllerManager::instance = nullptr;
ControllerManager* ControllerManager::get_instance() {
	if (instance == nullptr) {
		instance = new ControllerManager;
	}
	return instance;
}

void ControllerManager::destroy_instance() {
	for (int i = 0; i < 2; i++) {
		if (registered_controllers[i].controller != nullptr) {
			SDL_GameControllerClose(registered_controllers[i].controller);
			registered_controllers[i].controller = nullptr;
		}
		registered_controllers[i].id = -1;
	}
	if (instance != nullptr) {
		delete instance;
	}
}