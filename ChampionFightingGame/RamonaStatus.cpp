#include "Ramona.h"

void Ramona::chara_main() {

}

bool Ramona::specific_ground_status_act() {
	return false;
}

bool Ramona::specific_air_status_act() {
	return false;
}

bool Ramona::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Ramona::load_chara_status_scripts() {

}