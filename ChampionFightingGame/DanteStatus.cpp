#include "Dante.h"

void Dante::chara_main() {

}

bool Dante::chara_ground_status_act() {
	return false;
}

bool Dante::chara_air_status_act() {
	return false;
}

bool Dante::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Dante::load_chara_status_scripts() {

}