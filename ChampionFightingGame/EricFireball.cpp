#include "EricFireball.h"
#include "Game.h"

EricFireball::EricFireball(SDL_Renderer* renderer, int id, FighterInstanceAccessor *fighter_instance_accessor) {
	resource_dir = "resource/projectile/eric_fireball";
	this->projectile_kind = PROJECTILE_KIND_ERIC_FIREBALL;
	this->base_texture = loadTexture("resource/projectile/eric_fireball/sprite/sprite.png", renderer);
	this->fighter_instance_accessor = fighter_instance_accessor;
	superInit(renderer);
}