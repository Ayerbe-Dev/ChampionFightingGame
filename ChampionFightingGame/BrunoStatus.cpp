#include "Bruno.h"

void Bruno::chara_main() {

}

bool Bruno::specific_ground_status_act() {
	return false;
}

bool Bruno::specific_air_status_act() {
	return false;
}

bool Bruno::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Bruno::loadBrunoStatusFunctions() {

}