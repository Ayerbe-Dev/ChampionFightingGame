#include "Norman.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Norman::Norman() {

}

Norman::Norman(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/norman";
	chara_name = "norman";
	if (!crash_to_debug) {
		load_params();
	}
	loadNormanACMD();
	loadNormanStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_NORMAN;
	this->base_texture = loadTexture("resource/chara/norman/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->norman = this;
}

void Norman::chara_id() {

}

void Norman::chara_status() {
	(this->*norman_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Norman::chara_enter_status() {
	(this->*norman_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Norman::chara_exit_status() {
	(this->*norman_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}