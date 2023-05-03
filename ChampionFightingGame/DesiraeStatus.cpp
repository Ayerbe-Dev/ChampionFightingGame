#include "Desirae.h"

void Desirae::chara_main() {

}

bool Desirae::specific_ground_status_act() {
	return false;
}

bool Desirae::specific_air_status_act() {
	return false;
}

bool Desirae::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Desirae::load_chara_status_scripts() {

}