#include "Atlas.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Atlas::Atlas() {

}

Atlas::Atlas(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	chara_name = "atlas";
	resource_dir = "resource/chara/atlas";
	if (!crash_to_debug) {
		load_params();
	}
	loadAtlasACMD();
	loadAtlasStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ATLAS;
	this->base_texture = loadTexture("resource/chara/atlas/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}
}

void Atlas::chara_id() {

}

void Atlas::chara_status() {
	(this->*atlas_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Atlas::chara_enter_status() {
	(this->*atlas_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Atlas::chara_exit_status() {
	(this->*atlas_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}