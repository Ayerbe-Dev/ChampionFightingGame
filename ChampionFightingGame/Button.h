#pragma once
#include "utils.h"

struct Buttons {
	u32 mapping = 0;
	SDL_GameControllerButton c_mapping = SDL_CONTROLLER_BUTTON_INVALID;
	SDL_GameControllerAxis axis = SDL_CONTROLLER_AXIS_INVALID;
	bool button_on = false;
	bool changed = false;
	int buffer = 0;
};