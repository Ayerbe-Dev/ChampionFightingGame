#include "Priest.h"

void Priest::chara_main() {

}

bool Priest::specific_ground_status_act() {
	return false;
}

bool Priest::specific_air_status_act() {
	return false;
}

bool Priest::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Priest::loadPriestStatusFunctions() {

}