#include "Nightsabre.h"

Nightsabre::Nightsabre() {

}

Nightsabre::Nightsabre(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_NIGHTSABRE;
	chara_name = "nightsabre";
	resource_dir = "resource/chara/nightsabre";
	fighter_int.resize(CHARA_NIGHTSABRE_INT_MAX, 0);
	fighter_float.resize(CHARA_NIGHTSABRE_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_NIGHTSABRE_FLAG_MAX, false);
	load_fighter();
}