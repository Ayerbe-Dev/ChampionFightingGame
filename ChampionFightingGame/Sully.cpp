#include "Sully.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Sully::Sully() {

}

Sully::Sully(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/sully";
	chara_name = "sully";
	if (!crash_to_debug) {
		load_params();
	}
	loadCharaMoveScripts();
	loadSullyStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_SULLY;
	this->base_texture = loadTexture("resource/chara/sully/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->sully = this;
}

void Sully::chara_id() {

}

void Sully::chara_status() {
	(this->*sully_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Sully::chara_enter_status() {
	(this->*sully_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Sully::chara_exit_status() {
	(this->*sully_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}