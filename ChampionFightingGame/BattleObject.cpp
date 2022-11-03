#include "BattleObject.h"
#include "BattleObjectManager.h"

BattleObject::BattleObject() {
	this->id = -1;
	battle_object_manager = BattleObjectManager::get_instance();
}