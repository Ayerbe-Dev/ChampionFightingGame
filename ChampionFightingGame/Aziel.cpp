#include "Aziel.h"
#include "Battle.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Aziel::Aziel() {

}

Aziel::Aziel(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/aziel";
	chara_name = "aziel";
	if (!crash_to_debug) {
		load_params();
	}
	loadCharaMoveScripts();
	loadAzielStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_AZIEL;
	this->base_texture = loadTexture("resource/chara/aziel/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->aziel = this;
}

void Aziel::chara_id() {

}

void Aziel::chara_status() {
	(this->*aziel_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Aziel::chara_enter_status() {
	(this->*aziel_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Aziel::chara_exit_status() {
	(this->*aziel_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}