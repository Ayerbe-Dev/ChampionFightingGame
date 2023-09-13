#include "Desirae.h"

void Desirae::chara_main() {

}

bool Desirae::chara_ground_status_act() {
	return false;
}

bool Desirae::chara_air_status_act() {
	return false;
}

bool Desirae::chara_status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT]) {
		if (chara_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void Desirae::load_chara_status_scripts() {

}