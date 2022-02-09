#include "Bruno.h"
#include "Battle.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Bruno::Bruno() {

}

Bruno::Bruno(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/bruno";
	chara_name = "bruno";
	if (!crash_to_debug) {
		load_params();
	}
	loadCharaMoveScripts();
	loadBrunoStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_BRUNO;

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->bruno = this;
}

void Bruno::chara_id() {

}

void Bruno::chara_status() {
	(this->*bruno_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Bruno::chara_enter_status() {
	(this->*bruno_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Bruno::chara_exit_status() {
	(this->*bruno_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}