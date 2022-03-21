#include "Leon.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Leon::Leon() {

}

Leon::Leon(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	this->fighter_accessor = fighter_accessor;
	if (player_info->chara_kind == CHARA_KIND_LEON) {
		this->chara_kind = CHARA_KIND_LEON;
		chara_name = "leon";
		resource_dir = "resource/chara/leon";
	}
	else {
		this->chara_kind = CHARA_KIND_CHAMELEON;
		chara_name = "chameleon";
		resource_dir = "resource/chara/chameleon";
	}
	fighter_int.resize(CHARA_LEON_INT_MAX, 0);
	fighter_float.resize(CHARA_LEON_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_LEON_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadLeonStatusFunctions();
	set_current_move_script("default");
}

void Leon::chara_id() {

}