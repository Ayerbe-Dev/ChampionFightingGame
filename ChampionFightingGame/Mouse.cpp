#include "Mouse.h"
#include "WindowManager.h"
#include "InputManager.h"

MouseButton::MouseButton() {
	button_on = false;
	button_changed = false;
}

Mouse::Mouse() {
	pos = glm::vec2(0);
}

void Mouse::poll_buttons() {
	WindowManager* window_manager = WindowManager::get_instance();
	double rx, ry;
	glfwGetCursorPos(window_manager->window, &rx, &ry);
	pos = glm::vec2(rx, ry);

	for (int i = 0; i < GLFW_MOUSE_BUTTON_MAX; i++) {
		bool prev_on = buttons[i].button_on;
		buttons[i].button_on = glfwGetMouseButton(window_manager->window, i);
		buttons[i].button_changed = (buttons[i].button_on != prev_on);
	}
}

bool Mouse::check_button_on(unsigned int button) {
	return buttons[button].button_on;
}

bool Mouse::check_button_trigger(unsigned int button) {
	return buttons[button].button_on && buttons[button].button_changed;
}

bool Mouse::check_button_release(unsigned int button) {
	return (!buttons[button].button_on) && buttons[button].button_changed;
}

glm::vec2 Mouse::get_pos() {
	return pos;
}

glm::vec2 Mouse::get_pos_flip_y() {
	glm::vec2 ret = pos;
	ret.y = (WindowManager::get_instance()->window_height - ret.y);
	return ret;
}