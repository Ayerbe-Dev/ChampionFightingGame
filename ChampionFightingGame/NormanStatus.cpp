#include "Norman.h"

void Norman::chara_main() {

}

bool Norman::specific_ground_status_act() {
	return false;
}

bool Norman::specific_air_status_act() {
	return false;
}

bool Norman::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Norman::loadNormanStatusFunctions() {

}