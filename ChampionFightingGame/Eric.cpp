#include "Eric.h"
#include "utils.h"
#include "Battle.h"
#include "EricFireball.h"
#include "ProjectileInterface.h"

Eric::Eric() {
	
}

Eric::Eric(int id, PlayerInfo* player_info, FighterAccessor *fighter_accessor) {
	resource_dir = "resource/chara/eric";
	chara_name = "eric";
	if (!crash_to_debug) {
		load_params();
	}
	load_move_scripts();
	loadEricStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ERIC;

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

	projectiles[0] = create_projectile(PROJECTILE_KIND_ERIC_FIREBALL, id, player_info, fighter_accessor);
	EricFireball* eric_fireball = (EricFireball*)projectiles[0];
	eric_fireball->eric = this;
}

void Eric::chara_id() {

}

void Eric::chara_status() {
	(this->*eric_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Eric::chara_enter_status() {
	(this->*eric_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Eric::chara_exit_status() {
	(this->*eric_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}