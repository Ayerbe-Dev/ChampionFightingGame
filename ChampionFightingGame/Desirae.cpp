#include "Desirae.h"

Desirae::Desirae() {

}

Desirae::Desirae(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_DESIRAE;
	chara_name = "desirae";
	resource_dir = "resource/chara/desirae";
	fighter_int.resize(CHARA_DESIRAE_INT_MAX, 0);
	fighter_float.resize(CHARA_DESIRAE_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_DESIRAE_FLAG_MAX, false);
	load_fighter();
}