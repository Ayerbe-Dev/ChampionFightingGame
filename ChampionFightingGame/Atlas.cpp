#include "Atlas.h"

Atlas::Atlas() {

}

Atlas::Atlas(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->chara_kind = CHARA_KIND_ATLAS;
	chara_name = "atlas";
	resource_dir = "resource/chara/atlas";
	fighter_int.resize(CHARA_ATLAS_INT_MAX, 0);
	fighter_float.resize(CHARA_ATLAS_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ATLAS_FLAG_MAX, false);
	loadAtlasStatusFunctions();
}

void Atlas::chara_id() {

}