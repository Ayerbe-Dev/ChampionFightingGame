#include "Vesuvius.h"

bool Vesuvius::chara_ground_status_act() {
	return false;
}

bool Vesuvius::chara_status_attack() {
	return false;
}

bool Vesuvius::chara_status_attack_air() {
	return false;
}

void Vesuvius::load_chara_status_scripts() {
	status_script.resize(CHARA_VESUVIUS_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_VESUVIUS_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_VESUVIUS_STATUS_MAX, nullptr);
}