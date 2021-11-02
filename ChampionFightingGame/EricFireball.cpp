#include "EricFireball.h"
#include "Game.h"

EricFireball::EricFireball(SDL_Renderer* renderer, int id, PlayerInfo* player_info, FighterInstanceAccessor *fighter_instance_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/projectile/eric_fireball";
	this->projectile_kind = PROJECTILE_KIND_ERIC_FIREBALL;
	loadEricFireballACMD();
	loadEricFireballStatusFunctions();
	this->base_texture = loadTexture("resource/projectile/eric_fireball/sprite/sprite.png", renderer);
	this->fighter_instance_accessor = fighter_instance_accessor;
	superInit(renderer);
}

void EricFireball::loadEricFireballStatusFunctions() {

}

void EricFireball::loadEricFireballACMD() {
	script("default", [this]() {
		return;
	});
	script("move", [this]() {
		if (is_excute_frame(0)) {
			new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true);
			new_hitbox(2, 1, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true);
			new_hitbox(3, 2, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true, true);
		}
	});
}

void EricFireball::tickOnceProjectileUnique() {
	if (projectile_int[PROJECTILE_INT_HEALTH] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void EricFireball::projectile_unique_status() {
	(this->*eric_fireball_status[status_kind - PROJECTILE_STATUS_MAX])();
}

void EricFireball::projectile_unique_enter_status() {
	(this->*eric_fireball_enter_status[status_kind - PROJECTILE_STATUS_MAX])();
}

void EricFireball::projectile_unique_exit_status() {
	(this->*eric_fireball_exit_status[status_kind - PROJECTILE_STATUS_MAX])();
}

void EricFireball::status_default() {
	change_status(PROJECTILE_STATUS_MOVE);
}