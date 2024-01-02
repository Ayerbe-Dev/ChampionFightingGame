#include "Athena.h"

bool Athena::chara_ground_status_act() {
	return false;
}

bool Athena::chara_status_attack() {
	return false;
}

bool Athena::chara_status_attack_air() {
	return false;
}

void Athena::load_chara_status_scripts() {
	status_script.resize(CHARA_ATHENA_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ATHENA_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ATHENA_STATUS_MAX, nullptr);
}