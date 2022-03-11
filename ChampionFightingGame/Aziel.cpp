#include "Aziel.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Aziel::Aziel() {

}

Aziel::Aziel(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/aziel";
	chara_name = "aziel";
	fighter_int.resize(CHARA_AZIEL_INT_MAX, 0);
	fighter_float.resize(CHARA_AZIEL_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_AZIEL_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadAzielStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_AZIEL;
}

void Aziel::chara_id() {

}