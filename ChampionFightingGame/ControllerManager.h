#pragma once
#include <SDL/SDL.h>
#include <map>
#include "GameController.h"

struct ControllerInfo {
	int id;
	SDL_GameController* controller;
};

class ControllerManager {
public:
	ControllerManager(ControllerManager& other) = delete;
	void operator=(const ControllerManager& other) = delete;

	void register_controller(SDL_GameController* sdl_controller, GameController* controller);
	void unregister_controller(SDL_GameController* sdl_controller);
	GameController* get_owner(SDL_GameController* sdl_controller);

	const Uint8* keyboard_state;

	static ControllerManager* get_instance();
	void destroy_instance();
private:
	ControllerManager();

	std::map<SDL_GameController*, GameController*> controller_map;

	static ControllerManager* instance;
};