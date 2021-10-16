#pragma once
#include "utils.h"
#include "Button.h"

class Debugger {
public:
	Debugger();
	Debugger(int id);

	Buttons button_info[BUTTON_MAX];

	bool check_button_on(u32 button);
	bool check_button_trigger(u32 button);
	bool check_button_release(u32 button);
};