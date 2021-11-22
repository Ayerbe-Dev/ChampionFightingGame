#include "Leon.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Leon::Leon() {

}

Leon::Leon(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	if (player_info->chara_kind == CHARA_KIND_LEON) {
		this->chara_kind = CHARA_KIND_LEON;
		resource_dir = "resource/chara/leon";
		chara_name = "leon";
		this->base_texture = loadTexture("resource/chara/leon/sprite/sprite.png");
	}
	else {
		this->chara_kind = CHARA_KIND_CHAMELEON;
		resource_dir = "resource/chara/chameleon";
		chara_name = "chameleon";
		this->base_texture = loadTexture("resource/chara/chameleon/sprite/sprite.png");
	}
	if (!crash_to_debug) {
		load_params();
	}
	loadLeonACMD();
	loadLeonStatusFunctions();
	set_current_move_script("default");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->leon = this;
}

void Leon::chara_id() {

}

void Leon::chara_status() {
	(this->*leon_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Leon::chara_enter_status() {
	(this->*leon_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Leon::chara_exit_status() {
	(this->*leon_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}