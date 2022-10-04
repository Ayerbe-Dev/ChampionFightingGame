#include "Tessa.h"

Tessa::Tessa() {

}

Tessa::Tessa(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_TESSA;
	chara_name = "tessa";
	resource_dir = "resource/chara/tessa";
	fighter_int.resize(CHARA_TESSA_INT_MAX, 0);
	fighter_float.resize(CHARA_TESSA_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_TESSA_FLAG_MAX, false);
	init();
}