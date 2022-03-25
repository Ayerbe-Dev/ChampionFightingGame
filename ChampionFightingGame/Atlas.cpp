#include "Atlas.h"

Atlas::Atlas() {

}

Atlas::Atlas(int id, PlayerInfo* player_info) {
	this->player_info = player_info;
	this->chara_kind = CHARA_KIND_ATLAS;
	chara_name = "atlas";
	resource_dir = "resource/chara/atlas";
	fighter_int.resize(CHARA_ATLAS_INT_MAX, 0);
	fighter_float.resize(CHARA_ATLAS_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ATLAS_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadAtlasStatusFunctions();
	set_current_move_script("default");
}

void Atlas::chara_id() {

}