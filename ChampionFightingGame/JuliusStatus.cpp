#include "Julius.h"

void Julius::chara_main() {

}

bool Julius::specific_ground_status_act() {
	return false;
}

bool Julius::specific_air_status_act() {
	return false;
}

bool Julius::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Julius::load_chara_status_scripts() {

}