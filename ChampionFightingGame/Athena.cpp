#include "Athena.h"

Athena::Athena() {

}

Athena::Athena(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_ATHENA;
	chara_name = "athena";
	resource_dir = "resource/chara/athena";
	fighter_int.resize(CHARA_ATHENA_INT_MAX, 0);
	fighter_float.resize(CHARA_ATHENA_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ATHENA_FLAG_MAX, false);
	init();
}