#include "Aziel.h"

void Aziel::chara_main() {

}

bool Aziel::specific_ground_status_act() {
	return false;
}

bool Aziel::specific_air_status_act() {
	return false;
}

bool Aziel::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Aziel::loadAzielStatusFunctions() {

}