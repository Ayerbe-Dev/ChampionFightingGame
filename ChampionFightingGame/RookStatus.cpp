#include "Rook.h"

void Rook::chara_main() {

}

bool Rook::chara_ground_status_act() {
	return false;
}

bool Rook::chara_air_status_act() {
	return false;
}

bool Rook::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Rook::load_chara_status_scripts() {
	status_script.resize(CHARA_ROOK_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ROOK_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ROOK_STATUS_MAX, nullptr);
}