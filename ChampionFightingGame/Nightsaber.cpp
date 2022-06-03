#include "Nightsaber.h"

Nightsaber::Nightsaber() {

}

Nightsaber::Nightsaber(int id, Player* player) {
	this->player = player;
	this->chara_kind = CHARA_KIND_NIGHTSABER;
	chara_name = "nightsaber";
	resource_dir = "resource/chara/nightsaber";
	fighter_int.resize(CHARA_NIGHTSABER_INT_MAX, 0);
	fighter_float.resize(CHARA_NIGHTSABER_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_NIGHTSABER_FLAG_MAX, false);
	loadNightsaberStatusFunctions();
}

void Nightsaber::chara_id() {

}