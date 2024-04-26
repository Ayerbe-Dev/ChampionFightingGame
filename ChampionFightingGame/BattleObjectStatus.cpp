#include "BattleObject.h"

void BattleObject::status_none() {
	
}

void BattleObject::enter_status_none() {

}

void BattleObject::exit_status_none() {

}

void BattleObject::load_battle_object_status_scripts() {
	status_script[BATTLE_OBJECT_STATUS_NONE] = &BattleObject::status_none;
	enter_status_script[BATTLE_OBJECT_STATUS_NONE] = &BattleObject::enter_status_none;
	exit_status_script[BATTLE_OBJECT_STATUS_NONE] = &BattleObject::exit_status_none;
}