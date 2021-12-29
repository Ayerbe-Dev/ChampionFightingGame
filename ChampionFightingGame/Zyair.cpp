#include "Zyair.h"
#include "Battle.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Zyair::Zyair() {

}

Zyair::Zyair(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/zyair";
	chara_name = "zyair";
	if (!crash_to_debug) {
		load_params();
	}
	loadCharaMoveScripts();
	loadZyairStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ZYAIR;
	this->base_texture = loadSDLTexture("resource/chara/zyair/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->zyair = this;
}

void Zyair::chara_id() {

}

void Zyair::chara_status() {
	(this->*zyair_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Zyair::chara_enter_status() {
	(this->*zyair_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Zyair::chara_exit_status() {
	(this->*zyair_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}