#include "Bruno.h"

Bruno::Bruno() {

}

Bruno::Bruno(int id, Player* player) {
	this->player = player;
	this->chara_kind = CHARA_KIND_BRUNO;
	chara_name = "bruno";
	resource_dir = "resource/chara/bruno";
	fighter_int.resize(CHARA_BRUNO_INT_MAX, 0);
	fighter_float.resize(CHARA_BRUNO_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_BRUNO_FLAG_MAX, false);
	loadBrunoStatusFunctions();
}

void Bruno::chara_id() {

}