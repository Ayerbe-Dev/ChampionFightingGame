#include "Alejandro.h"
#include "Battle.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Alejandro::Alejandro() {

}

Alejandro::Alejandro(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/alejandro";
	chara_name = "alejandro";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadAlejandroStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ALEJANDRO;

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}
}

void Alejandro::chara_id() {

}

void Alejandro::chara_status() {
	(this->*alejandro_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Alejandro::chara_enter_status() {
	(this->*alejandro_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Alejandro::chara_exit_status() {
	(this->*alejandro_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}