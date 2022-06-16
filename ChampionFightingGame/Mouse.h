#pragma once
#include <SDL/SDL.h>
#include <glm/glm.hpp>

enum {
	MOUSE_BUTTON_M1,
	MOUSE_BUTTON_M2,
	MOUSE_BUTTON_M3,
	MOUSE_BUTTON_M4,
	MOUSE_BUTTON_M5,

	MOUSE_BUTTON_MAX
};

struct MouseButton {
	MouseButton();

	unsigned int mapping;
	bool button_on;
	bool button_changed;
};

class Mouse {
public:
	Mouse();

	void poll_buttons();

	bool check_button_on(unsigned int button);
	bool check_button_trigger(unsigned int button);
	bool check_button_release(unsigned int button);

	glm::ivec2 get_pos();
	glm::ivec2 get_pos_flip_y();

private:
	glm::ivec2 pos;
	MouseButton buttons[MOUSE_BUTTON_MAX]; //if society ever adds a 6th mouse button, we'll be ready fr
};