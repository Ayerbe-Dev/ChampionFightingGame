#include "Rook.h"

void Rook::chara_main() {

}

bool Rook::specific_ground_status_act() {
	return false;
}

bool Rook::specific_air_status_act() {
	return false;
}

bool Rook::specific_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Rook::load_chara_status_scripts() {

}