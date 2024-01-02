#include "Zyair.h"

bool Zyair::chara_ground_status_act() {
	return false;
}

bool Zyair::chara_status_attack() {
	return false;
}

bool Zyair::chara_status_attack_air() {
	return false;
}

void Zyair::load_chara_status_scripts() {
	status_script.resize(CHARA_ZYAIR_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ZYAIR_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ZYAIR_STATUS_MAX, nullptr);
}