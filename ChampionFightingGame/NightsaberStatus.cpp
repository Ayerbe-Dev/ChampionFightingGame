#include "Nightsaber.h"

void Nightsaber::chara_main() {

}

bool Nightsaber::specific_ground_status_act() {
	return false;
}

bool Nightsaber::specific_air_status_act() {
	return false;
}

bool Nightsaber::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Nightsaber::loadNightsaberStatusFunctions() {

}