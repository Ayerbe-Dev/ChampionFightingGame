#include "Atlas.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Atlas::Atlas() {

}

Atlas::Atlas(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	chara_name = "atlas";
	resource_dir = "resource/chara/atlas";
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

void Atlas::chara_status() {
	(this->*atlas_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Atlas::chara_enter_status() {
	(this->*atlas_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Atlas::chara_exit_status() {
	(this->*atlas_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}