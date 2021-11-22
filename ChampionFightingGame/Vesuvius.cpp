#include "Vesuvius.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Vesuvius::Vesuvius() {

}

Vesuvius::Vesuvius(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/vesuvius";
	chara_name = "vesuvius";
	if (!crash_to_debug) {
		load_params();
	}
	loadCharaMoveScripts();
	loadVesuviusStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_VESUVIUS;
	this->base_texture = loadTexture("resource/chara/vesuvius/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->vesuvius = this;
}

void Vesuvius::chara_id() {

}

void Vesuvius::chara_status() {
	(this->*vesuvius_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Vesuvius::chara_enter_status() {
	(this->*vesuvius_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Vesuvius::chara_exit_status() {
	(this->*vesuvius_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}