#include "Atlas.h"

Atlas::Atlas() {

}

Atlas::Atlas(Player* player) {
	this->id = player->id;
	this->player = player;
	chara_kind = CHARA_KIND_ATLAS;
	chara_name = "atlas";
	resource_dir = "resource/chara/atlas";
	fighter_int.resize(CHARA_ATLAS_INT_MAX, 0);
	fighter_float.resize(CHARA_ATLAS_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ATLAS_FLAG_MAX, false);
	load_fighter();
}