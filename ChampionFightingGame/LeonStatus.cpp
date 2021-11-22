#include "Leon.h"

void Leon::chara_main() {

}

bool Leon::specific_ground_status_act() {
	return false;
}

bool Leon::specific_air_status_act() {
	return false;
}

bool Leon::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Leon::loadLeonStatusFunctions() {

}