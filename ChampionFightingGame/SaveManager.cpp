#include "SaveManager.h"

SaveManager* SaveManager::instance = nullptr;

SaveManager::SaveManager() {

}

SaveManager* SaveManager::get_instance() {
	if (instance == nullptr) {
		instance = new SaveManager;
	}
	return instance;
}