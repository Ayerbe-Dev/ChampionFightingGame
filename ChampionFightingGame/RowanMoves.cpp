#include "Rowan.h"
#include "RowanFireball.h"
#include "RowanFireballConstants.h"
#include "SoundConstants.h"

void Rowan::load_move_list() {
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("stand_lp", FIGHTER_STATUS_ATTACK,
		BUFFER_LP, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("stand_mp", FIGHTER_STATUS_ATTACK,
		BUFFER_MP, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("stand_hp", FIGHTER_STATUS_ATTACK,
		BUFFER_HP, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("stand_lk", FIGHTER_STATUS_ATTACK,
		BUFFER_LK, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("stand_mk", FIGHTER_STATUS_ATTACK,
		BUFFER_MK, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("stand_hk", FIGHTER_STATUS_ATTACK,
		BUFFER_HK, 1, MOVESET_DIR_NEUTRAL, false
	);

	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("forward_hp", FIGHTER_STATUS_ATTACK,
		BUFFER_HP, 1, MOVESET_DIR_FORWARD, false
	);

	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("crouch_lp", FIGHTER_STATUS_ATTACK,
		BUFFER_LP, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("crouch_mp", FIGHTER_STATUS_ATTACK,
		BUFFER_MP, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("crouch_hp", FIGHTER_STATUS_ATTACK,
		BUFFER_HP, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("crouch_lk", FIGHTER_STATUS_ATTACK,
		BUFFER_LK, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("crouch_mk", FIGHTER_STATUS_ATTACK,
		BUFFER_MK, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("crouch_hk", FIGHTER_STATUS_ATTACK,
		BUFFER_HK, 1, MOVESET_DIR_DOWN, true
	);

	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("jump_squat", FIGHTER_STATUS_JUMPSQUAT,
		0, 0, MOVESET_DIR_UP, false
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("dash_f", FIGHTER_STATUS_DASH_F,
		INPUT_KIND_66, 0, 0, 0, SPECIAL_LEVEL_SETTING_N, false
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("dash_b", FIGHTER_STATUS_DASH_B,
		INPUT_KIND_44, 0, 0, 0, SPECIAL_LEVEL_SETTING_N, false
	);

	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("backdash_attack",
		FIGHTER_STATUS_BACKDASH_ATTACK, BUFFER_6B, 1, MOVESET_DIR_FORWARD, false, VBP(),
		0.0f, 0.0f, false, { "dash_b" }
	);

	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("special_uppercut",
		CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, INPUT_KIND_623, BUFFER_3P, 1, 0,
		SPECIAL_LEVEL_SETTING_L_M_H_EX, false, VBP(), 200.0f, 200.0f
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("special_fireball", 
		CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, INPUT_KIND_236, BUFFER_3P, 1, 0, 
		SPECIAL_LEVEL_SETTING_L_M_H_EX, false, VBP(fighter_flag[CHARA_ROWAN_FLAG_FIREBALL_ACTIVE]),
		200.0f, 200.0f
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("special_slide",
		CHARA_ROWAN_STATUS_SPECIAL_SLIDE, INPUT_KIND_214, BUFFER_3K, 1, 0,
		SPECIAL_LEVEL_SETTING_L_M_H_EX, false, VBP(), 200.0f, 200.0f
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("special_install",
		CHARA_ROWAN_STATUS_SPECIAL_INSTALL, INPUT_KIND_22, BUFFER_3P, 1, 0,
		SPECIAL_LEVEL_SETTING_N, false
	);



	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("grab_start", FIGHTER_STATUS_GRAB_START,
		BUFFER_2L, 2, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("parry_start", FIGHTER_STATUS_PARRY_START,
		BUFFER_2M, 2, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("advance", FIGHTER_STATUS_ADVANCE,
		BUFFER_2H, 2, MOVESET_DIR_NEUTRAL, false, VBP(), 100.0f, 300.0f
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("advance_forward", FIGHTER_STATUS_ADVANCE_FORWARD,
		BUFFER_2H, 2, MOVESET_DIR_FORWARD, false, VBP(), 100.0f, 300.0f
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("advance_back", FIGHTER_STATUS_ADVANCE_BACK,
		BUFFER_2H, 2, MOVESET_DIR_BACK, false, VBP(), 100.0f, 300.0f
	);
	move_list[FIGHTER_SITUATION_GROUND].add_movelist_entry("taunt", FIGHTER_STATUS_TAUNT,
		BUFFER_6B, 6, MOVESET_DIR_NEUTRAL, false
	);

	move_list[FIGHTER_SITUATION_AIR].add_movelist_entry("jump_lp", FIGHTER_STATUS_ATTACK_AIR,
		BUFFER_LP, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_AIR].add_movelist_entry("jump_mp", FIGHTER_STATUS_ATTACK_AIR,
		BUFFER_MP, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_AIR].add_movelist_entry("jump_hp", FIGHTER_STATUS_ATTACK_AIR,
		BUFFER_HP, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_AIR].add_movelist_entry("jump_lk", FIGHTER_STATUS_ATTACK_AIR,
		BUFFER_LK, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_AIR].add_movelist_entry("jump_mk", FIGHTER_STATUS_ATTACK_AIR,
		BUFFER_MK, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_AIR].add_movelist_entry("jump_hk", FIGHTER_STATUS_ATTACK_AIR,
		BUFFER_HK, 1, MOVESET_DIR_NEUTRAL, false
	);

	move_list[FIGHTER_SITUATION_AIR].add_movelist_entry("grab_start", FIGHTER_STATUS_GRAB_AIR_START,
		BUFFER_2L, 2, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_SITUATION_AIR].add_movelist_entry("parry_start", FIGHTER_STATUS_PARRY_START,
		BUFFER_2M, 2, MOVESET_DIR_NEUTRAL, false
	);
}

void Rowan::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::SET_RATE, 0.5);
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
			
	});
	script("walk_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("walk_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("dash_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("dash_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			if (fighter_flag[FIGHTER_FLAG_DASH_CANCEL]) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			}
			else {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_SOFT);
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_SOFT);
			}
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::SET_HURTBOX_INTANGIBLE_KIND, 0, INTANGIBLE_KIND_THROW);
			push_function(&Fighter::SET_HURTBOX_INTANGIBLE_KIND, 1, INTANGIBLE_KIND_THROW);
		});
		execute_frame(14, [this]() {
			push_function(&Fighter::SET_HURTBOX_INTANGIBLE_KIND, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_HURTBOX_INTANGIBLE_KIND, 1, INTANGIBLE_KIND_NONE);
		});
	});
	script("dash_air_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(196, 107), glm::vec2(61, 28), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(142, 37), glm::vec2(13, -14), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(16, 39), glm::vec2(186, 85), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(92, 85), glm::vec2(317, 138), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("dash_air_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(118, -16), glm::vec2(-100, 47), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(60, 41), glm::vec2(-81, 152), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-171, 47), glm::vec2(-88, 171), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-100, 22), glm::vec2(-6, 67), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(23, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-153, 53), glm::vec2(71, 118), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-119, 118), glm::vec2(31, 190), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_d", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX_ALL);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_u", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_squat", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
	});
	script("jump", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-76, 115), glm::vec2(123, 182), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-90, 49), glm::vec2(106, 161), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-98, 17), glm::vec2(8, 62), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-90, 49), glm::vec2(106, 161), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("fall", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-76, 115), glm::vec2(123, 182), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("turn_stand", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(30.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(30.0, 160.0));
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
	});
	script("turn_crouch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -42, 0 }, glm::vec2{ 62, 38 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -29, 39 }, glm::vec2{ 33, 141 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ 17, 82 }, glm::vec2{ 54, 104 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_block_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_block", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("high_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("parry_start_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			});
		});
	script("parry_start_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("parry_start_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 110 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("parry_air_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
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
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
		});
	});
	script("hitstun_parry_air_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
		});
	});
	script("launch_parry_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 110 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_RATE, (anim_kind->length - 2.0) / calc_launch_frames());
		});
	});
	script("stand_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::PLAY_RESERVED_SOUND, "rowan_attack_01");
			push_function(&Fighter::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.4), glm::vec4(0.0), "HaveL", glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES]);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 120 }, glm::vec2{ 115, 145 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2{ 5,145 }, glm::vec2{ 240, 165 });
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 4, glm::vec2(151, 164), glm::vec2(1, 132), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(5, 145), glm::vec2(200, 165), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_MID, /*Damage*/ 40.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 2, /*Meter Gain*/ 24.0, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 12, /*Blockstun*/ 9, /*Hit Pushback Ground*/ 8.0, /*Block Pushback Ground*/ 8.0, /*X Pushback Air*/ 3.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 7, DAMAGE_KIND_NORMAL, HIT_LEVEL_LIGHT, "", "");
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "stand_lp", CANCEL_KIND_ANY);
			push_function(&Fighter::ENABLE_CANCEL, "stand_mp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "crouch_hp", CANCEL_KIND_CONTACT);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::DISABLE_CANCEL, "stand_mp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "crouch_hp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 4);
		});
	});
	script("stand_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2{ 35,115 }, glm::vec2{ 240, 135 });
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 105, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 110 }, glm::vec2{ 155, 150 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(103, 165), glm::vec2(213, 113), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, 0, 1, 2, HIT_HEIGHT_MID, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 18, /*Blockstun*/ 11, /*Hit Pushback Ground*/ 10.0, /*Block Pushback Ground*/ 4.0, /*X Pushback Air*/ 3.0, /*Y Pushback Air*/ 28.0, /*Pushback Frames*/ 9, DAMAGE_KIND_NORMAL, HIT_LEVEL_MEDIUM, "", "");
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 3);
			push_function(&Fighter::ENABLE_CANCEL, "dash_b", CANCEL_KIND_HIT);
			push_function(&Fighter::ENABLE_CANCEL, "stand_hk", CANCEL_KIND_CONTACT);
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "stand_hp", CANCEL_KIND_CONTACT);
		});
	});
	script("stand_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-105, 0), glm::vec2(35, 90), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(30, 55), glm::vec2(95, 95), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(217, 168), glm::vec2(12, 128));
		});
		execute_frame(5, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(30, 0), glm::vec2(107, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(85, 167), glm::vec2(0, 108), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(180, 168), glm::vec2(12, 128), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_MID, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 23, /*Blockstun*/ 23, /*Hit Pushback Ground*/ 14.0, /*Block Pushback Ground*/ 8.0, /*X Pushback Air*/ 5.0, /*Y Pushback Air*/ 31.0, /*Pushback Frames*/ 11, /*Launch Init Y*/ 20.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 25.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
	});
	script("stand_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(45, 185), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-100, 0), glm::vec2(120, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-100, 35), glm::vec2(100, 80), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-100, 35), glm::vec2(100, 80), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-70, 70), glm::vec2(25, 185), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-35, 0), glm::vec2(220, 60), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-45, 40), glm::vec2(65, 155), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2(30, 45), glm::vec2(160, 95), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 4, glm::vec2(20, 70), glm::vec2(130, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(250, 57), glm::vec2(13, 4), COLLISION_KIND_GROUND, HIT_STATUS_NORMAL, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 13, /*Blockstun*/ 8, /*Hit Pushback Ground*/ 10.0, /*Block Pushback Ground*/ 10.0, /*Pushback Frames*/ 7, DAMAGE_KIND_NORMAL, HIT_LEVEL_LIGHT, "", "");
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::CLEAR_HURTBOX, 2);
			push_function(&Fighter::CLEAR_HURTBOX, 3);
			push_function(&Fighter::CLEAR_HURTBOX, 4);
		});
	});
	script("stand_mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-10, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(25, 60), glm::vec2(150, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(25, 60), glm::vec2(180, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(25, 105), glm::vec2(260, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(30, 105), glm::vec2(310, 155), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 2, HIT_HEIGHT_MID, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 19, /*Blockstun*/ 10, /*Hit Pushback Ground*/ 10.0, /*Block Pushback Ground*/ 4.0, /*X Pushback Air*/ 3.0, /*Y Pushback Air*/ 28.0, /*Pushback Frames*/ 10, /*Launch Init Y*/ 10.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch X Speed*/ 1.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_MEDIUM, "", "");
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
	});
	script("stand_hk", [this]() {
		execute_frame(0.0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-75, -3), glm::vec2(100, 149), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7.0, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(52, 82), glm::vec2(163, 137), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, HIT_FLAG_DISABLE_HITSTUN_PARRY | HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 4, /*Juggle Increase*/ 2, /*Juggle Max*/ 2, HIT_HEIGHT_MID, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 36.0, /*Hitlag*/ 6, /*Blocklag*/ 6, /*Hitstun*/ 20, /*Blockstun*/ 9, /*Hit Pushback Ground*/ 13.0, /*Block Pushback Ground*/ 6.0, /*X Pushback Air*/ 2.0, /*Y Pushback Air*/ 31.0, /*Pushback Frames*/ 5, /*Launch Init Y*/ 25.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch Speed X*/ 3.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(44, 76), glm::vec2(173, 143), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(11.0, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(12.0, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 1, glm::vec2(60, 95), glm::vec2(290, 180), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 3, HIT_HEIGHT_MID, /*Damage*/ 20.0, /*Chip Damage*/ 4.0, /*Damage Scale*/ 1, /*Meter Gain*/ 16.0, /*Hitlag*/ 6, /*Blocklag*/ 6, /*Hitstun*/ 24, /*Blockstun*/ 5, /*Hit Pushback Ground*/ 20.0, /*Block Pushback Ground*/ 10.0, /*X Pushback Air*/ 3.0, /*Y Pushback Air*/ 31.0, /*Pushback Frames*/ 10, /*Launch Init Y*/ 50.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch Speed X*/ 1.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(45, 75), glm::vec2(305, 190), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::CLEAR_HITBOX, 0);
		});
		execute_frame(14.0, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "dash_f", CANCEL_KIND_CONTACT);
		});
		execute_frame(15.0, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX, 1);
		});
		execute_frame(17.0, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("crouch_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(43, 108), glm::vec2(136, 79), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(51, 112), glm::vec2(199, 82), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 0, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, /*Hitlag*/ 6, /*Blocklag*/ 6, /*Hitstun*/ 13, /*Blockstun*/ 5, /*Hit Pushback Ground*/ 9.0, /*Block Pushback Ground*/ 11.0, /*X Pushback Air*/ 4.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 8, DAMAGE_KIND_NORMAL, HIT_LEVEL_LIGHT, "", "");
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "crouch_lp", CANCEL_KIND_ANY);
			push_function(&Fighter::ENABLE_CANCEL, "crouch_lk", CANCEL_KIND_ANY);
		});
	});
	script("crouch_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-100, 0), glm::vec2(165, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(75, 20), glm::vec2(-115, 100), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(125, 20), glm::vec2(-75, 100), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(260, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 20), glm::vec2(215, 90), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(175, 35), glm::vec2(270, 120), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(145, 40), glm::vec2(320, 120), COLLISION_KIND_GROUND, HIT_STATUS_NORMAL, HIT_HEIGHT_MID, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 22, /*Blockstun*/ 17, /*Hit Pushback Ground*/ 8.0, /*Block Pushback Ground*/ 8.0, /*Pushback Frames*/ 11, DAMAGE_KIND_NORMAL, HIT_LEVEL_MEDIUM, "", "");
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(255, 120), glm::vec2(355, 165), COLLISION_KIND_AIR, HIT_STATUS_NORMAL, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 3, HIT_HEIGHT_MID, /*Damage*/ 40.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Hitstun*/ 22, /*X Pushback Air*/ 10.0, /*Y Pushback Air*/ 18.0, /*Pushback Frames*/ 8, DAMAGE_KIND_NORMAL, HIT_LEVEL_MEDIUM, "", "");
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
		execute_frame(23, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(146, 58), glm::vec2(-67, -7), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-23, 114), glm::vec2(79, 8), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(185, -9), glm::vec2(-97, 48), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-53, 114), glm::vec2(49, 8), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX_ALL);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-31, 0), glm::vec2(132, 113), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(51, 253), glm::vec2(149, 70), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, SPECIAL_STATUS_CONDITION_JUMP_COUNTERHIT, HIT_STATUS_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_MID, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 23, /*Blockstun*/ 16, /*Hit Pushback Ground*/ 14.0, /*Block Pushback Ground*/ 8.0, /*X Pushback Air*/ 4.0, /*Y Pushback Air*/ 31.0, /*Pushback Frames*/ 11, /*Launch Init Y*/ 28.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch Speed X*/ 7.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(22, 158), glm::vec2(129, 64), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::DISABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		}); 
		execute_wait(1, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_frame(25, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("crouch_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(190, 36), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, 26), glm::vec2(75, 93), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(235, 36), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(230, 36), glm::vec2(129, -7), COLLISION_KIND_GROUND, HIT_STATUS_NORMAL, HIT_HEIGHT_LOW, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 11, /*Blockstun*/ 8, /*Hit Pushback Ground*/ 11.0, /*Block Pushback Ground*/ 13.0, /*Pushback Frames*/ 6, DAMAGE_KIND_NORMAL, HIT_LEVEL_LIGHT, "", "");
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(180, 36), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "crouch_lp", CANCEL_KIND_ANY);
			push_function(&Fighter::ENABLE_CANCEL, "crouch_lk", CANCEL_KIND_ANY);
		});
	});
	script("crouch_mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(190, 36), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, 26), glm::vec2(75, 93), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(293, -10), glm::vec2(14, 42), COLLISION_KIND_GROUND, HIT_STATUS_NORMAL, HIT_HEIGHT_LOW, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 19, /*Blockstun*/ 13, /*Hit Pushback Ground*/ 14.0, /*Block Pushback Ground*/ 8.0, /*Pushback Frames*/ 10, DAMAGE_KIND_NORMAL, HIT_LEVEL_MEDIUM, "", "");
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("crouch_hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(190, 36), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, 26), glm::vec2(75, 93), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-15, 0), glm::vec2(250, 50), COLLISION_KIND_GROUND, HIT_STATUS_KNOCKDOWN, HIT_FLAG_KNOCKDOWN_FACE_DOWN, HIT_HEIGHT_LOW, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 0, /*Blockstun*/ 13, /*Block Pushback Ground*/ 8.0, /*Pushback Frames*/ 10, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("jump_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_VERTICAL_PUSHBACK, true);
			push_function(&Fighter::ENABLE_CANCEL, "jump_mp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-105, -30), glm::vec2(65, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-45, 40), glm::vec2(135, 90), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-80, 75), glm::vec2(145, 125), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-95, 15), glm::vec2(100, 80), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-55, 80), glm::vec2(85, 125), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::CLEAR_HURTBOX, 2);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(60, 15), glm::vec2(185, 65), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 3, HIT_HEIGHT_HIGH, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 16, /*Blockstun*/ 13, /*Hit Pushback Ground*/ 15.0, /*Block Pushback Ground*/ 15.0, /*X Pushback Air*/ 5.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 8, DAMAGE_KIND_NORMAL, HIT_LEVEL_LIGHT, "", "");
		});
		execute_wait(8, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("jump_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_VERTICAL_PUSHBACK, true);
			push_function(&Fighter::ENABLE_CANCEL, "jump_hp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-86, 34), glm::vec2(86, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-73, 34), glm::vec2(136, 103), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(110, 18), glm::vec2(175, 62), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(90, 5), glm::vec2(215, 50), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_HIGH, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 21, /*Blockstun*/ 26, /*Hit Pushback Ground*/ 22.0, /*Block Pushback Ground*/ 22.0, /*X Pushback Air*/ 8.0, /*Y Pushback Air*/ 28.0, /*Pushback Frames*/ 11, DAMAGE_KIND_NORMAL, HIT_LEVEL_MEDIUM, "", "");
		});
		execute_wait(8, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
		execute_frame(22, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-86, 34), glm::vec2(86, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-75, 28), glm::vec2(116, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-83, 74), glm::vec2(316, 120), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-5, 102), glm::vec2(277, 178), COLLISION_KIND_AIR, HIT_STATUS_LAUNCH, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 5, HIT_HEIGHT_HIGH, /*Damage*/ 50.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 12, /*Hitstun*/ 0, /*Launch Init Y*/ -20.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch Speed X*/ 5.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(105, 74), glm::vec2(298, 137), COLLISION_KIND_GROUND, HIT_STATUS_NORMAL, SPECIAL_STATUS_CONDITION_COUNTERHIT, HIT_STATUS_LAUNCH, HIT_FLAG_HARD_KNOCKDOWN, /*Juggle Start*/ 1, HIT_HEIGHT_HIGH, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 25, /*Blockstun*/ 25, /*Hit Pushback Ground*/ 15.0, /*Block Pushback Ground*/ 15.0, /*Pushback Frames*/ 12, /*Launch Init Y*/ 20.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch Speed X*/ 5.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("jump_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(87, 129), glm::vec2(194, 225), COLLISION_KIND_GROUND, HIT_STATUS_NORMAL, HIT_HEIGHT_HIGH, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 2, /*Meter Gain*/ 24.0, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 13, /*Blockstun*/ 17, /*Hit Pushback Ground*/ 20.0, /*Block Pushback Ground*/ 20.0, /*Pushback Frames*/ 7, DAMAGE_KIND_NORMAL, HIT_LEVEL_LIGHT, "", "");
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(87, 129), glm::vec2(194, 225), COLLISION_KIND_AIR, HIT_STATUS_NORMAL, SPECIAL_STATUS_CONDITION_PUNISH, HIT_STATUS_LAUNCH, /*Juggle Start*/ 3, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_HIGH, /*Damage*/ 30.0, /*Damage Scale*/ 2, /*Meter Gain*/ 24.0, /*Hitlag*/ 8, /*Hitstun*/ 13, /*X Pushback Air*/ 5.0, /*Y Pushback Air*/ 25.0, /*Pushback Frames*/ 7, /*Launch Init Y*/ 25.0, /*Launch Gravity*/ 1.0, /*Launch Max Fall Speed*/ 10.0, /*Launch Speed X*/ 5.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_LIGHT, "", "");
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("jump_mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-71, 48), glm::vec2(185, 90), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-103, 59), glm::vec2(55, 10), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_HIGH, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 23, /*Blockstun*/ 18, /*Hit Pushback Ground*/ 15.0, /*Block Pushback Ground*/ 15.0, /*X Pushback Air*/ 5.0, /*Y Pushback Air*/ 28.0, /*Pushback Frames*/ 12, DAMAGE_KIND_NORMAL, HIT_LEVEL_MEDIUM, "", "");
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(-46, 90), glm::vec2(248, 43), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_HIGH, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 23, /*Blockstun*/ 18, /*Hit Pushback Ground*/ 15.0, /*Block Pushback Ground*/ 15.0, /*X Pushback Air*/ 5.0, /*Y Pushback Air*/ 28.0, /*Pushback Frames*/ 12, DAMAGE_KIND_NORMAL, HIT_LEVEL_MEDIUM, "", "");

		});
		execute_wait(5, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("jump_hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(250, 60), glm::vec2(-6, 114), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 2, HIT_HEIGHT_HIGH, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 15, /*Blockstun*/ 11, /*Hit Pushback Ground*/ 15.0, /*Block Pushback Ground*/ 15.0, /*X Pushback Air*/ 5.0, /*Y Pushback Air*/ 28.0, /*Pushback Frames*/ 15, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
		});
		execute_wait(6, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("forward_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(17, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(180, 168), glm::vec2(12, 128), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, HIT_FLAG_CONTINUE_LAUNCH, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, HIT_HEIGHT_HIGH, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 27, /*Blockstun*/ 23, /*Hit Pushback Ground*/ 14.0, /*Block Pushback Ground*/ 8.0, /*X Pushback Air*/ 5.0, /*Y Pushback Air*/ 31.0, /*Pushback Frames*/ 11, /*Launch Init Y*/ 20.0, /*Launch Gravity*/ 1.3, /*Launch Max Fall Speed*/ 15.0, /*Launch Speed X*/ 25.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
	});
	script("backdash_attack", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-215, 0), glm::vec2(-55, 160), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-91, 44), glm::vec2(180, -8), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(15, 25), glm::vec2(155, 160), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(25, 0), glm::vec2(188, 110), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2(295, 170), glm::vec2(90, 100), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(300, 190), glm::vec2(30, 130), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, SPECIAL_STATUS_CONDITION_PUNISH, HIT_STATUS_LAUNCH, HIT_FLAG_HARD_KNOCKDOWN, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 27, /*Blockstun*/ 20, /*Hit Pushback Ground*/ 15.0, /*Block Pushback Ground*/ 10.0, /*X Pushback Air*/ 5.0, /*Y Pushback Air*/ 31.0, /*Pushback Frames*/ 8, /*Launch Init Y*/ 30.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch Speed X*/ 4.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(12, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 2);
			push_function(&Fighter::CLEAR_HURTBOX, 3);
		});
	});
	script("advance_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
		execute_frame(19, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(226, 114), glm::vec2(65, -2), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_FLOAT, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_HIGH, /*Damage*/ 100.0, /*Chip Damage*/ 20.0, /*Damage Scale*/ 1, /*Meter Gain*/ 80.0, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 180, /*Blockstun*/ 5, /*Block Pushback Ground*/ 5.0, /*Pushback Frames*/ 3, /*Launch Init Y*/ -10.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch Speed X*/ 0.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
		});
		execute_frame(20, [this]() {
			push_function(&Fighter::RESET_WORLD_RATE);
		});
	});
	script("advance_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
		execute_frame(19, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(226, 114), glm::vec2(65, -2), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_FLOAT, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_LOW, /*Damage*/ 100.0, /*Chip Damage*/ 20.0, /*Damage Scale*/ 1, /*Meter Gain*/ 80.0, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 180, /*Blockstun*/ 5, /*Block Pushback Ground*/ 5.0, /*Pushback Frames*/ 3, /*Launch Init Y*/ 20.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch Speed X*/ 0.0, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
		});
		execute_frame(20, [this]() {
			push_function(&Fighter::RESET_WORLD_RATE);
		});
	});
	script("advance_forward", [this]() {
	
	});
	script("advance_back", [this]() {

	});
	script("grab_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_GRABBOX, 0, glm::vec2(0, 55), glm::vec2(150, 100), GRABBOX_KIND_NORMAL, COLLISION_KIND_GROUND, FIGHTER_STATUS_GRAB, FIGHTER_STATUS_GRABBED);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_GRABBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("grab", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec3(0.0));
		});
	});
	script("throw_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec3(0.0));
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
		});
		execute_frame(18, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 50.0, 15.0, 8.0, 1.3, 15.0);
		});
	});
	script("throw_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec3(0.0));
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END, true);
		});
		execute_frame(40, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 50.0, -35.0, 8.0, 1.3, 15.0);
		});
	});
	script("grab_air_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-105, -30), glm::vec2(65, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-45, 40), glm::vec2(135, 90), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-80, 75), glm::vec2(145, 125), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_GRABBOX, 0, glm::vec2(-70, 55), glm::vec2(100, 100), GRABBOX_KIND_HITSTUN, COLLISION_KIND_AIR, FIGHTER_STATUS_GRAB_AIR, FIGHTER_STATUS_GRABBED);
		});
	});
	script("grab_air", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec3(0.0));
		});
	});
	script("throw_f_air", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec3(0.0));
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, true);
		});
		execute_frame(17, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 50.0, 15.0, -15.0, 0.0, -15.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, false);
		});
	});
	script("throw_b_air", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec3(0.0));
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END, true);
		});
		execute_frame(17, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 50.0, -15.0, -15.0, 0.0, -15.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, false);
		});
	});
	script("grabbed", [this]() {

	});
	script("special_fireball_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-127, -10), glm::vec2(82, 153), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::SET_RATE, 1.0f);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-122, 175), glm::vec2(202, 98), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_AERIAL);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ACTIVATE_PROJECTILE, 0, glm::vec3(0.0));
		});
		execute_frame(41.0, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("special_fireball_punch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
		execute_frame(5, [this]() {
			push_function(&Fighter::CHANGE_PROJECTILE_STATUS, 0, PROJECTILE_ROWAN_FIREBALL_STATUS_PUNCHED);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ADD_PROJECTILE_POS, 0, glm::vec3(50.0 * facing_dir, 0.0, 0.0));
		});
	});
	script("special_fireball_kick", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::CHANGE_PROJECTILE_STATUS, 0, PROJECTILE_ROWAN_FIREBALL_STATUS_KICKED);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ADD_PROJECTILE_POS, 0, glm::vec3(30.0 * facing_dir, -40.0, 0.0));
		});
	});
	script("special_slide", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-136, -10), glm::vec2(159, 72), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-135, 0), glm::vec2(20, 25));
			push_function(&Fighter::NEW_PUSHBOX, 1, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Fighter::SET_RATE, 1.5);
			}
		});
		execute_frame(3, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] != SPECIAL_LEVEL_H) {
				push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 25.0));
			}
		});
		execute_frame(6, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] != SPECIAL_LEVEL_H) {
				push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30, 0), glm::vec2(120, 25));
			}
		});
		execute_frame(9, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] != SPECIAL_LEVEL_H) {
				push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30, 0), glm::vec2(170, 25));
			}
		});
		execute_frame(12, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] != SPECIAL_LEVEL_H) {
				push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-135, 0), glm::vec2(220, 25));
			}
			else {
				push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_CROSSUP, true);
				push_function(&Fighter::CLEAR_PUSHBOX_ALL);
			}
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(150, -8), glm::vec2(300, 30), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, -12), glm::vec2(250, 30), COLLISION_KIND_GROUND, HIT_STATUS_NORMAL, HIT_HEIGHT_LOW, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 56.0, /*Hitlag*/ 12, /*Blocklag*/ 12, /*Hitstun*/ 40, /*Blockstun*/ 19, /*Hit Pushback Ground*/ 0.0, /*Block Pushback Ground*/ 15.0, /*Pushback Frames*/ 20, DAMAGE_KIND_CHIP, HIT_LEVEL_MEDIUM, "", "");
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, -12), glm::vec2(250, 30), COLLISION_KIND_GROUND, HIT_STATUS_KNOCKDOWN, HIT_HEIGHT_LOW, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 56.0, /*Hitlag*/ 12, /*Blocklag*/ 12, /*Hitstun*/ 4, /*Blockstun*/ 16, /*Block Pushback Ground*/ 15.0, /*Pushback Frames*/ 20, DAMAGE_KIND_CHIP, HIT_LEVEL_MEDIUM, "", "");
			}
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HITBOX, 0);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
		execute_frame(get_local_param_int("special_slide_move_stop_frame", params), [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_CROSSUP, false);
		});
	});
	script("special_slide_followup", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-139, -2), glm::vec2(175, 79), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3.0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, -2), glm::vec2(152, 125), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_HIGH);
		});
		execute_frame(8.0, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(62, 45), glm::vec2(201, 236), COLLISION_KIND_AIR, HIT_STATUS_LAUNCH, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 70.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 12, /*Hitstun*/ 0, /*Launch Init Y*/ 38.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch Speed X*/ 0.5, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(62, 45), glm::vec2(201, 236), COLLISION_KIND_GROUND, HIT_STATUS_LAUNCH, /*Juggle Start*/ 0, HIT_HEIGHT_MID, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 0, /*Blockstun*/ 30, /*Block Pushback Ground*/ 15.0, /*Pushback Frames*/ 5, /*Launch Init Y*/ 45.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch Speed X*/ 0.5, DAMAGE_KIND_CHIP, HIT_LEVEL_HEAVY, "", "");
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(62, 45), glm::vec2(201, 236), COLLISION_KIND_AIR, HIT_STATUS_LAUNCH, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 70.0, /*Damage Scale*/ 1, /*Meter Gain*/ 56.0, /*Hitlag*/ 12, /*Hitstun*/ 0, /*Launch Init Y*/ 38.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch Speed X*/ 9.5, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(62, 45), glm::vec2(201, 236), COLLISION_KIND_GROUND, HIT_STATUS_LAUNCH, /*Juggle Start*/ 0, HIT_HEIGHT_MID, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 56.0, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 0, /*Blockstun*/ 30, /*Block Pushback Ground*/ 15.0, /*Pushback Frames*/ 5, /*Launch Init Y*/ 45.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch Speed X*/ 0.5, DAMAGE_KIND_CHIP, HIT_LEVEL_HEAVY, "", "");
			}
		});
		execute_frame(9.0, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(202, 129), glm::vec2(-72, 256), COLLISION_KIND_AIR, HIT_STATUS_LAUNCH, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 70.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 12, /*Hitstun*/ 0, /*Launch Init Y*/ 45.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch Speed X*/ 0.5, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(202, 129), glm::vec2(-72, 256), COLLISION_KIND_AIR, HIT_STATUS_LAUNCH, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 70.0, /*Damage Scale*/ 1, /*Meter Gain*/ 56.0, /*Hitlag*/ 12, /*Hitstun*/ 0, /*Launch Init Y*/ 45.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch Speed X*/ 0.5, DAMAGE_KIND_NORMAL, HIT_LEVEL_HEAVY, "", "");
			}
			push_function(&Fighter::CLEAR_HITBOX, 1);
		});
		execute_frame(14.0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, -2), glm::vec2(152, 200), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_frame(30.0, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});

	});
	script("special_uppercut_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-118, -4), glm::vec2(65, 138), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-98, -4), glm::vec2(171, 65), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			}
		});
		execute_frame(4, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-114, -12), glm::vec2(187, 127), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(81, 88), glm::vec2(9, 125), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(220, 0), glm::vec2(160, 120), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, HIT_FLAG_DISABLE_HITSTUN_PARRY, /*Juggle Start*/ 5, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, /*Hitlag*/ 12, /*Blocklag*/ 8, /*Hitstun*/ 38, /*Blockstun*/ 17, /*Hit Pushback Ground*/ -10.0, /*Block Pushback Ground*/ 10.0, /*X Pushback Air*/ -3.0, /*Y Pushback Air*/ -1.0, /*Pushback Frames*/ 5, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, "", "");
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(220, 0), glm::vec2(160, 120), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_NORMAL, HIT_FLAG_DISABLE_HITSTUN_PARRY, /*Juggle Start*/ 5, /*Juggle Increase*/ 1, /*Juggle Max*/ 7, HIT_HEIGHT_MID, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 12, /*Blocklag*/ 8, /*Hitstun*/ 38, /*Blockstun*/ 17, /*Hit Pushback Ground*/ -10.0, /*Block Pushback Ground*/ 10.0, /*X Pushback Air*/ -3.0, /*Y Pushback Air*/ -1.0, /*Pushback Frames*/ 5, DAMAGE_KIND_CHIP, HIT_LEVEL_LIGHT, "", "");
			}
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("special_uppercut", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
		execute_frame(4, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(47, 30), glm::vec2(177, 194), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_LAUNCH, /*Juggle Start*/ 6, /*Juggle Increase*/ 1, /*Juggle Max*/ 5, HIT_HEIGHT_MID, /*Damage*/ 60.0, /*Chip Damage*/ 12.0, /*Damage Scale*/ 1, /*Meter Gain*/ 48.0, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 0, /*Blockstun*/ 20, /*Block Pushback Ground*/ 20.0, /*Pushback Frames*/ 5, /*Launch Init Y*/ 43.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 22.0, /*Launch Speed X*/ 10.0, DAMAGE_KIND_CHIP, HIT_LEVEL_HEAVY, "", "");
			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(47, 30), glm::vec2(177, 194), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_LAUNCH, /*Juggle Start*/ 6, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 100.0, /*Chip Damage*/ 20.0, /*Damage Scale*/ 1, /*Meter Gain*/ 80.0, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 0, /*Blockstun*/ 20, /*Block Pushback Ground*/ 20.0, /*Pushback Frames*/ 5, /*Launch Init Y*/ 45.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 22.0, /*Launch Speed X*/ 10.0, DAMAGE_KIND_CHIP, HIT_LEVEL_HEAVY, "", "");
			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(47, 30), glm::vec2(177, 194), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_LAUNCH, /*Juggle Start*/ 6, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, HIT_HEIGHT_MID, /*Damage*/ 110.0, /*Chip Damage*/ 22.0, /*Damage Scale*/ 1, /*Meter Gain*/ 80.0, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 0, /*Blockstun*/ 20, /*Block Pushback Ground*/ 20.0, /*Pushback Frames*/ 5, /*Launch Init Y*/ 47.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 22.0, /*Launch Speed X*/ 10.0, DAMAGE_KIND_CHIP, HIT_LEVEL_HEAVY, "", "");
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(47, 30), glm::vec2(177, 194), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HIT_STATUS_LAUNCH, /*Juggle Start*/ 6, /*Juggle Increase*/ 1, /*Juggle Max*/ 8, HIT_HEIGHT_MID, /*Damage*/ 90.0, /*Chip Damage*/ 18.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 0, /*Blockstun*/ 20, /*Block Pushback Ground*/ 20.0, /*Pushback Frames*/ 5, /*Launch Init Y*/ 49.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 22.0, /*Launch Speed X*/ 10.0, DAMAGE_KIND_CHIP, HIT_LEVEL_HEAVY, "", "");
			}
		});
		execute_frame(8, [this]() {
			push_condition([this]() { return fighter_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT]; });
			push_true(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 500.0));
		});
		execute_frame(12, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("special_uppercut_fall", [this]() {
		execute_frame(0, [this]() {
			push_condition([this]() { return fighter_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT]; });
			push_true(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 500.0));
			push_false(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-76, 115), glm::vec2(123, 182), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("special_uppercut_land", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("special_install", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
	});
	script("throw_tech", [this]() {

	});
	script("throw_tech_air", [this]() {

	});
	script("stand_hitstun_l", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_m", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_h", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_l", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_m", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_h", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hitstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("thrown", [this]() {

	});
	script("hitstun_float_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_float_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			});
		});
	script("launch_start", [this]() {
		

	});
	script("launch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("launch_hkd", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("knockdown_start_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("knockdown_start_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("knockdown_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("knockdown_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("wakeup_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("wakeup_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("wakeup_fast_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("wakeup_fast_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("wakeup_back_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 160.0));
		});
	});	
	script("wakeup_back_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 160.0));
		});
	});
	script("crumple", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_lp_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_mp_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hp_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_lk_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_mk_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hk_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_hitstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("taunt", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("intro", [this]() {
		
	});
	script("knockdown_lose_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("knockdown_lose_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("round_win", [this]() {

	});
	script("round_win_perfect", [this]() {

	});
	script("round_win_timeout", [this]() {

	});
	script("round_lose_timeout", [this]() {

	});
}