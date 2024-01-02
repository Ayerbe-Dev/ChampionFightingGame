#include "Vesuvius.h"

Vesuvius::Vesuvius() {

}

Vesuvius::Vesuvius(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_VESUVIUS;
	chara_name = "vesuvius";
	resource_dir = "resource/chara/vesuvius";
	fighter_int.resize(CHARA_VESUVIUS_INT_MAX, 0);
	fighter_float.resize(CHARA_VESUVIUS_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_VESUVIUS_FLAG_MAX, false);
	fighter_string.resize(CHARA_VESUVIUS_STRING_MAX, "");
	load_fighter();
}

void Vesuvius::chara_main() {

}