#pragma once
#include <SDL/SDL.h>

const unsigned int BUTTON_UP = 0;
const unsigned int BUTTON_DOWN = 1;
const unsigned int BUTTON_LEFT = 2;
const unsigned int BUTTON_RIGHT = 3;
const unsigned int BUTTON_LP = 4;
const unsigned int BUTTON_MP = 5;
const unsigned int BUTTON_HP = 6;
const unsigned int BUTTON_LK = 7;
const unsigned int BUTTON_MK = 8;
const unsigned int BUTTON_HK = 9;
const unsigned int BUTTON_3P = 10;
const unsigned int BUTTON_3K = 11;
const unsigned int BUTTON_2L = 12;
const unsigned int BUTTON_2M = 13;
const unsigned int BUTTON_2H = 14;
const unsigned int BUTTON_START = 15;
const unsigned int BUTTON_MENU_UP = 16;
const unsigned int BUTTON_MENU_DOWN = 17;
const unsigned int BUTTON_MENU_LEFT = 18;
const unsigned int BUTTON_MENU_RIGHT = 19;
const unsigned int BUTTON_MENU_SELECT = 20;
const unsigned int BUTTON_MENU_START = 21;
const unsigned int BUTTON_MENU_BACK = 22;
const unsigned int BUTTON_MENU_FRAME_PAUSE = 23;
const unsigned int BUTTON_MENU_FRAME_ADVANCE = 24;
const unsigned int BUTTON_MENU_RECORD_INPUT = 25;
const unsigned int BUTTON_MENU_REPLAY_INPUT = 26;
const unsigned int BUTTON_MENU_SWITCH_INPUT = 27;
const unsigned int BUTTON_MAX = 28;

enum {
	CONTROL_TYPE_ADVANCE,
	CONTROL_TYPE_BASIC,
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