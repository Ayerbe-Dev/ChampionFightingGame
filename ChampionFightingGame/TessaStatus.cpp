#include "Tessa.h"

void Tessa::chara_main() {

}

bool Tessa::specific_ground_status_act() {
	return false;
}

bool Tessa::specific_air_status_act() {
	return false;
}

bool Tessa::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Tessa::loadTessaStatusFunctions() {

}