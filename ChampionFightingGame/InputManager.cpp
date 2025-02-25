#include "InputManager.h"
#include "WindowManager.h"
#include <iostream>
#include <glew/glew.h>
#include <glfw/glfw3.h>

InputManager::InputManager() {
	controller_map[-1] = nullptr;
	text_input_text = nullptr;
}

void InputManager::check_controllers() {
	for (int i = 0; i <= GLFW_JOYSTICK_LAST; i++) {
		bool present = glfwJoystickIsGamepad(i);
		if (present != available_controllers.contains(i)) {
			if (present) {
				controller_map[i] = nullptr;
				available_controllers.insert(i);
			}
			else {
				unregister_controller(i);
				available_controllers.erase(i);
			}
		}
	}
}

void InputManager::register_controller(int controller_id, GameController* controller) {
	controller_map[controller_id] = controller;
}

void InputManager::unregister_controller(int controller_id) {
	if (controller_map[controller_id]) {
		controller_map[controller_id]->remove_controller();
	}
	controller_map.erase(controller_id);
}

GameController* InputManager::get_owner(int controller_id) {
	return controller_map[controller_id];
}

bool InputManager::is_using_text_input() const {
	return text_input_text;
}

void InputManager::modify_text_input(unsigned int key, unsigned int mods) {
	std::string s = text_input_text->get_text();

	//TODO: Implement Text Entry Here

	text_input_text->update_text(s);
}

InputManager* InputManager::instance = nullptr;
InputManager* InputManager::get_instance() {
	if (instance == nullptr) {
		instance = new InputManager;
	}
	return instance;
}

void InputManager::destroy_instance() {
	text_input_text = nullptr;
	if (instance != nullptr) {
		delete instance;
	}
}