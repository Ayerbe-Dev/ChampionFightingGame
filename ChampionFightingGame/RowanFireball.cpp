#include "RowanFireball.h"
#include "RowanFireballConstants.h"

RowanFireball::RowanFireball() {}

RowanFireball::RowanFireball(int id, Player* player) {
	this->id = id;
	this->player = player;
	projectile_kind = PROJECTILE_KIND_ROWAN_FIREBALL;
	projectile_name = "rowan_fireball";
	resource_dir = "resource/projectile/rowan_fireball";
	object_int.resize(PROJECTILE_ROWAN_FIREBALL_INT_MAX, 0);
	object_float.resize(PROJECTILE_ROWAN_FIREBALL_FLOAT_MAX, 0.0);
	object_flag.resize(PROJECTILE_ROWAN_FIREBALL_FLAG_MAX, false);
	object_flag.resize(PROJECTILE_ROWAN_FIREBALL_STRING_MAX, "");
	status_script.resize(PROJECTILE_ROWAN_FIREBALL_STATUS_MAX, nullptr);
	enter_status_script.resize(PROJECTILE_ROWAN_FIREBALL_STATUS_MAX, nullptr);
	exit_status_script.resize(PROJECTILE_ROWAN_FIREBALL_STATUS_MAX, nullptr);

	load_projectile();
}

void RowanFireball::projectile_unique_main() {
	if (object_int[PROJECTILE_INT_HEALTH] <= 0 && active) {
		deactivate();
	}
}

void RowanFireball::unique_activate() {
	owner->object_flag[CHARA_ROWAN_FLAG_FIREBALL_ACTIVE] = true;
	if (owner->object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		object_int[PROJECTILE_INT_ATTACK_LEVEL] = 1;
		object_int[PROJECTILE_INT_HEALTH] = 2;
	}
	change_status(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER);
}

void RowanFireball::unique_deactivate() {
	owner->object_flag[CHARA_ROWAN_FLAG_FIREBALL_ACTIVE] = false;
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