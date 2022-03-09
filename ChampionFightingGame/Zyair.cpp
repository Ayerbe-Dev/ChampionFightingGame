#include "Zyair.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Zyair::Zyair() {

}

Zyair::Zyair(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/zyair";
	chara_name = "zyair";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadZyairStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ZYAIR;
}

void Zyair::chara_id() {

}

void Zyair::chara_status() {
	(this->*zyair_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Zyair::chara_enter_status() {
	(this->*zyair_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Zyair::chara_exit_status() {
	(this->*zyair_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}