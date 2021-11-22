#include "Julius.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Julius::Julius() {

}

Julius::Julius(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/julius";
	chara_name = "julius";
	if (!crash_to_debug) {
		load_params();
	}
	loadJuliusACMD();
	loadJuliusStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_JULIUS;
	this->base_texture = loadTexture("resource/chara/template/julius/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->julius = this;
}

void Julius::chara_id() {

}

void Julius::chara_status() {
	(this->*julius_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Julius::chara_enter_status() {
	(this->*julius_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Julius::chara_exit_status() {
	(this->*julius_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}