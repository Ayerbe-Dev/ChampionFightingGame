#pragma once
#include "utils.h"
#include "Button.h"
#include "Fighter.h"

class Debugger {
public:
	Debugger();

	Buttons button_info[BUTTON_DEBUG_MAX];
	int target{ 0 };
	bool print_frames{ false };
	bool zoom{ false };
	bool enabled{ true }; //Change this value to false to completely disable debug mode

	void poll_inputs(const Uint8* keyboard_state);

	bool check_button_on(unsigned int button);
	bool check_button_trigger(unsigned int button);

	void debug_mode(Fighter* target, GameRect* debug_rect, vec2* debug_anchor, vec2* debug_offset);
	void print_commands();
	void debug_query(string command, Fighter* target, Fighter* other);
};
