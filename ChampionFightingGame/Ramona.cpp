#include "Ramona.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Ramona::Ramona() {

}

Ramona::Ramona(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/ramona";
	chara_name = "ramona";
	fighter_int.resize(CHARA_RAMONA_INT_MAX, 0);
	fighter_float.resize(CHARA_RAMONA_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_RAMONA_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadRamonaStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_RAMONA;
}

void Ramona::chara_id() {

}