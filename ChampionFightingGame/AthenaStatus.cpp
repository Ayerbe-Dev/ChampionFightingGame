#include "Athena.h"

void Athena::chara_main() {

}

bool Athena::specific_ground_status_act() {
	return false;
}

bool Athena::specific_air_status_act() {
	return false;
}

bool Athena::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Athena::load_chara_status_scripts() {

}