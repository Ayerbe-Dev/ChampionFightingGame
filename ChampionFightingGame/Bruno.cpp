#include "Bruno.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Bruno::Bruno() {

}

Bruno::Bruno(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/bruno";
	chara_name = "bruno";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadBrunoStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_BRUNO;
}

void Bruno::chara_id() {

}

void Bruno::chara_status() {
	(this->*bruno_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Bruno::chara_enter_status() {
	(this->*bruno_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Bruno::chara_exit_status() {
	(this->*bruno_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}