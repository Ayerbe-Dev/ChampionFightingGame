#include "Roy.h"
#include "RoyFireball.h"
#include "RoyFireballConstants.h"
#include "SoundConstants.h"
#include <glm/gtx/string_cast.hpp>
#include "RenderManager.h"

void Roy::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::SET_RATE, 0.5);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-130, 0), glm::vec2(0, 50));
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(0, 0), glm::vec2(125, 80));
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-35, 35), glm::vec2(65, 160));
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2(35, 120), glm::vec2(115, 145));
		});
			
	});
	script("walk_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
		});
	});
	script("walk_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
		});
	});
	script("dash_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 120 }, glm::vec2{ 115, 145 });
		});
	});
	script("dash_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
		});
	});
	script("crouch_d", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX_ALL);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 80 });
		});
	});
	script("crouch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -38, 0 }, glm::vec2{ 43, 71 });
		});
	});
	script("crouch_u", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		});
	});
	script("jump_squat", [this]() {

	});
	script("jump", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -25, 20 }, glm::vec2{ 20, 120 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -35, 75 }, glm::vec2{ 35, 115 });
		});
	});
	script("jump_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 40 }, glm::vec2{ 35, 105 });
		});
	});
	script("jump_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 40 }, glm::vec2{ 35, 105 });
		});
	});
	script("fall", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -25, 0 }, glm::vec2{ 20, 120 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -35, 75 }, glm::vec2{ 35, 115 });
		});
	});
	script("turn_stand", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -42, 0 }, glm::vec2{ 62, 38 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -29, 39 }, glm::vec2{ 33, 141 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ 17, 82 }, glm::vec2{ 54, 104 });
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -17, 82 }, glm::vec2{ -54, 104 });
		});
	});
	script("turn_crouch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -42, 0 }, glm::vec2{ 62, 38 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -29, 39 }, glm::vec2{ 33, 141 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ 17, 82 }, glm::vec2{ 54, 104 });
		});
	});
	script("stand_block_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
		});
	});
	script("stand_block", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
		});
	});
	script("stand_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
		});
	});
	script("high_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
		});
	});
	script("crouch_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		});
	});
	script("jump_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		});
	});
	script("parry_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
		});
	});
	script("parry_start_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
		});
	});
	script("parry_air_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
		});
	});
	script("parry_mid", [this]() {

	});
	script("parry_high", [this]() {

	});
	script("parry_low", [this]() {

	});
	script("parry_air", [this]() {

	});
	script("hitstun_parry_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
		});
		execute_wait(9, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_USED_HITSTUN_PARRY, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::REENTER_LAST_ANIM);
		});
	});
	script("hitstun_parry_air_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
		});
		execute_wait(9, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_USED_HITSTUN_PARRY, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::REENTER_LAST_ANIM);
		});
	});
	script("stand_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::SET_RATE, 2.0);
			push_function(&Fighter::PLAY_VC, "roy_attack_01");
			push_function(&Fighter::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.4), glm::vec4(0.0), "HaveL", glm::vec3(0.0, 15.0, 0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 120 }, glm::vec2{ 115, 145 });
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2{ 5,145 }, glm::vec2{ 240, 165 });
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HITBOX, 0, 0, 30.0, 5.0, 1, 15.0, glm::vec2(5, 145), glm::vec2(200, 165), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_MID, 8, 6, 10, 4, 10.0, 10.0, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 1, 1, 4, CLANK_KIND_NORMAL, KO_KIND_NORMAL, false, false, 10.0, 0.0, 0.0, 1.0);
			push_function(&Fighter::SET_RATE, 1.0);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("stand_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2{ 35,115 }, glm::vec2{ 240, 135 });
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 105, 160 });
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 110 }, glm::vec2{ 155, 150 });
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 4.0, /*Damage Scale*/ 1, /*Meter Gain*/ 20.0, glm::vec2(103, 165), glm::vec2(213, 113), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 10, /*Blocklag*/12, /*Hitstun*/ 25, /*Blockstun*/ 19, /*Hit Pushback*/ 10.0, /*Block Pushback*/ 4.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 3, CLANK_KIND_NORMAL, KO_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 3);
		});
	});
	script("stand_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-105, 0), glm::vec2(35, 90));
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(30, 55), glm::vec2(95, 95));
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2{ 5,70 }, glm::vec2{ 130, 90 });
		});
		execute_frame(5, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(30, 0), glm::vec2(107, 50));
		});
		execute_frame(10, [this]() {
			//50, 16 16 12 7
		});
		execute_wait(2, [this]() {

		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("stand_lk", [this]() {

	});
	script("stand_mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::START_CINEMATIC_SEQUENCE, "super_start", 1.0, 0.0, 0.5, false, 0.0);
		});
	});
	script("stand_hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 0, 90 });
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -15, 100 }, glm::vec2{ 25, 60 });
		});
		execute_frame(6, [this]() {
\
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_frame(12, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		});
	});
	script("crouch_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -38, 0 }, glm::vec2{ 45, 71 });
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 21, 43 }, glm::vec2{ 68, 64 });
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 30.0, /*Chip Damage*/ 3.0, /*Damage Scale*/ 1, /*Meter Gain*/ 20.0, glm::vec2(51, 112), glm::vec2(199, 82), SITUATION_HIT_GROUND_AIR, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_MID, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 14, /*Blockstun*/ 12, /*Hit Pushback*/ 10.0, /*Block Pushback*/ 10.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 3, CLANK_KIND_NORMAL, KO_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("crouch_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -38, 0 }, glm::vec2{ 45, 71 });
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 21, 43 }, glm::vec2{ 68, 64 });

		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("crouch_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -45, 0 }, glm::vec2{ 45, 25 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -30, 20 }, glm::vec2{ 25, 73 });
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 40, 55 });
		});
		execute_wait(2, [this]() {

		});
		execute_wait(4, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("crouch_lk", [this]() {

	});
	script("crouch_mk", [this]() {

	});
	script("crouch_hk", [this]() {
		execute_frame(0, [this]() {

		});
		execute_frame(3, [this]() {

		});
		execute_frame(6, [this]() {

		});

	});
	script("jump_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -45, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -15, 10 }, glm::vec2{ 15, 100 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ 10, 40 }, glm::vec2{ 35, 100 });
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 5, 55 }, glm::vec2{ 40, 95 });
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -15, 10 }, glm::vec2{ 15, 75 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 75 });
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 5, 25 }, glm::vec2{ 45, 75 });

		});
	});
	script("jump_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -55, 15 }, glm::vec2{ 0, 100 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -10, 25 }, glm::vec2{ 27, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -65, 70 }, glm::vec2{ 5, 100 });
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -10, 25 }, glm::vec2{ 23, 50 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -45, 70 }, glm::vec2{ 5, 100 });
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 50 }, glm::vec2{ 40, 80 });
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 15, 45 }, glm::vec2{ 55, 70 });

		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_frame(13, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 3);
		});
	});
	script("jump_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -40, 25 }, glm::vec2{ 40, 65 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -30, 30 }, glm::vec2{ 30, 110 });
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -10, 30 }, glm::vec2{ 30, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -25, 75 }, glm::vec2{ 75, 95 });

		});
		execute_wait(5, [this]() {
			push_function(&Fighter::CLEAR_HITBOX, 1);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
	});
	script("jump_lk", [this]() {

	});
	script("jump_mk", [this]() {

	});
	script("jump_hk", [this]() {

	});
	script("advance_high", [this]() {
		execute_frame(20, [this]() {
			push_function(&Fighter::RESET_WORLD_RATE);
		});
	});
	script("advance_low", [this]() {
		execute_frame(20, [this]() {
			push_function(&Fighter::RESET_WORLD_RATE);
		});
	});
	script("advance_forward", [this]() {
	
	});
	script("advance_back", [this]() {

	});
	script("grab", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_GRABBOX, 0, glm::vec2{ 15, 55 }, glm::vec2{ 70, 100 }, GRABBOX_KIND_NORMAL, SITUATION_HIT_GROUND_AIR, FIGHTER_STATUS_THROW, FIGHTER_STATUS_GRABBED);
		});
	});
	script("throw_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec2(0.0), 3);
		});
		execute_frame(24, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 30.0, 15.0, 8.0);
		});
	});
	script("throw_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec2(0.0), 3);
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
		});
		execute_frame(40, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 30.0, -35.0, 8.0);
			push_function(&Fighter::CHANGE_OPPONENT_STATUS, FIGHTER_STATUS_THROWN);
		});
	});
	script("grab_air", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_GRABBOX, 0, glm::vec2{ 15, 55 }, glm::vec2{ 70, 100 }, GRABBOX_KIND_HITSTUN, SITUATION_HIT_GROUND_AIR, FIGHTER_STATUS_THROW_AIR, FIGHTER_STATUS_GRABBED);
		});
	});
	script("throw_f_air", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec2(0.0), 3);
		});
		execute_frame(22, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 30.0, 15.0, 8.0);
		});
	});
	script("throw_b_air", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec2(0.0), 3);
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
		});
		execute_frame(10, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 30.0, -15.0, 8.0);
			push_function(&Fighter::CHANGE_OPPONENT_STATUS, FIGHTER_STATUS_THROWN);
		});
	});
	script("special_fireball_start", [this]() {
		execute_frame(0, [this]() {

		});
		if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
			execute_frame(2, [this]() {
				push_function(&Fighter::SET_RATE, 1.2);
			});
		}
		else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
			execute_frame(4, [this]() {
				push_function(&Fighter::SET_RATE, 1.2);
			});
		}
		else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
			execute_frame(6, [this]() {
				push_function(&Fighter::SET_RATE, 1.2);
			});
		}
		else {
			execute_frame(2, [this]() {
				push_function(&Fighter::SET_RATE, 1.2);
			});
		}
		execute_frame(8, [this]() {
			push_function(&Fighter::ACTIVATE_PROJECTILE, 0, glm::vec3(0.0));
		});
	});
	script("special_fireball_punch", [this]() {
		execute_frame(0, [this]() {

		});
		execute_frame(5, [this]() {
			push_function(&Fighter::CHANGE_PROJECTILE_STATUS, 0, PROJECTILE_ROY_FIREBALL_STATUS_PUNCHED);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ADD_PROJECTILE_POS, 0, 50.0 * facing_dir, 0.0);
		});
	});
	script("special_fireball_kick", [this]() {
		execute_frame(0, [this]() {

		});
		execute_frame(9, [this]() {
			push_function(&Fighter::CHANGE_PROJECTILE_STATUS, 0, PROJECTILE_ROY_FIREBALL_STATUS_KICKED);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ADD_PROJECTILE_POS, 0, 30.0 * facing_dir, -40.0);
		});
	});
	script("special_slide", [this]() {
		execute_frame(0, [this]() {
			set_jostle_offset(120.0);
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Fighter::SET_RATE, 1.5);
			}
		});
	});
	script("special_slide_followup", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("special_uppercut_start", [this]() {
		execute_frame(0, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -82, 8 }, glm::vec2{ 34, 41 });
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -65, 36 }, glm::vec2{ 17, 95 });
			}
		});
		execute_frame(4, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -82, 8 }, glm::vec2{ 50, 41 });
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -18, 37 }, glm::vec2{ 46, 132 });

			}
			else {

			}
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("special_uppercut", [this]() {
		execute_frame(4, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {

			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {

			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {

			}
			else {

			}
		});
		execute_frame(5, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {

			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {

			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {

			}
			else {

			}
		});
	});
	script("special_uppercut_fall", [this]() {

	});
	script("stand_hitstun_l", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 120 }, glm::vec2{ 115, 145 });
		});
	});
	script("stand_hitstun_m", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 120 }, glm::vec2{ 115, 145 });
		});
	});
	script("stand_hitstun_h", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 });
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 120 }, glm::vec2{ 115, 145 });
		});
	});
	script("crouch_hitstun_l", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		});
	});
	script("crouch_hitstun_m", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		});
	});
	script("crouch_hitstun_h", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		});
	});
	script("jump_hitstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		});
	});
	script("launch_start", [this]() {

	});
	script("launch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 70 });
		});
	});
	script("knockdown_start", [this]() {

	});
	script("knockdown_wait", [this]() {

	});
	script("wakeup", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crumple", [this]() {

	});
	script("landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		});
	});
	script("landing_hitstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -35, 0 }, glm::vec2{ 37, 35 });
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -25, 0 }, glm::vec2{ 20, 110 });
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -15, 55 }, glm::vec2{ 35, 95 });
		});
	});
}
