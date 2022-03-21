#include "Julius.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Julius::Julius() {

}

Julius::Julius(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	this->fighter_accessor = fighter_accessor;
	this->chara_kind = CHARA_KIND_JULIUS;
	chara_name = "julius";
	resource_dir = "resource/chara/julius";
	fighter_int.resize(CHARA_JULIUS_INT_MAX, 0);
	fighter_float.resize(CHARA_JULIUS_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_JULIUS_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadJuliusStatusFunctions();
	set_current_move_script("default");
}

void Julius::chara_id() {

}