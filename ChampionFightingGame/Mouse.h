#pragma once
#include <glm/glm.hpp>
#include <glew/glew.h>
#include <glfw/glfw3.h>

#define GLFW_MOUSE_BUTTON_MAX GLFW_MOUSE_BUTTON_LAST + 1

struct MouseButton {
	MouseButton();

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
	glm::vec2 get_pos();
	glm::vec2 get_pos_flip_y();

private:
	glm::vec2 pos;
	MouseButton buttons[GLFW_MOUSE_BUTTON_MAX]; //if society ever adds a 6th mouse button, we'll be ready fr
};