#include "Alejandro.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Alejandro::Alejandro() {

}

Alejandro::Alejandro(int id, PlayerInfo* player_info, BattleObjectManager* battle_object_manager) {
	this->player_info = player_info;
	this->battle_object_manager = battle_object_manager;
	this->chara_kind = CHARA_KIND_ALEJANDRO;
	chara_name = "alejandro";
	resource_dir = "resource/chara/alejandro";
	fighter_int.resize(CHARA_ALEJANDRO_INT_MAX, 0);
	fighter_float.resize(CHARA_ALEJANDRO_FLOAT_MAX, 0.0);
	fighter_flag.resize(CHARA_ALEJANDRO_FLAG_MAX, false);
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadAlejandroStatusFunctions();
	set_current_move_script("default");}

void Alejandro::chara_id() {

}