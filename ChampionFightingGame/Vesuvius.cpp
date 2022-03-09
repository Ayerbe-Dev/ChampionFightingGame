#include "Vesuvius.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Vesuvius::Vesuvius() {

}

Vesuvius::Vesuvius(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/vesuvius";
	chara_name = "vesuvius";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadVesuviusStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_VESUVIUS;
}

void Vesuvius::chara_id() {

}

void Vesuvius::chara_status() {
	(this->*vesuvius_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Vesuvius::chara_enter_status() {
	(this->*vesuvius_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Vesuvius::chara_exit_status() {
	(this->*vesuvius_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}