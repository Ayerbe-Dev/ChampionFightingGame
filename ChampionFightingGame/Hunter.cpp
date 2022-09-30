#include "Hunter.h"

Hunter::Hunter() {

}

Hunter::Hunter(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->chara_kind = CHARA_KIND_HUNTER;
	chara_name = "hunter";
	resource_dir = "resource/chara/hunter";
	fighter_int.resize(CHARA_HUNTER_INT_MAX, 0);
	fighter_float.resize(CHARA_HUNTER_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_HUNTER_FLAG_MAX, false);
	init();
}