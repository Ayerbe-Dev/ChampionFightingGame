#include "RoyFireball.h"
#include "Game.h"

RoyFireball::RoyFireball(SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor) {
	resource_dir = "resource/projectile/roy_fireball";
	this->projectile_kind = PROJECTILE_KIND_ROY_FIREBALL;
	load_unique_params();
	loadRoyFireballACMD();
	loadRoyFireballStatusFunctions();
	this->base_texture = loadTexture("resource/projectile/roy_fireball/sprite/sprite.png", renderer);
	this->fighter_instance_accessor = fighter_instance_accessor;
	superInit(renderer);
}

void RoyFireball::loadRoyFireballStatusFunctions() {

}

void RoyFireball::loadRoyFireballACMD() {
	script("default", [this]() {
		return;
	});
	script("move", [this]() {
		if (is_excute_frame(1, 0)) {
			cout << fighter_instance_accessor->fighter_instance[id]->pos.x << endl;
			cout << fighter_instance_accessor->fighter_instance[id]->pos.y << endl;
			cout << pos.x << endl;
			cout << pos.y << endl;
			new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_KNOCKDOWN, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
		}
	});
}

void RoyFireball::status_default() {
	change_status(PROJECTILE_STATUS_MOVE);
}