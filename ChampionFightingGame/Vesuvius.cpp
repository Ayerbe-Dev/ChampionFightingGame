#include "Vesuvius.h"

Vesuvius::Vesuvius() {

}

Vesuvius::Vesuvius(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->chara_kind = CHARA_KIND_VESUVIUS;
	chara_name = "vesuvius";
	resource_dir = "resource/chara/vesuvius";
	fighter_int.resize(CHARA_VESUVIUS_INT_MAX, 0);
	fighter_float.resize(CHARA_VESUVIUS_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_VESUVIUS_FLAG_MAX, false);
	loadVesuviusStatusFunctions();
}

void Vesuvius::chara_id() {

}