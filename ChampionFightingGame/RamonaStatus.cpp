#include "Ramona.h"

bool Ramona::chara_ground_status_act() {
	return false;
}

bool Ramona::chara_status_attack() {
	return false;
}

bool Ramona::chara_status_attack_air() {
	return false;
}

void Ramona::load_chara_status_scripts() {
	status_script.resize(CHARA_RAMONA_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_RAMONA_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_RAMONA_STATUS_MAX, nullptr);
}