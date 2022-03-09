#include "Angelica.h"
#include "Battle.h"
#include "ProjectileTemplate.h"

Angelica::Angelica() {

}

Angelica::Angelica(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/angelica";
	chara_name = "angelica";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadAngelicaStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_CHARA_TEMPLATE;
}

void Angelica::chara_id() {

}

void Angelica::chara_status() {
	(this->*angelica_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Angelica::chara_enter_status() {
	(this->*angelica_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Angelica::chara_exit_status() {
	(this->*angelica_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}