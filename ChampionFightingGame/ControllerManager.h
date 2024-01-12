#pragma once
#include <SDL/SDL.h>
#include "GameController.h"

struct ControllerInfo {
	int id;
	SDL_GameController* controller;
};

class ControllerManager {
public:
	ControllerManager(ControllerManager& other) = delete;
	void operator=(const ControllerManager& other) = delete;

	ControllerInfo registered_controllers[2];
	const Uint8* keyboard_state;

	static ControllerManager* get_instance();
	void destroy_instance();
private:
	ControllerManager();

	static ControllerManager* instance;
};