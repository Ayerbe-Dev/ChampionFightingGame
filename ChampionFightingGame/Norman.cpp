#include "Norman.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Norman::Norman() {

}

Norman::Norman(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/norman";
	chara_name = "norman";
	fighter_int.resize(CHARA_NORMAN_INT_MAX, 0);
	fighter_float.resize(CHARA_NORMAN_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_NORMAN_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadNormanStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_NORMAN;
}

void Norman::chara_id() {

}