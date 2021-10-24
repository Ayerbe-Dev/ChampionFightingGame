#include "RoyFireball.h"
#include "Game.h"

RoyFireball::RoyFireball(SDL_Renderer* renderer, int id) {
	this->id = -1;
	resource_dir = "resource/projectile/roy_fireball";
	this->projectile_kind = PROJECTILE_KIND_ROY_FIREBALL;
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

	});
}

void RoyFireball::status_default() {
	change_status(PROJECTILE_STATUS_MOVE);
}