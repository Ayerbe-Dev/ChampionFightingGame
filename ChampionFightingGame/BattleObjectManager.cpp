#include "BattleObjectManager.h"

BattleObjectManager* BattleObjectManager::instance = nullptr;

BattleObjectManager::BattleObjectManager() {
	world_rate = 1.0;
}

BattleObjectManager* BattleObjectManager::get_instance() {
	if (instance == nullptr) {
		instance = new BattleObjectManager;
	}
	return instance;
}