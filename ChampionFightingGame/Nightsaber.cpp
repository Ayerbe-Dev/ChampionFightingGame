#include "Nightsaber.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Nightsaber::Nightsaber() {

}

Nightsaber::Nightsaber(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/nightsaber";
	chara_name = "nightsaber";
	fighter_int.resize(CHARA_NIGHTSABER_INT_MAX, 0);
	fighter_float.resize(CHARA_NIGHTSABER_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_NIGHTSABER_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadNightsaberStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_NIGHTSABER;
}

void Nightsaber::chara_id() {

}