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
const unsigned int BUTTON_2L = 10;
const unsigned int BUTTON_2M = 11;
const unsigned int BUTTON_2H = 12;
const unsigned int BUTTON_3P = 13;
const unsigned int BUTTON_3K = 14;
const unsigned int BUTTON_6B = 15;
const unsigned int BUTTON_START = 16;
const unsigned int BUTTON_MENU = 17;
const unsigned int BUTTON_MENU_UP = 17;
const unsigned int BUTTON_MENU_DOWN = 18;
const unsigned int BUTTON_MENU_LEFT = 19;
const unsigned int BUTTON_MENU_RIGHT = 20;
const unsigned int BUTTON_MENU_SELECT = 21;
const unsigned int BUTTON_MENU_START = 22;
const unsigned int BUTTON_MENU_BACK = 23;
const unsigned int BUTTON_MENU_FRAME_PAUSE = 24;
const unsigned int BUTTON_MENU_FRAME_ADVANCE = 25;
const unsigned int BUTTON_MENU_RECORD_INPUT = 26;
const unsigned int BUTTON_MENU_REPLAY_INPUT = 27;
const unsigned int BUTTON_MENU_SWITCH_INPUT = 28;
const unsigned int BUTTON_MENU_MAX = 29;

const int BUTTON_UP_BIT = 1 << BUTTON_UP;
const int BUTTON_DOWN_BIT = 1 << BUTTON_DOWN;
const int BUTTON_LEFT_BIT = 1 << BUTTON_LEFT;
const int BUTTON_RIGHT_BIT = 1 << BUTTON_RIGHT;
const int BUTTON_LP_BIT = 1 << BUTTON_LP;
const int BUTTON_MP_BIT = 1 << BUTTON_MP;
const int BUTTON_HP_BIT = 1 << BUTTON_HP;
const int BUTTON_LK_BIT = 1 << BUTTON_LK;
const int BUTTON_MK_BIT = 1 << BUTTON_MK;
const int BUTTON_HK_BIT = 1 << BUTTON_HK;

const int BUTTON_STICK_BIT = BUTTON_UP_BIT | BUTTON_DOWN_BIT | BUTTON_LEFT_BIT | BUTTON_RIGHT_BIT;
const int BUTTON_2L_BIT = BUTTON_LP_BIT | BUTTON_LK_BIT;
const int BUTTON_2M_BIT = BUTTON_MP_BIT | BUTTON_MK_BIT;
const int BUTTON_2H_BIT = BUTTON_HP_BIT | BUTTON_HK_BIT;
const int BUTTON_3P_BIT = BUTTON_LP_BIT | BUTTON_MP_BIT | BUTTON_HP_BIT;
const int BUTTON_3K_BIT = BUTTON_LK_BIT | BUTTON_MK_BIT | BUTTON_HK_BIT;
const int BUTTON_6B_BIT = BUTTON_3P_BIT | BUTTON_3K_BIT;
const int BUTTON_ALL_BIT = BUTTON_STICK_BIT | BUTTON_6B_BIT;

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
};