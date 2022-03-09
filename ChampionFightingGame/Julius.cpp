#include "Julius.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Julius::Julius() {

}

Julius::Julius(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/julius";
	chara_name = "julius";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadJuliusStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_JULIUS;
}

void Julius::chara_id() {

}

void Julius::chara_status() {
	(this->*julius_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Julius::chara_enter_status() {
	(this->*julius_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Julius::chara_exit_status() {
	(this->*julius_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}