#include "Leon.h"

Leon::Leon() {

}

Leon::Leon(Player* player) {
	this->id = player->id;
	this->player = player;
	if (player->chara_kind == CHARA_KIND_LEON) {
		chara_kind = CHARA_KIND_LEON;
		chara_name = "leon";
		resource_dir = "resource/chara/leon";
	}
	else {
		chara_kind = CHARA_KIND_CHAMELEON;
		chara_name = "chameleon";
		resource_dir = "resource/chara/chameleon";
	}
	fighter_int.resize(CHARA_LEON_INT_MAX, 0);
	fighter_float.resize(CHARA_LEON_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_LEON_FLAG_MAX, false);
	init();
}