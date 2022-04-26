#include "AIManager.h"

AIManager* AIManager::instance = nullptr;

AIManager::AIManager() {}

AIManager* AIManager::get_instance() {
	if (instance == nullptr) {
		instance = new AIManager;
	}
	return instance;
}