#include "Leon.h"

void Leon::chara_main() {

}

bool Leon::chara_ground_status_act() {
	return false;
}

bool Leon::chara_air_status_act() {
	return false;
}

bool Leon::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Leon::load_chara_status_scripts() {

}