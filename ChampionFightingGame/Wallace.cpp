#include "Wallace.h"

Wallace::Wallace() {

}

Wallace::Wallace(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_WALLACE;
	chara_name = "wallace";
	resource_dir = "resource/chara/wallace";
	fighter_int.resize(CHARA_WALLACE_INT_MAX, 0);
	fighter_float.resize(CHARA_WALLACE_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_WALLACE_FLAG_MAX, false);
	init();
}