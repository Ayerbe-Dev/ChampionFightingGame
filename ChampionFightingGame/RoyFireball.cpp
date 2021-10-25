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
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
			}
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				new_hitbox(0, 0, 40, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
			}
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				new_hitbox(0, 0, 50, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
			}
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
				new_hitbox(1, 1, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0);
			}
		}
	});
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

void RoyFireball::status_default() {
	if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		projectile_int[PROJECTILE_INT_HEALTH] = 2;
	}
	change_status(PROJECTILE_STATUS_MOVE);
}

void RoyFireball::status_move() {
	if (roy->roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_L) {
		pos.x += get_param_float("move_x_speed_l", unique_param_table) * facing_dir;
	}
	else if (roy->roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_M) {
		pos.x += get_param_float("move_x_speed_m", unique_param_table) * facing_dir;
	}
	else {
		pos.x += get_param_float("move_x_speed_h", unique_param_table) * facing_dir;
	}
}