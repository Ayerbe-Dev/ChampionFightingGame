#include "RoyFireball.h"
#include "RoyFireballConstants.h"

RoyFireball::RoyFireball() {}

RoyFireball::RoyFireball(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->projectile_kind = PROJECTILE_KIND_ROY_FIREBALL;
	projectile_name = "roy_fireball";
	resource_dir = "resource/projectile/roy_fireball";
	projectile_int.resize(PROJECTILE_ROY_FIREBALL_INT_MAX, 0);
	projectile_float.resize(PROJECTILE_ROY_FIREBALL_FLOAT_MAX, 0.0);
	projectile_flag.resize(PROJECTILE_ROY_FIREBALL_FLAG_MAX, false);

	loadRoyFireballStatusFunctions();
	super_init();
}

/*
  (                                 _
   )                               /=>
  (  +____________________/\/\___ / /|
   .''._____________'._____      / /|/\
  : () :              :\ ----\|    \ )
   '..'______________.'0|----|      \
					0_0/____/        \
						|----    /----\
					   || -\\ --|      \
					   ||   || ||\      \
						\\____// '|      \
Bang! Bang!                     .'/       |
							   .:/        |
							   :/_________|
*/