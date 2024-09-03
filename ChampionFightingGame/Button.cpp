#include "Button.h"
#include <iostream>

bool is_any_controller_input(int controller_id) {
	GLFWgamepadstate state;

	glfwGetGamepadState(controller_id, &state);
	for (int i = 0; i < GLFW_GAMEPAD_BUTTON_MAX; i++) {
		if (state.buttons[i]) {
			return true;
		}
	}
	for (int i = 0; i < GLFW_GAMEPAD_AXIS_LEFT_TRIGGER; i++) {
		if (abs(state.axes[i]) >= 0.4) {
			return true;
		}
	}
	for (int i = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER; i < GLFW_GAMEPAD_AXIS_MAX; i++) {
		if (state.axes[i] >= 0.4) {
			return true;
		}
	}
	return false;
}

bool is_menu_button(unsigned int button_kind) {
	return button_kind >= BUTTON_MENU;
}