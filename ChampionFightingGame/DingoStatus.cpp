#include "Dingo.h"

bool Dingo::chara_ground_status_act() {
	return false;
}

bool Dingo::chara_status_attack() {
	return false;
}

bool Dingo::chara_status_attack_air() {
	return false;
}

void Dingo::load_chara_status_scripts() {
	status_script.resize(CHARA_DINGO_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_DINGO_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_DINGO_STATUS_MAX, nullptr);
}