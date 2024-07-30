#pragma once
#include <SDL/SDL.h>
#include <map>
#include "GameController.h"

struct ControllerInfo {
	int id;
	SDL_GameController* controller;
};

class InputManager {
public:
	InputManager(InputManager& other) = delete;
	void operator=(const InputManager& other) = delete;

	void register_controller(SDL_GameController* sdl_controller, GameController* controller);
	void unregister_controller(SDL_GameController* sdl_controller);
	GameController* get_owner(SDL_GameController* sdl_controller);

	bool check_backspace();

	const Uint8* keyboard_state;
	char input_char;
	unsigned char backspace_time;

	static InputManager* get_instance();
	void destroy_instance();
private:
	InputManager();

	std::map<SDL_GameController*, GameController*> controller_map;

	static InputManager* instance;
};