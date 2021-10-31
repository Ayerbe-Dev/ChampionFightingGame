#include "Roy.h"
#include "Game.h"
#include "RoyFireball.fwd.h"
#include "RoyFireball.h"

Roy::Roy() {

}

Roy::Roy(SDL_Renderer *renderer, int id, FighterInstanceAccessor* fighter_instance_accessor) {
	resource_dir = "resource/chara/roy";
	load_params();
	loadRoyACMD();
	loadRoyStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ROY;
	this->base_texture = loadTexture("resource/chara/roy/sprite/sprite.png", renderer);

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectile_objects[i] = new ProjectileInstance();
	}
	projectile_instances[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_ROY_FIREBALL, renderer, id, fighter_instance_accessor);
	delete (projectile_objects[0]);
	this->projectile_objects[0] = projectile_instances[0]->get_projectile();
	RoyFireball* roy_fireball_instance = (RoyFireball*)projectile_objects[0];
	roy_fireball_instance->roy = this;
}

void Roy::chara_id() {

}

void Roy::loadRoyStatusFunctions() {
	pStatus[CHARA_ROY_STATUS_SPECIAL_FIREBALL_START] = &FighterInstance::roy_status_special_fireball_start;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_START] = &FighterInstance::roy_enter_status_special_fireball_start;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_START] = &FighterInstance::roy_exit_status_special_fireball_start;

	pStatus[CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH] = &FighterInstance::roy_status_special_fireball_punch;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH] = &FighterInstance::roy_enter_status_special_fireball_punch;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH] = &FighterInstance::roy_exit_status_special_fireball_punch;

	pStatus[CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK] = &FighterInstance::roy_status_special_fireball_kick;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK] = &FighterInstance::roy_enter_status_special_fireball_kick;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK] = &FighterInstance::roy_exit_status_special_fireball_kick;

	pStatus[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START] = &FighterInstance::roy_status_special_uppercut_start;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START] = &FighterInstance::roy_enter_status_special_uppercut_start;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START] = &FighterInstance::roy_exit_status_special_uppercut_start;

	pStatus[CHARA_ROY_STATUS_SPECIAL_UPPERCUT] = &FighterInstance::roy_status_special_uppercut;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT] = &FighterInstance::roy_enter_status_special_uppercut;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT] = &FighterInstance::roy_exit_status_special_uppercut;

	pStatus[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL] = &FighterInstance::roy_status_special_uppercut_fall;
	pEnter_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL] = &FighterInstance::roy_enter_status_special_uppercut_fall;
	pExit_status[CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL] = &FighterInstance::roy_exit_status_special_uppercut_fall;
}

void Roy::loadRoyACMD() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 3;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("walk_f", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("walk_b", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("dash_f", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("dash_b", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("crouch_d", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
		if (is_excute_frame(2)) {
			clear_hurtbox_all();
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 80 });
		}
	});
	script("crouch", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -38, 0 }, GameCoordinate{ 43, 71 });
		}
	});
	script("crouch_u", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("jump_squat", [this]() {

	});
	script("jump", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -25, 20 }, GameCoordinate{ 20, 120 });
			new_hurtbox(1, GameCoordinate{ -35, 75 }, GameCoordinate{ 35, 115 });
		}
	});
	script("jump_f", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 40 }, GameCoordinate{ 35, 105 });
		}
	});
	script("jump_b", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 40 }, GameCoordinate{ 35, 105 });
		}
	});
	script("fall", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 120 });
			new_hurtbox(1, GameCoordinate{ -35, 75 }, GameCoordinate{ 35, 115 });
		}
	});
	script("stand_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("high_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("crouch_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("jump_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("parry_start", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 4;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
		if (is_excute_frame(1)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = true;
		}
		if (is_excute_wait(4)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = false;
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = true;
		}
	});
	script("parry_start_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 4;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
		if (is_excute_frame(1)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = true;
		}
		if (is_excute_wait(4)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = false;
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = true;
		}
	});
	script("parry_mid", [this]() {

	});
	script("parry_high", [this]() {

	});
	script("parry_low", [this]() {

	});
	script("parry_air", [this]() {

	});
	script("hitstun_parry", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 5;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = true;
			chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
		}
		if (is_excute_wait(2)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = false;
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = true;
		}
		if (is_excute_wait(9)) {
			reenter_last_anim();
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = false;
		}
	});
	script("hitstun_parry_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 5;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = true;
			chara_int[CHARA_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
		}
		if (is_excute_wait(2)) {
			chara_flag[CHARA_FLAG_PARRY_ACTIVE] = false;
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = true;
		}
		if (is_excute_wait(9)) {
			reenter_last_anim();
			chara_flag[CHARA_FLAG_ENABLE_COUNTERHIT] = false;
		}
	});
	script("stand_lp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
			new_hitbox(0, 0, 30, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 0.0, 0.0, 0.0, 0.0, false);
		}
		if (is_excute_frame(4)) {
			new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ 35,70 }, GameCoordinate{ 90, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
		}
	});
	script("stand_mp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
			new_hitbox(0, 0, 40, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 0.0, 0.0, 0.0, 0.0, false);
		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 0, 40, 5, 1.2, 1, GameCoordinate{ 35,70 }, GameCoordinate{ 90, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
		}
	});
	script("stand_hp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
			new_hitbox(0, 0, 50, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 0.0, 0.0, 0.0, 0.0, true);
		}
		if (is_excute_frame(10)) {
			new_hitbox(1, 0, 50, 5, 1.2, 1, GameCoordinate{ 25,70 }, GameCoordinate{ 90, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, true);
		}
		if (is_excute_wait(2)) {
			new_hitbox(2, 0, 60, 10, 1.2, 1, GameCoordinate{ 80,70 }, GameCoordinate{ 100, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 20, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_CRUMPLE, HIT_STATUS_CRUMPLE, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, true);
		}
		if (is_excute_wait(3)) {
			clear_hitbox_all();
		}
	});
	script("stand_lk", [this]() {

	});
	script("stand_mk", [this]() {

	});
	script("stand_hk", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 0, 90 });
		}
		if (is_excute_frame(2)) {
			new_hurtbox(1, GameCoordinate{ -15, 100 }, GameCoordinate{ 25, 60 });
		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 0, 50, 10, 1.2, 1, GameCoordinate{ 25,60 }, GameCoordinate{ 60, 120 }, HITBOX_KIND_NORMAL, 20, 40, 10, SITUATION_HIT_GROUND_AIR, 4, 15, 12, 70, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 3, 3, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 20, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_AERIAL, 20.0, 0.0, 2.0, 1.0, true);
		}
		if (is_excute_frame(8)) {
			clear_hitbox_all();
		}
		if (is_excute_frame(12)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("crouch_lp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -38, 0 }, GameCoordinate{ 45, 71 });		
		}
		if (is_excute_frame(4)) {
			new_hurtbox(1, GameCoordinate{ 21, 43 }, GameCoordinate{ 68, 64 });
			new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ 42,43 }, GameCoordinate{ 76, 64 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
		}
		if (is_excute_wait(2)) {
			clear_hitbox_all();
		}
		if (is_excute_wait(2)) {
			clear_hurtbox(1);
		}
	});
	script("crouch_mp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -38, 0 }, GameCoordinate{ 45, 71 });
		}
		if (is_excute_frame(4)) {
			new_hurtbox(1, GameCoordinate{ 21, 43 }, GameCoordinate{ 68, 64 });
			new_hitbox(1, 0, 40, 5, 1.2, 1, GameCoordinate{ 42,43 }, GameCoordinate{ 76, 64 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
		}
		if (is_excute_wait(2)) {
			clear_hitbox_all();
		}
		if (is_excute_wait(2)) {
			clear_hurtbox(1);
		}
	});
	script("crouch_hp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -45, 0 }, GameCoordinate{ 45, 25 });
			new_hurtbox(1, GameCoordinate{ -30, 20 }, GameCoordinate{ 25, 73 });
		}
		if (is_excute_frame(6)) {
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 40, 55 });
		}
		if (is_excute_wait(2)) {
			new_hitbox(1, 0, 50, 5, 1.2, 1, GameCoordinate{ 25,55 }, GameCoordinate{ -10, 140 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 14.0, 1.0, 14.0, 1.0, true);
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
		}
	});
	script("crouch_lk", [this]() {

	});
	script("crouch_mk", [this]() {

	});
	script("crouch_hk", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
		}
		if (is_excute_frame(3)) {

		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 0, 50, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, ATTACK_HEIGHT_LOW, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_KNOCKDOWN, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, true);
		}

	});
	script("jump_lp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -45, 0 }, GameCoordinate{ 0, 50 });
			new_hurtbox(1, GameCoordinate{ -15, 10 }, GameCoordinate{ 15, 100 });
			new_hurtbox(2, GameCoordinate{ 10, 40 }, GameCoordinate{ 35, 100 });
		}
		if (is_excute_frame(2)) {
			new_hurtbox(3, GameCoordinate{ 5, 55 }, GameCoordinate{ 40, 95 });
		}
		if (is_excute_wait(1)) {
			max_ticks = 2;
			new_hurtbox(1, GameCoordinate{ -15, 10 }, GameCoordinate{ 15, 75 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 75 });
			new_hurtbox(3, GameCoordinate{ 5, 25 }, GameCoordinate{ 45, 75 });
			new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ 20,20 }, GameCoordinate{ 50, 60 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 8, 6, 4, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 3, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 4.0, false);
		}
	});
	script("jump_mp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -55, 15 }, GameCoordinate{ 0, 100 });
			new_hurtbox(1, GameCoordinate{ -10, 25 }, GameCoordinate{ 27, 80 });
			new_hurtbox(2, GameCoordinate{ -65, 70 }, GameCoordinate{ 5, 100 });
		}
		if (is_excute_wait(4)) {
			new_hurtbox(1, GameCoordinate{ -10, 25 }, GameCoordinate{ 23, 50 });
			new_hurtbox(2, GameCoordinate{ -45, 70 }, GameCoordinate{ 5, 100 });
		}
		if (is_excute_wait(2)) {
			new_hurtbox(1, GameCoordinate{ 0, 50 }, GameCoordinate{ 40, 80 });
			new_hurtbox(3, GameCoordinate{ 15, 45 }, GameCoordinate{ 55, 70 });
			new_hitbox(1, 0, 30, 15, 1.2, 1, GameCoordinate{ 45,35 }, GameCoordinate{ 75, 55 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 8, 8, 6, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 3, 5, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 4.0, false);
		}
		if (is_excute_wait(2)) {
			clear_hitbox_all();
		}
		if (is_excute_frame(13)) {
			clear_hurtbox(3);
		}
	});
	script("jump_hp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -40, 25 }, GameCoordinate{ 40, 65 });
			new_hurtbox(1, GameCoordinate{ -30, 30 }, GameCoordinate{ 30, 110 });
		}
		if (is_excute_frame(6)) {
			new_hurtbox(1, GameCoordinate{ -10, 30 }, GameCoordinate{ 30, 110 });
			new_hurtbox(2, GameCoordinate{ -25, 75 }, GameCoordinate{ 75, 95 });
			new_hitbox(1, 0, 60, 15, 1.2, 1, GameCoordinate{ 60,75 }, GameCoordinate{ 95, 95 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 10, 8, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 7, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 10.0, false);
			new_hitbox(2, 0, 50, 15, 1.2, 1, GameCoordinate{ 20,75 }, GameCoordinate{ 95, 95 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 10, 8, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 7, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 10.0, false);
		}
		if (is_excute_wait(5)) {
			clear_hitbox(1);
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
			clear_hurtbox(2);
		}
	});
	script("jump_lk", [this]() {

	});
	script("jump_mk", [this]() {

	});
	script("jump_hk", [this]() {

	});
	script("grab", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
		if (is_excute_frame(3)) {
			new_grabbox(0, GameCoordinate{ 15, 55 }, GameCoordinate{ 70, 100 }, GRABBOX_KIND_NORMAL, SITUATION_HIT_GROUND_AIR, CHARA_STATUS_THROW, CHARA_STATUS_GRABBED);
		}
	});
	script("throw_f", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = -60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 5);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(13)) {
			damage_opponent(30.0, 1, 15.0, 8.0);
			change_opponent_status(CHARA_STATUS_THROWN);
		}
	});
	script("throw_b", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = 60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 5);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(10)) {
			damage_opponent(30.0, -1, -15.0, 8.0);
			change_opponent_status(CHARA_STATUS_THROWN);
		}
	});
	script("grab_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
		if (is_excute_frame(3)) {
			new_grabbox(0, GameCoordinate{ 15, 55 }, GameCoordinate{ 70, 100 }, GRABBOX_KIND_HITSTUN, SITUATION_HIT_GROUND_AIR, CHARA_STATUS_THROW_AIR, CHARA_STATUS_GRABBED);
		}
	});
	script("throw_f_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = -60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 5);			
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(13)) {
			damage_opponent(30.0, 1, 15.0, 8.0);
			change_opponent_status(CHARA_STATUS_THROWN);
		}
	});
	script("throw_b_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = 60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 5);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(10)) {
			damage_opponent(30.0, -1, -15.0, 8.0);
			change_opponent_status(CHARA_STATUS_THROWN);
		}
	});
	script("stand_hitstun_l", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("stand_hitstun_m", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("stand_hitstun_h", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("crouch_hitstun_l", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("crouch_hitstun_m", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("crouch_hitstun_h", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("jump_hitstun", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("launch_start", [this]() {

	});
	script("launch", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("knockdown_start", [this]() {

	});
	script("knockdown_wait", [this]() {

	});
	script("wakeup", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
		}
	});
	script("crumple", [this]() {

	});
	script("landing", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("landing_hitstun", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("special_fireball_start", [this]() {
		if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
			if (is_excute_frame(11)) {
				init_projectile(0, GameCoordinate{ 50,100 });
			}
		}
		else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
			if (is_excute_frame(9)) {
				max_ticks++;
			}
			if (is_excute_frame(14)) {
				init_projectile(0, GameCoordinate{ 50,100 });
			}
		}
		else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
			if (is_excute_frame(8)) {
				max_ticks++;
			}
			if (is_excute_frame(15)) {
				init_projectile(0, GameCoordinate{ 50,100 });
			}
		}
		else {
			if (is_excute_frame(11)) {
				init_projectile(0, GameCoordinate{ 50,100 });
			}
		}
	});
	script("special_fireball_punch", [this]() {
		if (is_excute_frame(0)) {

		}
		if (is_excute_frame(12)) {
			projectile_objects[0]->change_status(PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED);
		}
	});
	script("special_fireball_kick", [this]() {
		if (is_excute_frame(0)) {

		}
		if (is_excute_frame(20)) {
			projectile_objects[0]->change_status(PROJECTILE_ROY_FIREBALL_STATUS_KICKED);
		}
	});
	script("special_uppercut_start", [this]() {
		if (is_excute_frame(0)) {
			if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
				new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
				new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
			}
		}
		if (is_excute_frame(4)) {
			max_ticks = 2;
			if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(1, 0, 60, 5, 1.2, 1, GameCoordinate{ 15,55 }, GameCoordinate{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, -10, 4, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 1, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
			}
			else {
				new_hitbox(1, 0, 60, 5, 1.2, 1, GameCoordinate{ 15,55 }, GameCoordinate{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, -10, 4, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 1, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
			}
		}
		if (is_excute_wait(2)) {
			clear_hitbox_all();
		}
	});
	script("special_uppercut_rise", [this]() {
		if (is_excute_frame(4)) {
			if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ 0,5 }, GameCoordinate{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 6, 1, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 20.0, 1.0, 12.0, 3.0, false);

			}
			else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				new_hitbox(1, 0, 40, 5, 1.2, 1, GameCoordinate{ 0,5 }, GameCoordinate{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 6, 1, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 25.0, 1.0, 12.0, 3.0, false);
			}
			else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				new_hitbox(1, 0, 50, 5, 1.2, 1, GameCoordinate{ 0,5 }, GameCoordinate{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 6, 1, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 30.0, 1.0, 12.0, 3.0, false);
			}
			else {
				new_hitbox(1, 0, 70, 5, 1.2, 1, GameCoordinate{ 0,5 }, GameCoordinate{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 6, 1, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 30.0, 1.0, 12.0, 3.0, false);
			}
		}
	});
	script("special_uppercut", [this]() {
		if (is_excute_frame(0)) {
			if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ 0,85 }, GameCoordinate{ 40, 150 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_AIR, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 6, 6, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 20.0, 1.0, 12.0, 3.0, false);
			}
			else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				new_hitbox(1, 0, 40, 5, 1.2, 1, GameCoordinate{ 0,85 }, GameCoordinate{ 40, 150 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 6, 7, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 25.0, 1.0, 12.0, 3.0, false);
			}
			else if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				new_hitbox(1, 0, 50, 5, 1.2, 1, GameCoordinate{ 0,85 }, GameCoordinate{ 40, 150 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 6, 6, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 30.0, 1.0, 12.0, 3.0, false);
			}
			else {
				new_hitbox(1, 0, 70, 5, 1.2, 1, GameCoordinate{ 0,85 }, GameCoordinate{ 40, 150 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 6, 6, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 30.0, 1.0, 12.0, 3.0, false);
			}
		}
	});
}

bool Roy::specific_ground_status_act() {
	if (get_special_input(SPECIAL_KIND_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_EX;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_FIREBALL_START);
	}

	if (get_special_input(SPECIAL_KIND_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_EX;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_START);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status_after_hitlag(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
	}
	return false;
}

bool Roy::specific_air_status_act() {
	return false;
}

bool Roy::specific_status_attack() {
	if (chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (situation_kind == CHARA_SITUATION_GROUND && specific_ground_status_act()) {
			return true;
		}
		else if (situation_kind == CHARA_SITUATION_AIR && specific_air_status_act()) {
			return true;
		}
		else {
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_LP, CHARA_SITUATION_GROUND)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_MP, CHARA_SITUATION_GROUND)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_MP, BUTTON_HP, CHARA_SITUATION_GROUND)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_LP, CHARA_SITUATION_AIR)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_MP, CHARA_SITUATION_AIR)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_LP, BUTTON_HP, CHARA_SITUATION_AIR)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_MP, BUTTON_LP, CHARA_SITUATION_AIR)) {
				return true;
			}
			if (get_normal_cancel(ATTACK_KIND_MP, BUTTON_HP, CHARA_SITUATION_AIR)) {
				return true;
			}
		}
	}
	return false;
}

void Roy::roy_status_special_fireball_start() {
	if (frame >= get_param_int_special("special_fireball_transition_frame")) {
		if (check_button_input(BUTTON_LP)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_MP)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}
		if (check_button_input(BUTTON_HP)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_PUNCH);
			return;
		}	
		if (check_button_input(BUTTON_LK)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_L;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_MK)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_M;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
		if (check_button_input(BUTTON_HK)) {
			roy_int[CHARA_ROY_INT_FIREBALL_LEVEL] = SPECIAL_LEVEL_H;
			change_status(CHARA_ROY_STATUS_SPECIAL_FIREBALL_KICK);
			return;
		}
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
}

void Roy::roy_enter_status_special_fireball_start() {
	if (chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L || chara_int[CHARA_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
		change_anim("special_fireball_start", 2);
	}
	else {
		change_anim("special_fireball_start", 3);
	}
}

void Roy::roy_exit_status_special_fireball_start() {

}


void Roy::roy_status_special_fireball_punch() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable()) {
		if (common_ground_status_act()) {
			return;
		}
	}
}

void Roy::roy_enter_status_special_fireball_punch() {
	change_anim("special_fireball_punch", 2);
}

void Roy::roy_exit_status_special_fireball_punch() {

}

void Roy::roy_status_special_fireball_kick() {
	if (is_anim_end) {
		change_status(CHARA_STATUS_FALL);
		return;
	}
	if (is_actionable()) {
		if (common_ground_status_act()) {
			return;
		}
	}
}

void Roy::roy_enter_status_special_fireball_kick() {
	change_anim("special_fireball_kick", 2);
}

void Roy::roy_exit_status_special_fireball_kick() {

}

void Roy::roy_status_special_uppercut_start() {
	if (frame >= get_param_int("special_uppercut_transition_frame", param_table) && !chara_flag[CHARA_FLAG_ATTACK_BLOCKED_DURING_STATUS]) {
		change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT);
		return;
	}
	if (is_anim_end) {
		change_status(CHARA_STATUS_WAIT);
		return;
	}
	if (is_actionable() && common_ground_status_act()) {
		return;
	}
}

void Roy::roy_enter_status_special_uppercut_start() {
	change_anim("special_uppercut_start", 1);
}

void Roy::roy_exit_status_special_uppercut_start() {

}

void Roy::roy_status_special_uppercut() {
	if (anim_kind->name == "special_uppercut") {
		if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
			if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float_special("special_uppercut_fall_speed") * -1.0) {
				chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float_special("special_uppercut_gravity");
			}
			situation_kind = CHARA_SITUATION_AIR;
			add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
			if (is_anim_end) {
				change_status(CHARA_ROY_STATUS_SPECIAL_UPPERCUT_FALL);
				return;
			}
		}
	}
	else {
		if (is_anim_end) {
			change_anim("special_uppercut");
		}
		if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
			add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir, 0);
		}
	}
}

void Roy::roy_enter_status_special_uppercut() {
	if (situation_kind == CHARA_SITUATION_GROUND) { //Not sure if we want air dp to be a thing but if we do, this is designed to account for it
		change_anim("special_uppercut_rise", 2);
	}
	else {
		change_anim("special_uppercut");
	}

	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = get_param_float_special("special_uppercut_x");
	chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] = get_param_float_special("special_uppercut_init_y");
}

void Roy::roy_exit_status_special_uppercut() {

}

void Roy::roy_status_special_uppercut_fall() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(CHARA_STATUS_LANDING);
		return;
	}
	if (chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] > get_param_float_special("special_uppercut_fall_speed") * -1.0) {
		chara_float[CHARA_FLOAT_CURRENT_Y_SPEED] -= get_param_float_special("special_uppercut_gravity");
	}
	add_pos(chara_float[CHARA_FLOAT_CURRENT_X_SPEED] * facing_dir, chara_float[CHARA_FLOAT_CURRENT_Y_SPEED]);
}
void Roy::roy_enter_status_special_uppercut_fall() {
	change_anim("special_uppercut_fall");
	chara_float[CHARA_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Roy::roy_exit_status_special_uppercut_fall() {

}