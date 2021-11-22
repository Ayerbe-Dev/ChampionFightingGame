#include "Vesuvius.h"

void Vesuvius::chara_main() {

}

bool Vesuvius::specific_ground_status_act() {
	return false;
}

bool Vesuvius::specific_air_status_act() {
	return false;
}

bool Vesuvius::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Vesuvius::loadVesuviusStatusFunctions() {

}