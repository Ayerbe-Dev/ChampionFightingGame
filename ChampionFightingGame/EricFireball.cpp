#include "EricFireball.h"
#include "EricFireballConstants.h"

EricFireball::EricFireball() {}

EricFireball::EricFireball(int id, Player* player) {
	this->id = id;
	this->player = player;
	projectile_kind = PROJECTILE_KIND_ERIC_FIREBALL;
	projectile_name = "eric_fireball";
	resource_dir = "resource/projectile/eric_fireball";
	projectile_int.resize(PROJECTILE_ERIC_FIREBALL_INT_MAX, 0);
	projectile_float.resize(PROJECTILE_ERIC_FIREBALL_FLOAT_MAX, 0.0);
	projectile_flag.resize(PROJECTILE_ERIC_FIREBALL_FLAG_MAX, false);
	projectile_flag.resize(PROJECTILE_ERIC_FIREBALL_STRING_MAX, "");

	load_projectile();
}

void EricFireball::projectile_unique_main() {
	if (projectile_int[PROJECTILE_INT_HEALTH] == 0) {

	}
}

void EricFireball::unique_activate() {

}

void EricFireball::unique_deactivate() {

}