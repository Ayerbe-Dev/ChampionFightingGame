#include "Nightsaber.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Nightsaber::Nightsaber() {

}

Nightsaber::Nightsaber(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/nightsaber";
	chara_name = "nightsaber";
	if (!crash_to_debug) {
		load_params();
	}
	loadNightsaberACMD();
	loadNightsaberStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_NIGHTSABER;
	this->base_texture = loadTexture("resource/chara/nightsaber/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->nightsaber = this;
}

void Nightsaber::chara_id() {

}

void Nightsaber::chara_status() {
	(this->*nightsaber_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Nightsaber::chara_enter_status() {
	(this->*nightsaber_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Nightsaber::chara_exit_status() {
	(this->*nightsaber_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}