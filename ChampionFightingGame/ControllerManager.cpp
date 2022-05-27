#include "ControllerManager.h"

ControllerManager* ControllerManager::instance = nullptr;

ControllerManager::ControllerManager() {
	for (int i = 0; i < 2; i++) {
		registered_controllers[i].controller = nullptr;
		registered_controllers[i].id = -1;
	}
}

ControllerManager* ControllerManager::get_instance() {
	if (instance == nullptr) {
		instance = new ControllerManager;
	}
	return instance;
}