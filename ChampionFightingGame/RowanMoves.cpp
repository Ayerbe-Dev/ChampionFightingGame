#include "Rowan.h"
#include "RowanFireball.h"
#include "RowanFireballConstants.h"

void Rowan::load_move_list() {
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("5lp", FIGHTER_STATUS_ATTACK,
		BUTTON_LP_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("5mp", FIGHTER_STATUS_ATTACK,
		BUTTON_MP_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("5hp", FIGHTER_STATUS_ATTACK,
		BUTTON_HP_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("5lk", FIGHTER_STATUS_ATTACK,
		BUTTON_LK_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("5mk", FIGHTER_STATUS_ATTACK,
		BUTTON_MK_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("5hk", FIGHTER_STATUS_ATTACK,
		BUTTON_HK_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("6mp", FIGHTER_STATUS_ATTACK,
		BUTTON_MP_BIT, 1, MOVESET_DIR_FORWARD, false
	);

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("2lp", FIGHTER_STATUS_ATTACK,
		BUTTON_LP_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("2mp", FIGHTER_STATUS_ATTACK,
		BUTTON_MP_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("2hp", FIGHTER_STATUS_ATTACK,
		BUTTON_HP_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("2lk", FIGHTER_STATUS_ATTACK,
		BUTTON_LK_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("2mk", FIGHTER_STATUS_ATTACK,
		BUTTON_MK_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("2hk", FIGHTER_STATUS_ATTACK,
		BUTTON_HK_BIT, 1, MOVESET_DIR_DOWN, true
	);

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("jump", FIGHTER_STATUS_JUMPSQUAT,
		0, 0, MOVESET_DIR_UP, false
	);

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("dash_f", FIGHTER_STATUS_DASH_F,
		INPUT_KIND_66, 0, 0, 0, SPECIAL_LEVEL_SETTING_N, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("dash_b", FIGHTER_STATUS_DASH_B,
		INPUT_KIND_44, 0, 0, 0, SPECIAL_LEVEL_SETTING_N, false
	);

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("backdash_attack",
		FIGHTER_STATUS_BACKDASH_ATTACK, BUTTON_6B_BIT, 1, MOVESET_DIR_FORWARD, false, VBP(),
		0.0f, 0.0f, false, { "dash_b" }
	);

	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("8lp", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_LP_BIT, 1, MOVESET_DIR_ALL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("8mp", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_MP_BIT, 1, MOVESET_DIR_ALL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("8hp", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_HP_BIT, 1, MOVESET_DIR_ALL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("8lk", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_LK_BIT, 1, MOVESET_DIR_ALL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("8mk", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_MK_BIT, 1, MOVESET_DIR_ALL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("8hk", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_HK_BIT, 1, MOVESET_DIR_ALL, false
	);
}

void Rowan::load_move_scripts() {
	script("default", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("wait", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("walk_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("walk_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("dash_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "dash_b", CANCEL_KIND_ANY);
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, "dash_b", CANCEL_KIND_ANY);
		});
	});
	script("dash_b", [this]() {
		execute_frame(0, [this]() {
			execute_frame(0, [this]() {
				push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_THROW);
			});
			execute_frame(9, [this]() {
				push_function(&Fighter::ENABLE_CANCEL, "backdash_attack", CANCEL_KIND_ANY);
			});
			execute_frame(14, [this]() {
				push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_DODGE_COUNTER, false);
				push_function(&Fighter::SET_HURTBOX_INTANGIBLE_KIND, 0, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::SET_HURTBOX_INTANGIBLE_KIND, 1, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::DISABLE_CANCEL, "backdash_attack", CANCEL_KIND_ANY);
			});
		});
	});
	script("crouch_d", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("crouch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_u", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_squat", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 90), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_THROW);
		});
	});
	script("jump_n", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 90), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 75), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 60), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 45), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 30), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 15), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("fall", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 20.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
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
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
		});
	});
	script("turn_crouch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-120, 0), glm::vec2(120, 60), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 40), glm::vec2(55, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_block_start", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("stand_block", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("stand_blockstun", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("high_blockstun", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_blockstun", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("parry_start", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("parry_start_low", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("parry_air_start", [this]() {
		execute_frame(0, [this]() {

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

		});
	});
	script("hitstun_parry_air_start", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("5lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0, 131), glm::vec2(180, 160));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::PLAY_RESERVED_SOUND, "rowan_attack_01");
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(0, 120), glm::vec2(180, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 131),
				glm::vec2(190, 170), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(30).meter(18).start_scale(0.8f).hit(8, 12).block(8, 9).j_min(1)
				.anims("light_high", "light_high", "high", "high"), HIT_STATUS_NORMAL,
				HitMove().pushback(50.0f, 30.0f).frames(7).air(25.0, 1.6, 15.0, 16.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "5lp", CANCEL_KIND_ANY);
			push_function(&Fighter::ENABLE_CANCEL, "5mp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "6mp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("5mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0, 110), glm::vec2(200, 165));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, "5mp", CANCEL_KIND_ANY);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 120),
				glm::vec2(240, 170), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(60).meter(36).hit(12, 22).block(12, 15).j_min(1)
				.anims("light_high", "light_high", "high", "high"), HIT_STATUS_NORMAL,
				HitMove().pushback(80.0, 80.0).frames(7).air(25.0, 1.6, 15.0, 16.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::ENABLE_CANCEL, "5hp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "5hk", CANCEL_KIND_CONTACT);
		});
	});
	script("5hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0, 130), glm::vec2(210, 170));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 35, 125 }, glm::vec2{ 150, 175 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 130),
			glm::vec2(220, 170), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
			HitResult().damage(100).meter(48).hit(16, 23).block(16, 20).j_start(2).j_inc(2).j_max(1)
			.anims("heavy_high", "heavy_high", "mid", "mid"), HIT_STATUS_NORMAL,
				HitMove().pushback(170.0, 80.0).frames(11).air(25.0, 1.6, 15.0, 16.0),
				HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL,
				"", "common_attack_hit_01");
		});
		execute_wait(11, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 35, 125 }, glm::vec2{ 200, 175 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("5lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0, 0), glm::vec2(210, 60));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-35, 0), glm::vec2(210, 60), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(30, 45), glm::vec2(160, 95), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 0),
			glm::vec2(220, 60), COLLISION_KIND_GROUND, HitResult().damage(30).meter(18).hit(8, 13)
			.block(8, 8).anims("light_low", "light_mid", "low", "low"), HIT_STATUS_NORMAL,
				HitMove().pushback(40.0, 40.0).frames(7), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "5mk", CANCEL_KIND_CONTACT);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
	});
	script("5mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0.0, 80.0), glm::vec2(250.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

			push_function(&Fighter::ENABLE_CANCEL, "dash_b", CANCEL_KIND_CONTACT);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(25, 60), glm::vec2(150, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(25, 60), glm::vec2(180, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(25, 70), glm::vec2(240, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(30, 80),
				glm::vec2(260, 120), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(60).meter(36).hit(10, 23).block(10, 12)
				.anims("medium_mid", "medium_mid", "mid", "mid"), HIT_STATUS_NORMAL,
				HitMove().pushback(100.0, 40.0).frames(10).air(10.0, 1.3, 15.0, 1.0),
				HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL,
				"", "common_attack_hit_01"
			);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(6, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("5hk", [this]() {
		execute_frame(0.0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0.0, 95.0), glm::vec2(280.0, 180.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7.0, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 80),
			glm::vec2(165, 135), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
			HitResult().damage(50).meter(36).hit(6, 20).block(6, 13).j_start(4).j_inc(2)
			.j_max(2).anims("heavy_mid", "heavy_high", "high", "mid"), HIT_STATUS_NORMAL,
				HitMove().pushback(130.0, 60.0).frames(5).air(22.0, 1.5, 15.0, 8.0),
				HIT_FLAG_DISABLE_HITSTUN_PARRY | HIT_FLAG_CONTINUE_LAUNCH,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
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
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 1, glm::vec2(60, 95),
				glm::vec2(290, 180), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(40).scale(0.0f).start_scale(0.9f).meter(18).hit(6, 24).block(6, 15)
				.anims("heavy_high", "heavy_high", "high", "mid").j_start(1).j_inc(1).j_max(3),
				HIT_STATUS_NORMAL, HitMove().pushback(170.0, 100.0).frames(10)
				.air(27.0, 1.5, 15.0, 5.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
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
			push_function(&Fighter::CLEAR_BLOCKBOX);
		});
		execute_frame(17.0, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("6mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0, 110), glm::vec2(200, 165));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 35, 110 }, glm::vec2{ 210, 150 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(11, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(100, 110),
			glm::vec2(210, 165), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
			HitResult().damage(60).meter(36).hit(12, 12).block(12, 11).j_min(2)
			.anims("medium_high", "medium_high", "mid", "mid"), HIT_STATUS_NORMAL,
				HitMove().pushback(80.0, 40.0).frames(9).air(25.0, 1.6, 15.0, 16.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "2hp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "6hp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("2lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(50.0, 51.0), glm::vec2(150.0, 89.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(40, 45), glm::vec2(180, 95), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 51),
				glm::vec2(160, 89), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(30).meter(18).hit(6, 11).block(6, 5).j_min(1)
				.anims("light_mid", "light_mid", "low", "mid"), HIT_STATUS_NORMAL,
				HitMove().pushback(40.0, 60.0).frames(8).air(25.0, 1.6, 15.0, 16.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01");
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
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "2lp", CANCEL_KIND_ANY);
			push_function(&Fighter::ENABLE_CANCEL, "2lk", CANCEL_KIND_ANY);
		});
	});
	script("2mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(120.0, 50.0), glm::vec2(230.0, 90.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(80, 40), glm::vec2(240, 100), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(120, 50),
			glm::vec2(240, 90), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
			HitResult().damage(50).meter(36).hit(12, 21).block(12, 17)
			.anims("medium_mid", "medium_mid", "mid", "mid").j_start(1).j_inc(1).j_max(3),
				HIT_STATUS_NORMAL, HitMove().pushback(60.0, 80.0).frames(11).air(25.0, 1.6, 15.0, 16.0),
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(80, 40), glm::vec2(250, 100), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("2hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0.0, 40.0), glm::vec2(170.0, 220.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "jump", CANCEL_KIND_CONTACT);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 40),
			glm::vec2(180, 89), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
			HitResult().damage(90).meter(54).hit(16, 23).block(16, 16).crit(32, 0).j_start(1)
			.j_inc(1).j_max(1).anims("forced", "", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().pushback(140.0, 80.0).frames(10).air(25.0, 1.6, 15.0, 16.0),
				HIT_FLAG_FORCE_STAND | HIT_FLAG_DISABLE_HITSTUN_PARRY, CRITICAL_CONDITION_NONE,
				HIT_STATUS_LAUNCH, HitMove().air(28.0, 1.3, 15.0, 7.0), HIT_FLAG_NONE, 
				HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 70),
			glm::vec2(180, 160), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
			HitResult().damage(90).meter(54).hit(16, 23).block(16, 16).crit(32, 0).j_start(1)
			.j_inc(1).j_max(1).anims("forced", "", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().pushback(140.0, 80.0).frames(10).air(20.0, 1.3, 15.0, 7.0),
				HIT_FLAG_FORCE_STAND | HIT_FLAG_DISABLE_HITSTUN_PARRY, 
				CRITICAL_CONDITION_DODGE_COUNTER, HIT_STATUS_LAUNCH,
				HitMove().air(28.0, 1.3, 15.0, 7.0), HIT_FLAG_NONE, HIT_HEIGHT_MID,
				DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(20, 50), glm::vec2(130, 170), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 70),
				glm::vec2(180, 220), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(90).meter(54).hit(16, 23).block(16, 16).crit(32, 0).j_start(1)
				.j_inc(1).j_max(1).anims("forced", "", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().pushback(140.0, 80.0).frames(10).air(25.0, 1.6, 15.0, 16.0),
				HIT_FLAG_FORCE_STAND | HIT_FLAG_DISABLE_HITSTUN_PARRY, CRITICAL_CONDITION_NONE, 
				HIT_STATUS_LAUNCH, HitMove().air(28.0, 1.3, 15.0, 7.0), HIT_FLAG_NONE, 
				HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_frame(25, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("2lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(130.0, 0.0), glm::vec2(220.0, 40.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(170, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(240, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 0),
				glm::vec2(230, 40.0), COLLISION_KIND_GROUND, HitResult().damage(30).meter(18).hit(8, 11)
				.block(8, 8).anims("light_low", "light_low", "", "low"),
				HIT_STATUS_NORMAL, HitMove().pushback(70.0, 90.0).frames(6),
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_LOW, DAMAGE_KIND_NORMAL,
				"", "common_attack_hit_01"
			);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::ENABLE_CANCEL, "2lp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "2lk", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "2mk", CANCEL_KIND_CONTACT);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(170, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "2lp", CANCEL_KIND_ANY);
			push_function(&Fighter::ENABLE_CANCEL, "2lk", CANCEL_KIND_ANY);
		});
	});
	script("2mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(15.0, 0.0), glm::vec2(285.0, 40.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(190, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(250, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(15, 0),
				glm::vec2(295, 40), COLLISION_KIND_GROUND, HitResult().damage(60).meter(36).hit(12, 19)
				.block(12, 13).anims("medium_low", "medium_low", "", "low"), HIT_STATUS_NORMAL,
				HitMove().pushback(14.0, 8.0).frames(10), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_LOW, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(310, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("2hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0.0, 0.0), glm::vec2(240.0, 50.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(190, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 0),
			glm::vec2(250, 50), COLLISION_KIND_GROUND, HitResult().damage(80).meter(48).hit(16, 0)
			.block(16, 13).anims("", "", "", "low"), HIT_STATUS_KNOCKDOWN,
				HitMove().pushback(0.0, 8.0).frames(10), HIT_FLAG_KNOCKDOWN_FACE_DOWN,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_LOW, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01");
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(270, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("8lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(20, 85),
			glm::vec2(145, 135), COLLISION_KIND_AIR, HitResult().damage(20).meter(18).hit(8, 16)
			.j_start(3).j_inc(0).j_max(4), HIT_STATUS_NORMAL, HitMove().air(-1.0f, 1.0f, 20.0f, 5.0f),
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL,
				"", "common_attack_hit_01"
			);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "8mp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 1, glm::vec2(60, 5),
				glm::vec2(185, 85), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HitResult().damage(20)
				.meter(18).hit(8, 11).block(8, 7).j_start(3).j_inc(0).j_max(4)
				.anims("light_high", "light_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().pushback(150.0, 150.0).frames(8).air(3.3, 1.6, 15.0, 4.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("8mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "8mk", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "8hp", CANCEL_KIND_CONTACT);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(90, 0),
			glm::vec2(215, 70), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
			HitResult().damage(40).meter(24).hit(10, 14).block(10, 11).j_start(3).j_inc(0).j_max(4)
			.anims("medium_high", "medium_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().pushback(220.0, 220.0).frames(11).air(-2.0, 1.0, 30.0, 5.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(8, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
	});
	script("8hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(100, 75),
				glm::vec2(280, 135), COLLISION_KIND_AIR,
				HitResult().damage(50).meter(30).hit(12, 0).crit(12, 17).j_start(3).j_inc(0).j_max(4),
				HIT_STATUS_LAUNCH, HitMove().air(-20.0, 0.0, 20.0, 5.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_COUNTERHIT, HIT_STATUS_FLOAT, HitMove().air(-20.0, 0.0, 20.0, 5.0),
				HIT_FLAG_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(100, 75),
				glm::vec2(280, 135), COLLISION_KIND_GROUND,
				HitResult().damage(50).meter(30).hit(12, 17).block(12, 14)
				.anims("heavy_high", "heavy_high", "high", ""),
				HIT_STATUS_NORMAL, HitMove().pushback(150.0, 150.0).frames(12), HIT_FLAG_NONE,
				CRITICAL_CONDITION_COUNTERHIT, HIT_STATUS_FLOAT, HitMove(),
				HIT_FLAG_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(30, -10),
				glm::vec2(180, 90), COLLISION_KIND_AIR,
				HitResult().damage(50).meter(30).hit(12, 0).crit(12, 17).j_start(3).j_inc(0).j_max(4),
				HIT_STATUS_LAUNCH, HitMove().air(-20.0, 0.0, 20.0, 5.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_COUNTERHIT, HIT_STATUS_FLOAT, HitMove().air(-20.0, 0.0, 20.0, 5.0),
				HIT_FLAG_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(30, -10),
				glm::vec2(180, 90), COLLISION_KIND_GROUND,
				HitResult().damage(50).meter(30).hit(12, 17).block(12, 21)
				.anims("heavy_high", "heavy_high", "high", ""),
				HIT_STATUS_NORMAL, HitMove().pushback(150.0, 150.0).frames(12), HIT_FLAG_NONE,
				CRITICAL_CONDITION_COUNTERHIT, HIT_STATUS_FLOAT, HitMove(),
				HIT_FLAG_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("8lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(5, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(60, 100),
				glm::vec2(190, 190), COLLISION_KIND_GROUND,
				HitResult().damage(30).meter(18).hit(8, 11).block(8, 7)
				.anims("light_high", "light_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().pushback(20.0, 20.0).frames(7), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE,
				HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(60, 100),
				glm::vec2(190, 190), COLLISION_KIND_AIR,
				HitResult().damage(30).meter(18).hit(8, 17).j_start(3).j_inc(0).j_max(4),
				HIT_STATUS_NORMAL, HitMove().air(15.0, 1.0, 10.0, 5.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_PUNISH, HIT_STATUS_LAUNCH, HitMove().air(25.0, 1.0, 10.0, 5.0),
				HIT_FLAG_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("8mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "8lk", CANCEL_KIND_CONTACT);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(110, -15),
				glm::vec2(270, 45), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(40).meter(24).hit(10, 14).block(10, 11).j_start(3).j_inc(0).j_max(4)
				.anims("medium_high", "medium_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().pushback(150.0, 150.0).frames(5).air(15.0, 1.0, 15.0, 10.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(-45, 45),
				glm::vec2(250, 80), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(40).meter(24).hit(10, 14).block(10, 11).j_start(3).j_inc(0).j_max(4)
				.anims("medium_high", "medium_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().pushback(150.0, 150.0).frames(5).air(15.0, 1.0, 15.0, 10), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("8hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 20),
				glm::vec2(250, 80), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(50).meter(30).hit(12, 17).block(12, 14).j_start(3).j_inc(0).j_max(4)
				.anims("heavy_high", "heavy_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().pushback(150.0, 150.0).frames(15).air(6.0, 1.0, 15.0, 10), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(6, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("backdash_attack", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-215, 0), glm::vec2(-55, 160), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(120, 0), glm::vec2(295, 170), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(300, 190),
				glm::vec2(30, 130), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(50).meter(30).hit(12, 27).block(12, 20).j_start(0).j_inc(1).j_max(4)
				.anims("medium_high", "medium_high", "high", "high"), HIT_STATUS_NORMAL,
				HitMove().pushback(15.0, 10.0).frames(8).air(20.0, 1.3, 15.0, 7.0), HIT_FLAG_CONTINUE_LAUNCH,
				CRITICAL_CONDITION_PUNISH, HIT_STATUS_LAUNCH, HitMove().air(30.0, 1.3, 15.0, 4.0),
				HIT_FLAG_HARD_KNOCKDOWN, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(12, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
	});
	script("advance_high", [this]() {
		execute_frame(20, [this]() {
			object_manager->reset_world_rate(this);
		});
	});
	script("advance_low", [this]() {
		execute_frame(20, [this]() {
			object_manager->reset_world_rate(this);
		});
	});
	script("advance_forward", [this]() {

	});
	script("advance_back", [this]() {

	});
	script("grab", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("throw_f", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("throw_b", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("grab_air", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("throw_f_air", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("throw_b_air", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("special_uppercut_start", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("special_uppercut", [this]() {
		execute_frame(4, [this]() {

		});
	});
	script("special_uppercut_fall", [this]() {

	});
	script("hitstun_stand_light_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_stand_light_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_stand_light_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_stand_medium_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_stand_medium_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_stand_medium_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_stand_heavy_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_stand_heavy_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_stand_overhead", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			});
		});
	script("hitstun_stand_forced", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_light_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_light_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_light_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_medium_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_medium_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_medium_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_heavy_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_heavy_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_overhead", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_crouch_forced", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 120.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hitstun", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("launch_start", [this]() {

	});
	script("launch", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("knockdown_start_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("knockdown_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
			if (object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN]) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			}
		});
	});
	script("wakeup_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("knockdown_start_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("knockdown_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
			if (object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN]) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			}
		});
	});
	script("wakeup_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("crumple", [this]() {

	});
	script("landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("8lp_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("8mp_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("8hp_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("8lk_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("8mk_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("8hk_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_hitstun", [this]() {
		execute_frame(0, [this]() {

		});
	});
}

void Rowan::load_cpu_move_info() {
	cpu.add_action("5lp", "5lp", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_LP_BIT, 5, { 4, 5, 6 }, { CPU_TAG_MULTIHIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("5mp", "5mp", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MP_BIT, 5, { 4, 5 }, { CPU_TAG_HIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("5hp", "5hp", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_HP_BIT, 5, { 4, 5 }, { CPU_TAG_HIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("5lk", "5lk", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_LK_BIT, 5, { 4, 5, 6 }, { CPU_TAG_MULTIHIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("5mk", "5mk", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MK_BIT, 5, { 4, 5, 6 }, { CPU_TAG_HIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("5hk", "5hk", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_HK_BIT, 5, { 4, 5, 6 }, { CPU_TAG_HIT_CONFIRM }, {}, 0.0f, false, true);

	cpu.add_action("6mp", "6mp", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MP_BIT, 6, { 6 }, { CPU_TAG_HIT_CONFIRM }, {}, 0.0f, false, true);
//	cpu.add_action("6hp", "6hp", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_HP_BIT, 6, { 6 }, {}, {}, 0.0f, false);

	cpu.add_action("2lp", "2lp", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_LP_BIT, 2, { 1, 2, 3 }, { CPU_TAG_MULTIHIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("2mp", "2mp", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MP_BIT, 2, { 1, 2, 3 }, { CPU_TAG_HIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("2hp", "2hp", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_HP_BIT, 2, { 1, 2, 3 }, {}, {}, 0.0f, false, true);
	cpu.add_action("2lk", "2lk", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_LK_BIT, 2, { 1, 2, 3 }, { CPU_TAG_MULTIHIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("2mk", "2mk", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_MK_BIT, 2, { 1, 2, 3 }, {}, {}, 0.0f, false, true);
	cpu.add_action("2hk", "2hk", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_HK_BIT, 2, { 1, 2, 3 }, {}, {}, 0.0f, false, true);

	cpu.add_action("backdash_attack", "backdash_attack", FIGHTER_CONTEXT_GROUND, INPUT_KIND_NORMAL, BUTTON_LP_BIT, 6, { 3, 6, 9 }, { CPU_TAG_HIT_CONFIRM }, {}, 0.0f, false, false);

	cpu.add_action("8lp", "8lp", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, BUTTON_LP_BIT, 5, MOVESET_DIR_ALL, { CPU_TAG_AIR_ATK_RISING, CPU_TAG_MULTIHIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("8mp", "8mp", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, BUTTON_MP_BIT, 5, MOVESET_DIR_ALL, { CPU_TAG_MULTIHIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("8hp", "8hp", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, BUTTON_HP_BIT, 5, MOVESET_DIR_ALL, { CPU_TAG_HIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("8lk", "8lk", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, BUTTON_LK_BIT, 5, MOVESET_DIR_ALL, { CPU_TAG_MULTIHIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("8mk", "8mk", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, BUTTON_MK_BIT, 5, MOVESET_DIR_ALL, { CPU_TAG_AIR_ATK_CROSSUP, CPU_TAG_MULTIHIT_CONFIRM }, {}, 0.0f, false, true);
	cpu.add_action("8hk", "8hk", FIGHTER_CONTEXT_AIR, INPUT_KIND_NORMAL, BUTTON_HK_BIT, 5, MOVESET_DIR_ALL, { CPU_TAG_HIT_CONFIRM }, {}, 0.0f, false, true);
}