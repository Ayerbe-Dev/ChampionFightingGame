#include "ControllerManager.h"

ControllerManager::ControllerManager() {
	controller_map[nullptr] = nullptr;
	keyboard_state = SDL_GetKeyboardState(nullptr);
}

void ControllerManager::register_controller(SDL_GameController* sdl_controller, GameController* controller) {
	controller_map[sdl_controller] = controller;
}

void ControllerManager::unregister_controller(SDL_GameController* sdl_controller) {
	controller_map.erase(sdl_controller);
}

GameController* ControllerManager::get_owner(SDL_GameController* sdl_controller) {
	return controller_map[sdl_controller];
}

ControllerManager* ControllerManager::instance = nullptr;
ControllerManager* ControllerManager::get_instance() {
	if (instance == nullptr) {
		instance = new ControllerManager;
	}
	return instance;
}

void ControllerManager::destroy_instance() {
	for (auto controller : controller_map) {
		if (controller.first != nullptr && controller.second != nullptr) {
			SDL_GameControllerClose(controller.first);
		}
	}
	if (instance != nullptr) {
		delete instance;
	}
}