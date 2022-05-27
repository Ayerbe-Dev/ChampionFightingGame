#pragma once
#include <SDL/SDL.h>

struct ControllerInfo {
	int id;
	SDL_GameController* controller;
};

class ControllerManager {
public:
	ControllerManager(ControllerManager& other) = delete;
	void operator=(const ControllerManager& other) = delete;

	ControllerInfo registered_controllers[2];

	static ControllerManager* get_instance();
private:
	ControllerManager();

	static ControllerManager* instance;
};