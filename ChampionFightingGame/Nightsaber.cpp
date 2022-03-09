#include "Nightsaber.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Nightsaber::Nightsaber() {

}

Nightsaber::Nightsaber(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/nightsaber";
	chara_name = "nightsaber";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadNightsaberStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_NIGHTSABER;
}

void Nightsaber::chara_id() {

}

void Nightsaber::chara_status() {
	(this->*nightsaber_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Nightsaber::chara_enter_status() {
	(this->*nightsaber_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Nightsaber::chara_exit_status() {
	(this->*nightsaber_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}