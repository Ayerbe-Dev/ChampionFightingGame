#include "Desirae.h"

bool Desirae::chara_ground_status_act() {
	return false;
}

bool Desirae::chara_status_attack() {
	return false;
}

bool Desirae::chara_status_attack_air() {
	return false;
}

void Desirae::load_chara_status_scripts() {
	status_script.resize(CHARA_DESIRAE_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_DESIRAE_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_DESIRAE_STATUS_MAX, nullptr);
}