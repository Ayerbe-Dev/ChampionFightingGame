#pragma once
#include "utils.h"
#include "Button.h"
#include "PlayerInfo.h"
#include "SDL_rect.h"

class Debugger {
public:
	Debugger();

	Buttons button_info[BUTTON_DEBUG_MAX];
	int target{ 0 };
	bool print_frames{ false };
	bool enabled{ true }; //Change this value to false to completely disable debug mode

	bool check_button_on(u32 button);
	bool check_button_trigger(u32 button);
};

void debug_mode(Debugger* debugger, PlayerInfo* target, SDL_Renderer *renderer, SDL_Rect *debug_rect, GameCoordinate *debug_anchor, GameCoordinate *debug_offset);