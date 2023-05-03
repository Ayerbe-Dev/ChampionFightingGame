#include "Wallace.h"

void Wallace::chara_main() {

}

bool Wallace::specific_ground_status_act() {
	return false;
}

bool Wallace::specific_air_status_act() {
	return false;
}

bool Wallace::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Wallace::load_chara_status_scripts() {

}