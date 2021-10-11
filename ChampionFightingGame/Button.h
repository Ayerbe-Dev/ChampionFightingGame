#pragma once
#include "utils.h"
struct Buttons {
	u32 mapping = 0;
	bool button_on = false;
	bool changed = false;
};

/*
For each of the user's controls, track what key they're assigned to, whether or not that button is being pressed, and whether or not a change was made
on the current frame
*/