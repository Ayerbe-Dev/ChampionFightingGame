#include "Ramona.h"

Ramona::Ramona() {

}

Ramona::Ramona(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_RAMONA;
	chara_name = "ramona";
	resource_dir = "resource/chara/ramona";
	fighter_int.resize(CHARA_RAMONA_INT_MAX, 0);
	fighter_float.resize(CHARA_RAMONA_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_RAMONA_FLAG_MAX, false);
	init();
}