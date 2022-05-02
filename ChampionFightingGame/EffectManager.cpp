#include "EffectManager.h"
EffectManager* EffectManager::instance = nullptr;

EffectManager::EffectManager() {

}

EffectManager* EffectManager::get_instance() {
	if (instance == nullptr) {
		instance = new EffectManager;
	}
	return instance;
}