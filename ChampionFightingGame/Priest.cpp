#include "Priest.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Priest::Priest() {

}

Priest::Priest(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/priest";
	chara_name = "priest";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadPriestStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_PRIEST;
}

void Priest::chara_id() {

}

void Priest::chara_status() {
	(this->*priest_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Priest::chara_enter_status() {
	(this->*priest_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Priest::chara_exit_status() {
	(this->*priest_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}