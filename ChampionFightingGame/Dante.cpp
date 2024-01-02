#include "Dante.h"

Dante::Dante() {

}

Dante::Dante(Player* player) {
	this->id = player->id;
	this->player = player;
	this->chara_kind = CHARA_KIND_DANTE;
	chara_name = "dante";
	resource_dir = "resource/chara/dante";
	fighter_int.resize(CHARA_DANTE_INT_MAX, 0);
	fighter_float.resize(CHARA_DANTE_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_DANTE_FLAG_MAX, false);
	fighter_string.resize(CHARA_DANTE_STRING_MAX, "");
	load_fighter();
}

void Dante::chara_main() {

}