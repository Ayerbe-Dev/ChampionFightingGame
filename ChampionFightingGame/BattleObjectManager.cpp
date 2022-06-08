#include "BattleObjectManager.h"

BattleObjectManager::BattleObjectManager() {
	world_rate = 1.0;
	world_frame = 0.0;
	real_time_id = -1;
}

bool BattleObjectManager::allow_dec_var(int object_id) {
	if (real_time_id == object_id || real_time_id == -1) {
		return true;
	}
	else {
		return world_frame >= 0.97;
	}
}

float BattleObjectManager::get_time_multiplier(int object_id) {
	if (real_time_id == object_id || real_time_id == -1) {
		return 1.0;
	}
	else {
		return world_rate;
	}
}

void BattleObjectManager::set_world_rate(int object_id, float world_rate) {
	if (real_time_id == object_id || real_time_id == -1) {
		real_time_id = object_id;
		this->world_rate = world_rate;
	}
}

void BattleObjectManager::reset_world_rate(int object_id) {
	if (real_time_id == object_id || real_time_id == -1) {
		world_frame = 1.0;
		world_rate = 1.0;
		real_time_id = -1;
	}
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