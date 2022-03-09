#include "Leon.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Leon::Leon() {

}

Leon::Leon(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	if (player_info->chara_kind == CHARA_KIND_LEON) {
		this->chara_kind = CHARA_KIND_LEON;
		resource_dir = "resource/chara/leon";
		chara_name = "leon";
	}
	else {
		this->chara_kind = CHARA_KIND_CHAMELEON;
		resource_dir = "resource/chara/chameleon";
		chara_name = "chameleon";
	}
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadLeonStatusFunctions();
	set_current_move_script("default");
}

void Leon::chara_id() {

}

void Leon::chara_status() {
	(this->*leon_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Leon::chara_enter_status() {
	(this->*leon_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Leon::chara_exit_status() {
	(this->*leon_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}