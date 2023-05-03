#include "Nightsabre.h"

void Nightsabre::chara_main() {

}

bool Nightsabre::specific_ground_status_act() {
	return false;
}

bool Nightsabre::specific_air_status_act() {
	return false;
}

bool Nightsabre::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Nightsabre::load_chara_status_scripts() {

}