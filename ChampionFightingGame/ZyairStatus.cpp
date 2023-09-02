#include "Zyair.h"

void Zyair::chara_main() {

}

bool Zyair::chara_ground_status_act() {
	return false;
}

bool Zyair::chara_air_status_act() {
	return false;
}

bool Zyair::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Zyair::load_chara_status_scripts() {

}