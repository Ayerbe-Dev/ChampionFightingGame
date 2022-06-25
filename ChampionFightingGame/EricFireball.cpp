#include "EricFireball.h"
#include "EricFireballConstants.h"

EricFireball::EricFireball() {}

EricFireball::EricFireball(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->projectile_kind = PROJECTILE_KIND_ERIC_FIREBALL;
	projectile_name = "eric_fireball";
	resource_dir = "resource/projectile/eric_fireball";
	projectile_int.resize(PROJECTILE_ERIC_FIREBALL_INT_MAX, 0);
	projectile_float.resize(PROJECTILE_ERIC_FIREBALL_FLOAT_MAX, 0.0);
	projectile_flag.resize(PROJECTILE_ERIC_FIREBALL_FLAG_MAX, false);

	loadEricFireballStatusFunctions();
	super_init();
}