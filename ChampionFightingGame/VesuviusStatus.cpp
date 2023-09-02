#include "Vesuvius.h"

void Vesuvius::chara_main() {

}

bool Vesuvius::chara_ground_status_act() {
	return false;
}

bool Vesuvius::chara_air_status_act() {
	return false;
}

bool Vesuvius::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Vesuvius::load_chara_status_scripts() {

}