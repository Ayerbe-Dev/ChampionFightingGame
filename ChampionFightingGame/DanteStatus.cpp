#include "Dante.h"

void Dante::chara_main() {

}

bool Dante::specific_ground_status_act() {
	return false;
}

bool Dante::specific_air_status_act() {
	return false;
}

bool Dante::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Dante::loadDanteStatusFunctions() {

}