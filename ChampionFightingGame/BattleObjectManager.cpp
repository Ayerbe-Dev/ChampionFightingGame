#include "BattleObjectManager.h"

BattleObjectManager* BattleObjectManager::instance = nullptr;

BattleObjectManager::BattleObjectManager() {
	world_rate = 1.0;
	world_frame = 0.0;
}

BattleObjectManager* BattleObjectManager::get_instance() {
	if (instance == nullptr) {
		instance = new BattleObjectManager;
	}
	return instance;
}

bool BattleObjectManager::counters_can_move() {
	return world_frame >= 0.97;
}