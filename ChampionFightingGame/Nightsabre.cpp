#include "Nightsabre.h"

Nightsabre::Nightsabre() {

}

Nightsabre::Nightsabre(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_NIGHTSABRE;
	chara_name = "nightsabre";
	resource_dir = "resource/chara/nightsabre";
	object_int.resize(CHARA_NIGHTSABRE_INT_MAX, 0);
	object_float.resize(CHARA_NIGHTSABRE_FLOAT_MAX, 0.0);
	object_flag.resize(CHARA_NIGHTSABRE_FLAG_MAX, false);
	object_string.resize(CHARA_NIGHTSABRE_STRING_MAX, "");
	status_script.resize(CHARA_NIGHTSABRE_STATUS_MAX, nullptr);
	enter_status_script.resize(CHARA_NIGHTSABRE_STATUS_MAX, nullptr);
	exit_status_script.resize(CHARA_NIGHTSABRE_STATUS_MAX, nullptr);

	load_fighter();
}

void Nightsabre::chara_main() {

}