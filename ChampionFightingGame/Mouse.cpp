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

glm::vec2 mouse_pos_to_rect_coord(glm::vec2 mouse_pos) {
	WindowManager* window_manager = WindowManager::get_instance();
	Camera& camera = window_manager->camera;

	mouse_pos.y = window_manager->res_height - mouse_pos.y;
	glm::vec3 screen_pos = glm::vec3(mouse_pos.x, mouse_pos.y, 0.0f);
	glReadPixels(mouse_pos.x, mouse_pos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &screen_pos.z);

	glm::vec4 viewport = glm::vec4(0.0f, 0.0f, window_manager->res_width, window_manager->res_height);
	glm::mat4 modelview = camera.view_matrix;
	glm::mat4 projection = camera.projection_matrix;

	glm::vec3 world_pos = glm::unProject(screen_pos, modelview, projection, viewport);

	float l = -world_pos.z / (world_pos.z - camera.pos.z);

	world_pos.x = (world_pos.x + l * (world_pos.x - camera.pos.x)) * (WINDOW_WIDTH / 100);
	world_pos.y = (world_pos.y + l * (world_pos.y - camera.pos.y)) * (WINDOW_HEIGHT / 100);

	return world_pos;
}
