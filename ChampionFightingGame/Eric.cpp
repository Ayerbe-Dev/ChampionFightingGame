#include "Eric.h"
#include "utils.h"
#include "Battle.h"
#include "EricFireball.fwd.h"
#include "EricFireball.h"
extern SDL_Renderer* g_renderer;

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

	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_ERIC_FIREBALL, id, player_info, fighter_accessor);
	delete (projectiles[0]);
	this->projectiles[0] = projectile_objects[0]->get_projectile();
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