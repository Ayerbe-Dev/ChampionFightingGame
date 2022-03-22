#include "BattleObjectManager.h"

BattleObjectManager* BattleObjectManager::instance = nullptr;

BattleObjectManager::BattleObjectManager() {}

BattleObjectManager* BattleObjectManager::get_instance() {
	if (instance == nullptr) {
		instance = new BattleObjectManager;
	}
	return instance;
}