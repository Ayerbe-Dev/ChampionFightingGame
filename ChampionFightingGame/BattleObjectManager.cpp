#include "BattleObjectManager.h"

BattleObjectManager::BattleObjectManager() {
	world_rate = 1.0;
	world_frame = 0.0;
}

bool BattleObjectManager::counters_can_move() {
	return world_frame >= 0.97;
}

BattleObjectManager* BattleObjectManager::instance = nullptr;
BattleObjectManager* BattleObjectManager::get_instance() {
	if (instance == nullptr) {
		instance = new BattleObjectManager;
	}
	return instance;
}

void BattleObjectManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}