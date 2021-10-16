#include "Debugger.h"

Debugger::Debugger() {}

Debugger::Debugger(int id) {
	if (id == 0) {
		button_info[BUTTON_UP].mapping = SDL_SCANCODE_W;
		button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_A;
		button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_S;
		button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_D;
		button_info[BUTTON_LP].mapping = SDL_SCANCODE_Y;
		button_info[BUTTON_MP].mapping = SDL_SCANCODE_U;
		button_info[BUTTON_HP].mapping = SDL_SCANCODE_I;
		button_info[BUTTON_LK].mapping = SDL_SCANCODE_H;
		button_info[BUTTON_MK].mapping = SDL_SCANCODE_J;
		button_info[BUTTON_HK].mapping = SDL_SCANCODE_K;

		button_info[BUTTON_START].mapping = SDL_SCANCODE_SPACE;
		button_info[BUTTON_DEBUG].mapping = SDL_SCANCODE_LSHIFT;
		button_info[BUTTON_DEBUG_2].mapping = SDL_SCANCODE_LCTRL;
	}
	else {
		button_info[BUTTON_UP].mapping = SDL_SCANCODE_UP;
		button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_DOWN;
		button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_LEFT;
		button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_RIGHT;

		button_info[BUTTON_LP].mapping = SDL_SCANCODE_Z;
		button_info[BUTTON_MP].mapping = SDL_SCANCODE_X;
		button_info[BUTTON_HP].mapping = SDL_SCANCODE_C;
		button_info[BUTTON_LK].mapping = SDL_SCANCODE_V;
		button_info[BUTTON_MK].mapping = SDL_SCANCODE_B;
		button_info[BUTTON_HK].mapping = SDL_SCANCODE_N;

		button_info[BUTTON_START].mapping = SDL_SCANCODE_RETURN;
		button_info[BUTTON_DEBUG].mapping = SDL_SCANCODE_SLASH;
		button_info[BUTTON_DEBUG_2].mapping = SDL_SCANCODE_RSHIFT;
	}

}

bool Debugger::check_button_on(u32 button) {
	return button_info[button].button_on;
}

bool Debugger::check_button_trigger(u32 button) {
	return button_info[button].changed && button_info[button].button_on;
}

bool Debugger::check_button_release(u32 button) {
	return button_info[button].changed && !button_info[button].button_on;
}