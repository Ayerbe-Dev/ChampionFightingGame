#include "Atlas.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Atlas::Atlas() {

}

Atlas::Atlas(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
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
	this->chara_kind = CHARA_KIND_ATLAS;
}

void Atlas::chara_id() {

}