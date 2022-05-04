#include "Roy.h"
#include "RoyFireball.h"
#include "RoyFireballConstants.h"
#include "SoundConstants.h"
#include <glm/gtx/string_cast.hpp>

void Roy::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -42, 0 }, glm::vec2{ 62, 38 });
			new_hurtbox(1, glm::vec2{ -29, 39 }, glm::vec2{ 33, 141 });
			new_hurtbox(2, glm::vec2{ 17, 82 }, glm::vec2{ 54, 104 });
		}
		if (is_excute_frame(10)) { //todo: Delete this
			frame++;
		}
	});
	script("walk_f", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("walk_b", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("dash_f", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("dash_b", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("crouch_d", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
		if (is_excute_frame(2)) {
			clear_hurtbox_all();
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 80 });
		}
	});
	script("crouch", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -38, 0 }, glm::vec2{ 43, 71 });
		}
	});
	script("crouch_u", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("jump_squat", [this]() {

	});
	script("jump", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -25, 20 }, glm::vec2{ 20, 120 });
			new_hurtbox(1, glm::vec2{ -35, 75 }, glm::vec2{ 35, 115 });
		}
	});
	script("jump_f", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 40 }, glm::vec2{ 35, 105 });
		}
	});
	script("jump_b", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 40 }, glm::vec2{ 35, 105 });
		}
	});
	script("fall", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -25, 0 }, glm::vec2{ 20, 120 });
			new_hurtbox(1, glm::vec2{ -35, 75 }, glm::vec2{ 35, 115 });
		}
	});
	script("turn_stand", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -42, 0 }, glm::vec2{ 62, 38 });
			new_hurtbox(1, glm::vec2{ -29, 39 }, glm::vec2{ 33, 141 });
			new_hurtbox(2, glm::vec2{ 17, 82 }, glm::vec2{ 54, 104 });
		}
		if (is_excute_frame(4)) {
			clear_hurtbox(2);
		}
		if (is_excute_frame(7)) {
			new_hurtbox(2, glm::vec2{ -17, 82 }, glm::vec2{ -54, 104 });
		}
	});
	script("turn_crouch", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -42, 0 }, glm::vec2{ 62, 38 });
			new_hurtbox(1, glm::vec2{ -29, 39 }, glm::vec2{ 33, 141 });
			new_hurtbox(2, glm::vec2{ 17, 82 }, glm::vec2{ 54, 104 });
		}
	});
	script("stand_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("high_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("crouch_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		}
	});
	script("jump_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("parry_start", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
		if (is_excute_frame(1)) {
			fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = true;
		}
		if (is_excute_wait(4)) {
			fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = false;
			fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
		}
	});
	script("parry_start_air", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
		if (is_excute_frame(1)) {
			fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = true;
		}
		if (is_excute_wait(4)) {
			fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = false;
			fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
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
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
			fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = true;
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
		}
		if (is_excute_wait(2)) {
			fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = false;
			fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
		}
		if (is_excute_wait(9)) {
			reenter_last_anim();
			fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
		}
	});
	script("hitstun_parry_air", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
			fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = true;
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
		}
		if (is_excute_wait(2)) {
			fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = false;
			fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
		}
		if (is_excute_wait(9)) {
			reenter_last_anim();
			fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
		}
	});
	script("stand_lp", [this]() {
		if (is_excute_frame(0)) {
			rate = 2.0;
			playVC(ROY_VC_ATTACK_01);
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
			new_hitbox(0, 0, 30, 5, 1.2, 1, glm::vec2{ 5,70 }, glm::vec2{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 10, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 25, 25, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 0.0, 0.0, 0.0, 0.0, false, false);
		}
		if (is_excute_frame(4)) {
			new_hitbox(1, 0, 30, 5, 1.2, 1, glm::vec2{ 35,70 }, glm::vec2{ 90, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 10, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 25, 25, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false, false);
			rate = 1.0;
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
		}
	});
	script("stand_mp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
			new_hitbox(0, 0, 40, 5, 1.2, 3, glm::vec2{ 5,70 }, glm::vec2{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 12, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 0.0, 0.0, 0.0, 0.0, false, false);
		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 0, 40, 5, 1.2, 3, glm::vec2{ 35,70 }, glm::vec2{ 90, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 12, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false, false);
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
		}
	});
	script("stand_hp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
			new_hitbox(0, 0, 50, 5, 1.2, 1, glm::vec2{ 5,70 }, glm::vec2{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 0.0, 0.0, 0.0, 0.0, true, false);
		}
		if (is_excute_frame(10)) {
			new_hitbox(1, 0, 50, 5, 1.2, 1, glm::vec2{ 25,70 }, glm::vec2{ 90, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, true, false);
		}
		if (is_excute_wait(2)) {
			new_hitbox(2, 0, 60, 10, 1.2, 1, glm::vec2{ 80,70 }, glm::vec2{ 100, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 20, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_CRUMPLE, HIT_STATUS_CRUMPLE, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, true, false);
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
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 0, 90 });
		}
		if (is_excute_frame(2)) {
			new_hurtbox(1, glm::vec2{ -15, 100 }, glm::vec2{ 25, 60 });
		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 0, 50, 10, 1.2, 1, glm::vec2{ 25,60 }, glm::vec2{ 60, 120 }, HITBOX_KIND_NORMAL, 20, 40, 10, SITUATION_HIT_GROUND_AIR, 4, 15, 12, 70, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 3, 3, CLANK_KIND_NORMAL, 1, 20, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_AERIAL, 20.0, 0.0, 2.0, 1.0, true, false);
		}
		if (is_excute_frame(8)) {
			clear_hitbox_all();
		}
		if (is_excute_frame(12)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("crouch_lp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -38, 0 }, glm::vec2{ 45, 71 });
		}
		if (is_excute_frame(4)) {
			new_hurtbox(1, glm::vec2{ 21, 43 }, glm::vec2{ 68, 64 });
			new_hitbox(1, 0, 30, 5, 1.2, 1, glm::vec2{ 42,43 }, glm::vec2{ 76, 64 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false, false);
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
			new_hurtbox(0, glm::vec2{ -38, 0 }, glm::vec2{ 45, 71 });
		}
		if (is_excute_frame(4)) {
			new_hurtbox(1, glm::vec2{ 21, 43 }, glm::vec2{ 68, 64 });
			new_hitbox(1, 0, 40, 5, 1.2, 3, glm::vec2{ 42,43 }, glm::vec2{ 76, 64 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false, false);
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
			new_hurtbox(0, glm::vec2{ -45, 0 }, glm::vec2{ 45, 25 });
			new_hurtbox(1, glm::vec2{ -30, 20 }, glm::vec2{ 25, 73 });
		}
		if (is_excute_frame(6)) {
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 40, 55 });
		}
		if (is_excute_wait(2)) {
			new_hitbox(1, 0, 50, 5, 1.2, 1, glm::vec2{ 25,55 }, glm::vec2{ -10, 140 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 14.0, 1.0, 14.0, 1.0, true, false);
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

		}
		if (is_excute_frame(3)) {

		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 0, 50, 5, 1.2, 1, glm::vec2{ -5,35 }, glm::vec2{ 90, 0 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, ATTACK_HEIGHT_LOW, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 1, 4, HIT_STATUS_KNOCKDOWN, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, true, false);
		}

	});
	script("jump_lp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -45, 0 }, glm::vec2{ 0, 50 });
			new_hurtbox(1, glm::vec2{ -15, 10 }, glm::vec2{ 15, 100 });
			new_hurtbox(2, glm::vec2{ 10, 40 }, glm::vec2{ 35, 100 });
		}
		if (is_excute_frame(2)) {
			new_hurtbox(3, glm::vec2{ 5, 55 }, glm::vec2{ 40, 95 });
		}
		if (is_excute_wait(1)) {
			new_hurtbox(1, glm::vec2{ -15, 10 }, glm::vec2{ 15, 75 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 75 });
			new_hurtbox(3, glm::vec2{ 5, 25 }, glm::vec2{ 45, 75 });
			if (fighter_flag[FIGHTER_FLAG_SELF_CANCEL]) {
				new_hitbox(1, 0, 30, 5, 1.2, 1, glm::vec2{ 20,20 }, glm::vec2{ 40, 60 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 8, 8, 6, 4, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_LIGHT, 50, 50, CLANK_KIND_NORMAL, 1, 3, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 4.0, false, false);
			}
			else {
				new_hitbox(1, 0, 30, 5, 1.2, 1, glm::vec2{ 20,20 }, glm::vec2{ 50, 60 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 8, 8, 6, 4, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_LIGHT, 30, 30, CLANK_KIND_NORMAL, 1, 3, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 4.0, false, false);
			}
			new_hitbox(2, 0, 30, 5, 1.2, 1, glm::vec2{ 20,20 }, glm::vec2{ 50, 60 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_AIR, 8, 8, 6, 4, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, 1, 3, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 4.0, false, false);
		}
	});
	script("jump_mp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -55, 15 }, glm::vec2{ 0, 100 });
			new_hurtbox(1, glm::vec2{ -10, 25 }, glm::vec2{ 27, 80 });
			new_hurtbox(2, glm::vec2{ -65, 70 }, glm::vec2{ 5, 100 });
		}
		if (is_excute_wait(4)) {
			new_hurtbox(1, glm::vec2{ -10, 25 }, glm::vec2{ 23, 50 });
			new_hurtbox(2, glm::vec2{ -45, 70 }, glm::vec2{ 5, 100 });
		}
		if (is_excute_wait(2)) {
			new_hurtbox(1, glm::vec2{ 0, 50 }, glm::vec2{ 40, 80 });
			new_hurtbox(3, glm::vec2{ 15, 45 }, glm::vec2{ 55, 70 });
			new_hitbox(1, 0, 30, 15, 1.2, 1, glm::vec2{ 45,35 }, glm::vec2{ 75, 55 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 8, 8, 6, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, 3, 5, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 4.0, false, false);
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
			new_hurtbox(0, glm::vec2{ -40, 25 }, glm::vec2{ 40, 65 });
			new_hurtbox(1, glm::vec2{ -30, 30 }, glm::vec2{ 30, 110 });
		}
		if (is_excute_frame(6)) {
			new_hurtbox(1, glm::vec2{ -10, 30 }, glm::vec2{ 30, 110 });
			new_hurtbox(2, glm::vec2{ -25, 75 }, glm::vec2{ 75, 95 });
			new_hitbox(1, 0, 60, 15, 1.2, 1, glm::vec2{ 60,75 }, glm::vec2{ 95, 95 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 10, 8, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 1, 7, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 10.0, false, false);
			new_hitbox(2, 0, 50, 15, 1.2, 1, glm::vec2{ 20,75 }, glm::vec2{ 95, 95 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 10, 8, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 1, 7, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 10.0, false, false);
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
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
		if (is_excute_frame(3)) {
			new_grabbox(0, glm::vec2{ 15, 55 }, glm::vec2{ 70, 100 }, GRABBOX_KIND_NORMAL, SITUATION_HIT_GROUND_AIR, FIGHTER_STATUS_THROW, FIGHTER_STATUS_GRABBED);
		}
	});
	script("throw_f", [this]() {
		if (is_excute_frame(0)) {
			change_opponent_anim("stand_hitstun_m", 2);
			grab_opponent("Throw", "Bust", glm::vec2(0.0), 3);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(22)) {
			throw_opponent(30.0, 15.0, 8.0);
		}
	});
	script("throw_b", [this]() {
		if (is_excute_frame(0)) {
			grab_opponent("HandL", "Bust", glm::vec2(0.0), 3);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(10)) {
			throw_opponent(30.0, -15.0, 8.0);
			change_opponent_status(FIGHTER_STATUS_THROWN);
		}
	});
	script("grab_air", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
		if (is_excute_frame(3)) {
			new_grabbox(0, glm::vec2{ 15, 55 }, glm::vec2{ 70, 100 }, GRABBOX_KIND_HITSTUN, SITUATION_HIT_GROUND_AIR, FIGHTER_STATUS_THROW_AIR, FIGHTER_STATUS_GRABBED);
		}
	});
	script("throw_f_air", [this]() {
		if (is_excute_frame(0)) {
			change_opponent_anim("stand_hitstun_m", 2);
			grab_opponent("HandL", "Bust", glm::vec2(0.0), 3);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(22)) {
			throw_opponent(30.0, 15.0, 8.0);
		}
	});
	script("throw_b_air", [this]() {
		if (is_excute_frame(0)) {
			grab_opponent("HandL", "Bust", glm::vec2(0.0), 3);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(10)) {
			throw_opponent(30.0, -15.0, 8.0);
			change_opponent_status(FIGHTER_STATUS_THROWN);
		}
	});
	script("stand_hitstun_l", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("stand_hitstun_m", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("stand_hitstun_h", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("crouch_hitstun_l", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		}
	});
	script("crouch_hitstun_m", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		}
	});
	script("crouch_hitstun_h", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		}
	});
	script("jump_hitstun", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		}
	});
	script("launch_start", [this]() {

	});
	script("launch", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		}
	});
	script("knockdown_start", [this]() {

	});
	script("knockdown_wait", [this]() {

	});
	script("wakeup", [this]() {
		if (is_excute_frame(0)) {
		}
	});
	script("crumple", [this]() {

	});
	script("landing", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("landing_hitstun", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			new_hurtbox(1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			new_hurtbox(2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		}
	});
	script("special_fireball_start", [this]() {
		if (is_excute_frame(0)) {

		}
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
			if (is_excute_frame(3)) {
				rate = 1.2;
			}
		}
		else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
			if (is_excute_frame(5)) {
				rate = 1.2;
			}
		}
		else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
			if (is_excute_frame(7)) {
				rate = 1.2;
			}
		}
		else {
			if (is_excute_frame(3)) {
				rate = 1.2;
			}
		}
		if (is_excute_frame(11)) {
			init_projectile(0, glm::vec3(70, 100, 0));
		}
	});
	script("special_fireball_punch", [this]() {
		if (is_excute_frame(0)) {

		}
		if (is_excute_frame(5)) {
			projectiles[0]->change_status(PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED);
		}
	});
	script("special_fireball_kick", [this]() {
		if (is_excute_frame(0)) {

		}
		if (is_excute_frame(20)) {
			projectiles[0]->change_status(PROJECTILE_ROY_FIREBALL_STATUS_KICKED);
		}
	});
	script("special_uppercut_start", [this]() {
		if (is_excute_frame(0)) {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hurtbox(0, glm::vec2{ -82, 8 }, glm::vec2{ 34, 41 });
				new_hurtbox(1, glm::vec2{ -65, 36 }, glm::vec2{ 17, 95 });
			}
		}
		if (is_excute_frame(4)) {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hurtbox(0, glm::vec2{ -82, 8 }, glm::vec2{ 50, 41 });
				new_hurtbox(1, glm::vec2{ -18, 37 }, glm::vec2{ 46, 132 });
				new_hitbox(1, 0, 60, 5, 1.2, 1, glm::vec2{ 34, 77 }, glm::vec2{ 54, 103 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 20, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, -30, 4, CLANK_KIND_NORMAL, 1, 1, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 3.0, 0.1, 0.1, 0.0, false, true);
				new_hitbox(2, 0, 60, 5, 1.2, 1, glm::vec2{ 51, 77 }, glm::vec2{ 75, 103 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 20, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, -90, 4, CLANK_KIND_NORMAL, 1, 1, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 3.0, 0.1, 0.1, 0.0, false, true);
			}
			else {
				new_hitbox(1, 0, 60, 5, 1.2, 1, glm::vec2{ 34, 77 }, glm::vec2{ 54, 103 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 20, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, -30, 4, CLANK_KIND_NORMAL, 1, 6, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 3.0, 0.1, 0.1, 0.0, false, true);
				new_hitbox(2, 0, 60, 5, 1.2, 1, glm::vec2{ 51, 77 }, glm::vec2{ 75, 103 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 20, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, -90, 4, CLANK_KIND_NORMAL, 1, 6, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 3.0, 0.1, 0.1, 0.0, false, true);
			}
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
		}
	});
	script("special_uppercut", [this]() {
		if (is_excute_frame(4)) {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(1, 0, 30, 5, 1.2, 1, glm::vec2{ 0,5 }, glm::vec2{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 6, 1, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 20.0, 1.0, 12.0, 3.0, false, true);
			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				new_hitbox(1, 0, 40, 5, 1.2, 1, glm::vec2{ 0,5 }, glm::vec2{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 6, 1, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 25.0, 1.0, 12.0, 3.0, false, true);
			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				new_hitbox(1, 0, 50, 5, 1.2, 1, glm::vec2{ 0,5 }, glm::vec2{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 6, 1, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 30.0, 1.0, 12.0, 3.0, false, true);
			}
			else {
				new_hitbox(1, 0, 70, 5, 1.2, 1, glm::vec2{ 0,5 }, glm::vec2{ 50, 75 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 8, 1, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 30.0, 1.0, 12.0, 3.0, false, true);
			}
		}
		if (is_excute_frame(5)) {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				new_hitbox(1, 0, 30, 5, 1.2, 1, glm::vec2{ 0,65 }, glm::vec2{ 40, 150 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 6, 5, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 20.0, 1.0, 12.0, 3.0, false, true);
			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				new_hitbox(1, 0, 40, 5, 1.2, 1, glm::vec2{ 0,85 }, glm::vec2{ 40, 150 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 6, 5, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 25.0, 1.0, 12.0, 3.0, false, true);
			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				new_hitbox(1, 0, 50, 5, 1.2, 1, glm::vec2{ 0,85 }, glm::vec2{ 40, 150 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 6, 5, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 30.0, 1.0, 12.0, 3.0, false, true);
			}
			else {
				new_hitbox(1, 0, 70, 5, 1.2, 1, glm::vec2{ 0,65 }, glm::vec2{ 40, 150 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 18, 6, 14, 10, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, 8, 7, HIT_STATUS_LAUNCH, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_NORMAL, 30.0, 1.0, 12.0, 3.0, false, true);
			}
		}
	});
}