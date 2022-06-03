#pragma once
#include <SDL/SDL.h>
#include "ButtonConstants.h"

enum {
	CONTROL_TYPE_NORMAL,
	CONTROL_TYPE_WEENIE,
};

bool is_any_controller_input(SDL_GameController* controller);
bool is_menu_button(unsigned int button_kind);

struct Button {
	unsigned int button_kind = 0;
	unsigned int k_mapping = 0;
	SDL_GameControllerButton c_mapping = SDL_CONTROLLER_BUTTON_INVALID;
	SDL_GameControllerAxis c_axis = SDL_CONTROLLER_AXIS_INVALID;
	bool button_on = false;
	bool changed = false;
	int buffer = 0;
	int force_duration = 0;
};