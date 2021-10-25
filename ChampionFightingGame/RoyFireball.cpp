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
	pStatus[PROJECTILE_ROY_FIREBALL_STATUS_HOVER] = &ProjectileInstance::status_roy_fireball_hover;
	pEnter_status[PROJECTILE_ROY_FIREBALL_STATUS_HOVER] = &ProjectileInstance::enter_status_roy_fireball_hover;
	pExit_status[PROJECTILE_ROY_FIREBALL_STATUS_HOVER] = &ProjectileInstance::exit_status_roy_fireball_hover;

	pStatus[PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED] = &ProjectileInstance::status_roy_fireball_punched;
	pEnter_status[PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED] = &ProjectileInstance::enter_status_roy_fireball_punched;
	pExit_status[PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED] = &ProjectileInstance::exit_status_roy_fireball_punched;

	pStatus[PROJECTILE_ROY_FIREBALL_STATUS_KICKED] = &ProjectileInstance::status_roy_fireball_kicked;
	pEnter_status[PROJECTILE_ROY_FIREBALL_STATUS_KICKED] = &ProjectileInstance::enter_status_roy_fireball_kicked;
	pExit_status[PROJECTILE_ROY_FIREBALL_STATUS_KICKED] = &ProjectileInstance::exit_status_roy_fireball_kicked;

	pStatus[PROJECTILE_ROY_FIREBALL_STATUS_FALL] = &ProjectileInstance::status_roy_fireball_fall;
	pEnter_status[PROJECTILE_ROY_FIREBALL_STATUS_FALL] = &ProjectileInstance::enter_status_roy_fireball_fall;
	pExit_status[PROJECTILE_ROY_FIREBALL_STATUS_FALL] = &ProjectileInstance::exit_status_roy_fireball_fall;

	pStatus[PROJECTILE_ROY_FIREBALL_STATUS_GROUND] = &ProjectileInstance::status_roy_fireball_ground;
	pEnter_status[PROJECTILE_ROY_FIREBALL_STATUS_GROUND] = &ProjectileInstance::enter_status_roy_fireball_ground;
	pExit_status[PROJECTILE_ROY_FIREBALL_STATUS_GROUND] = &ProjectileInstance::exit_status_roy_fireball_ground;
}

void RoyFireball::loadRoyFireballACMD() {
	script("default", [this]() {
		return;
	});
	script("hover", [this]() {

	});
	script("punched", [this]() {
		if (is_excute_frame(1, 0)) {
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
			}
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				new_hitbox(0, 0, 40, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
			}
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				new_hitbox(0, 0, 50, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
			}
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
				new_hitbox(1, 1, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
			}
		}
	});
	script("kicked", [this]() {
		if (is_excute_frame(1, 0)) {
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
			}
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				new_hitbox(0, 0, 40, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
			}
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				new_hitbox(0, 0, 50, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
			}
			if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				new_hitbox(0, 0, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
				new_hitbox(1, 1, 30, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, 10, 10, projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NONE, 10.0, 0.0, 0.0, 1.0, true);
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

void RoyFireball::tickOnceProjectileUnique() {
	if (projectile_int[PROJECTILE_INT_HEALTH] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void RoyFireball::status_default() {
	if (fighter_instance_accessor->fighter_instance[owner_id]->chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		projectile_int[PROJECTILE_INT_HEALTH] = 2;
	}
	change_status(PROJECTILE_ROY_FIREBALL_STATUS_HOVER);
}

void RoyFireball::status_hit() {
	if (is_anim_end) {
		id = -1;
	}
}

void RoyFireball::enter_status_hit() {
	change_anim("hit");
}

void RoyFireball::status_roy_fireball_hover() {
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		change_status(PROJECTILE_ROY_FIREBALL_STATUS_FALL);
	}
}

void RoyFireball::enter_status_roy_fireball_hover() {
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("hover_active_time", unique_param_table);
	change_anim("hover");
}

void RoyFireball::exit_status_roy_fireball_hover() {

}

void RoyFireball::status_roy_fireball_punched() {
	if (roy->roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_L) {
		pos.x += get_param_float("punch_move_x_speed_l", unique_param_table) * facing_dir;
	}
	else if (roy->roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] == SPECIAL_LEVEL_M) {
		pos.x += get_param_float("punch_move_x_speed_m", unique_param_table) * facing_dir;
	}
	else {
		pos.x += get_param_float("punch_move_x_speed_h", unique_param_table) * facing_dir;
	}
}

void RoyFireball::enter_status_roy_fireball_punched() {
	projectile_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("punch_active_time", unique_param_table);
	change_anim("punched");
}

void RoyFireball::exit_status_roy_fireball_punched() {

}

void RoyFireball::status_roy_fireball_kicked() {

}

void RoyFireball::enter_status_roy_fireball_kicked() {
	change_anim("kicked");
}

void RoyFireball::exit_status_roy_fireball_kicked() {

}

void RoyFireball::status_roy_fireball_fall() {
	if (is_anim_end) {
		change_status(PROJECTILE_ROY_FIREBALL_STATUS_GROUND);
	}
}

void RoyFireball::enter_status_roy_fireball_fall() {
	change_anim("fall");
}

void RoyFireball::exit_status_roy_fireball_fall() {

}

void RoyFireball::status_roy_fireball_ground() {
	if (is_anim_end) {
		id = -1;
	}
}

void RoyFireball::enter_status_roy_fireball_ground() {
	change_anim("ground");
}

void RoyFireball::exit_status_roy_fireball_ground() {

}