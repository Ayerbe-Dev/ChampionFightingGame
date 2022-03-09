#include "Roy.h"
#include "Battle.h"
#include "RoyFireball.h"
#include "ProjectileInterface.h"

Roy::Roy() {

}

Roy::Roy(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/roy";
	chara_name = "roy";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadRoyStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ROY;

	projectiles[0] = create_projectile(PROJECTILE_KIND_ROY_FIREBALL, id, player_info, fighter_accessor);
	RoyFireball* roy_fireball = (RoyFireball*)projectiles[0];
	roy_fireball->roy = this;
}

void Roy::chara_id() {

}

void Roy::loadCharaSounds() {
	loadVC(ROY_VC_ATTACK_01);
}

void Roy::chara_status() {
	(this->*roy_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Roy::chara_enter_status() {
	(this->*roy_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Roy::chara_exit_status() {
	(this->*roy_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}