#include "Tessa.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Tessa::Tessa() {

}

Tessa::Tessa(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/tessa";
	chara_name = "tessa";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadTessaStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_TESSA;
}

void Tessa::chara_id() {

}

void Tessa::chara_status() {
	(this->*tessa_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Tessa::chara_enter_status() {
	(this->*tessa_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Tessa::chara_exit_status() {
	(this->*tessa_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}