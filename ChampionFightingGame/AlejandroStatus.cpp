#include "Alejandro.h"

void Alejandro::chara_main() {

}

bool Alejandro::specific_ground_status_act() {
	return false;
}

bool Alejandro::specific_air_status_act() {
	return false;
}

bool Alejandro::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Alejandro::loadAlejandroStatusFunctions() {

}