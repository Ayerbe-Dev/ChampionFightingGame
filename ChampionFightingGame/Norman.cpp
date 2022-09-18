#include "Norman.h"

Norman::Norman() {

}

Norman::Norman(int id, Player* player) {
	this->id = id;
	this->player = player;
	chara_kind = CHARA_KIND_NORMAN;
	chara_name = "norman";
	resource_dir = "resource/chara/norman";
	fighter_int.resize(CHARA_NORMAN_INT_MAX, 0);
	fighter_float.resize(CHARA_NORMAN_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_NORMAN_FLAG_MAX, false);
	init();
}