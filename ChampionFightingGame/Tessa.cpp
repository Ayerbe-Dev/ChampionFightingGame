#include "Tessa.h"
#include "Battle.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Tessa::Tessa() {

}

Tessa::Tessa(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/tessa";
	chara_name = "tessa";
	if (!crash_to_debug) {
		load_params();
	}
	loadCharaMoveScripts();
	loadTessaStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_TESSA;
	this->base_texture = loadTexture("resource/chara/tessa/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->tessa = this;
}

void Tessa::chara_id() {

}

void Tessa::chara_status() {
	(this->*tessa_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Tessa::chara_enter_status() {
	(this->*tessa_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Tessa::chara_exit_status() {
	(this->*tessa_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}