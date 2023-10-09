#include "RowanFireball.h"
#include "RowanFireballConstants.h"

RowanFireball::RowanFireball() {}

RowanFireball::RowanFireball(int id, Player* player) {
	this->id = id;
	this->player = player;
	projectile_kind = PROJECTILE_KIND_ROWAN_FIREBALL;
	projectile_name = "rowan_fireball";
	resource_dir = "resource/projectile/rowan_fireball";
	projectile_int.resize(PROJECTILE_ROWAN_FIREBALL_INT_MAX, 0);
	projectile_float.resize(PROJECTILE_ROWAN_FIREBALL_FLOAT_MAX, 0.0);
	projectile_flag.resize(PROJECTILE_ROWAN_FIREBALL_FLAG_MAX, false);

	load_projectile();
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