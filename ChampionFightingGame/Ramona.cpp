#include "Ramona.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Ramona::Ramona() {

}

Ramona::Ramona(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/ramona";
	chara_name = "ramona";
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

void Ramona::chara_status() {
	(this->*ramona_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Ramona::chara_enter_status() {
	(this->*ramona_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Ramona::chara_exit_status() {
	(this->*ramona_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}