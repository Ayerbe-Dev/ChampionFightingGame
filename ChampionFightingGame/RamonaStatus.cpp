#include "Ramona.h"

void Ramona::chara_main() {

}

bool Ramona::chara_ground_status_act() {
	return false;
}

bool Ramona::chara_air_status_act() {
	return false;
}

bool Ramona::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Ramona::load_chara_status_scripts() {
	status_script.resize(CHARA_RAMONA_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_RAMONA_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_RAMONA_STATUS_MAX, nullptr);
}