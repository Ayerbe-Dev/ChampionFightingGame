#include "Sully.h"

void Sully::chara_main() {

}

bool Sully::specific_ground_status_act() {
	return false;
}

bool Sully::specific_air_status_act() {
	return false;
}

bool Sully::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Sully::load_chara_status_scripts() {

}