#include "Atlas.h"

void Atlas::chara_main() {

}

bool Atlas::specific_ground_status_act() {
	return false;
}

bool Atlas::specific_air_status_act() {
	return false;
}

bool Atlas::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (situation_kind == FIGHTER_SITUATION_GROUND && specific_ground_status_act()) {
			return true;
		}
		else if (situation_kind == FIGHTER_SITUATION_AIR && specific_air_status_act()) {
			return true;
		}
	}
	return false;
}

void Atlas::loadAtlasStatusFunctions() {

}