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
	projectile_flag.resize(PROJECTILE_ROWAN_FIREBALL_STRING_MAX, "");

	load_projectile();
}

void RowanFireball::projectile_unique_main() {
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0 && active) {
		deactivate();
	}
}

void RowanFireball::unique_activate() {
	owner->fighter_flag[CHARA_ROWAN_FLAG_FIREBALL_ACTIVE] = true;
	if (owner->fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		projectile_int[PROJECTILE_INT_ATTACK_LEVEL] = 1;
		projectile_int[PROJECTILE_INT_HEALTH] = 2;
	}
	change_status(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER);
}

void RowanFireball::unique_deactivate() {
	owner->fighter_flag[CHARA_ROWAN_FLAG_FIREBALL_ACTIVE] = false;
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