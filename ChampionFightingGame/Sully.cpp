#include "Sully.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Sully::Sully() {

}

Sully::Sully(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/sully";
	chara_name = "sully";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadSullyStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_SULLY;
}

void Sully::chara_id() {

}

void Sully::chara_status() {
	(this->*sully_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Sully::chara_enter_status() {
	(this->*sully_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Sully::chara_exit_status() {
	(this->*sully_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}