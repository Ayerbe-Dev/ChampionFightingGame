#include "Bruno.h"

Bruno::Bruno() {

}

Bruno::Bruno(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_BRUNO;
	chara_name = "bruno";
	resource_dir = "resource/chara/bruno";
	fighter_int.resize(CHARA_BRUNO_INT_MAX, 0);
	fighter_float.resize(CHARA_BRUNO_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_BRUNO_FLAG_MAX, false);
	init();
}