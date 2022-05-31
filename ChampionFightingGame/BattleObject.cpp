#include "BattleObject.h"
#include "BattleObjectManager.h"
#include "EffectManager.h"
#include "SoundManager.h"

BattleObject::BattleObject() {
	this->id = -1;
	battle_object_manager = BattleObjectManager::get_instance();
	effect_manager = EffectManager::get_instance();
	sound_manager = SoundManager::get_instance();
}