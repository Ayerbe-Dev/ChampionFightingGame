#include "Priest.h"
#include "Battle.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Priest::Priest() {

}

Priest::Priest(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/priest";
	chara_name = "priest";
	if (!crash_to_debug) {
		load_params();
	}
	loadCharaMoveScripts();
	loadPriestStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_PRIEST;

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->priest = this;
}

void Priest::chara_id() {

}

void Priest::chara_status() {
	(this->*priest_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Priest::chara_enter_status() {
	(this->*priest_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Priest::chara_exit_status() {
	(this->*priest_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}