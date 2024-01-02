#include "Rook.h"

bool Rook::chara_ground_status_act() {
	return false;
}

bool Rook::chara_status_attack() {
	return false;
}

bool Rook::chara_status_attack_air() {
	return false;
}

void Rook::load_chara_status_scripts() {
	status_script.resize(CHARA_ROOK_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_ROOK_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_ROOK_STATUS_MAX, nullptr);
}