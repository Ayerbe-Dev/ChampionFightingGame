#include "Rowan.h"
#include "RowanFireball.h"
#include "RowanFireballConstants.h"
#include "SoundConstants.h"

void Rowan::load_move_list() {
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("stand_lp", FIGHTER_STATUS_ATTACK,
		BUTTON_LP_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("stand_mp", FIGHTER_STATUS_ATTACK,
		BUTTON_MP_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("stand_hp", FIGHTER_STATUS_ATTACK,
		BUTTON_HP_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("stand_lk", FIGHTER_STATUS_ATTACK,
		BUTTON_LK_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("stand_mk", FIGHTER_STATUS_ATTACK,
		BUTTON_MK_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("stand_hk", FIGHTER_STATUS_ATTACK,
		BUTTON_HK_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("forward_hp", FIGHTER_STATUS_ATTACK,
		BUTTON_HP_BIT, 1, MOVESET_DIR_FORWARD, false
	);

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("crouch_lp", FIGHTER_STATUS_ATTACK,
		BUTTON_LP_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("crouch_mp", FIGHTER_STATUS_ATTACK,
		BUTTON_MP_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("crouch_hp", FIGHTER_STATUS_ATTACK,
		BUTTON_HP_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("crouch_lk", FIGHTER_STATUS_ATTACK,
		BUTTON_LK_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("crouch_mk", FIGHTER_STATUS_ATTACK,
		BUTTON_MK_BIT, 1, MOVESET_DIR_DOWN, true
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("crouch_hk", FIGHTER_STATUS_ATTACK,
		BUTTON_HK_BIT, 1, MOVESET_DIR_DOWN, true
	);

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("stand_mp_crouch_hp", FIGHTER_STATUS_ATTACK,
		BUTTON_HP_BIT, 1, MOVESET_DIR_DOWN, true, VBP(), 0.0f, 0.0f, false, { "stand_mp" }
	);

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("jump_squat", FIGHTER_STATUS_JUMPSQUAT,
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

	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("special_uppercut",
		CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START, INPUT_KIND_623, BUTTON_3P_BIT, 1, 0,
		SPECIAL_LEVEL_SETTING_L_M_H_EX, false, VBP(), 200.0f, 200.0f
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("special_fireball", 
		CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START, INPUT_KIND_236, BUTTON_3P_BIT, 1, 0, 
		SPECIAL_LEVEL_SETTING_L_M_H_EX, false, VBP(object_flag[CHARA_ROWAN_FLAG_FIREBALL_ACTIVE]),
		200.0f, 200.0f
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("special_slide",
		CHARA_ROWAN_STATUS_SPECIAL_SLIDE, INPUT_KIND_214, BUTTON_3K_BIT, 1, 0,
		SPECIAL_LEVEL_SETTING_L_M_H_EX, false, VBP(), 200.0f, 200.0f
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("special_install",
		CHARA_ROWAN_STATUS_SPECIAL_INSTALL, INPUT_KIND_22, BUTTON_3P_BIT, 1, 0,
		SPECIAL_LEVEL_SETTING_N, false
	);



	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("grab_start", FIGHTER_STATUS_GRAB_START,
		BUTTON_2L_BIT, 2, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("parry_start", FIGHTER_STATUS_PARRY_START,
		BUTTON_2M_BIT, 2, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("advance", FIGHTER_STATUS_ADVANCE,
		BUTTON_2H_BIT, 2, MOVESET_DIR_NEUTRAL, false, VBP(), 100.0f, 300.0f
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("advance_forward", FIGHTER_STATUS_ADVANCE_FORWARD,
		BUTTON_2H_BIT, 2, MOVESET_DIR_FORWARD, false, VBP(), 100.0f, 300.0f
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("advance_back", FIGHTER_STATUS_ADVANCE_BACK,
		BUTTON_2H_BIT, 2, MOVESET_DIR_BACK, false, VBP(), 100.0f, 300.0f
	);
	move_list[FIGHTER_CONTEXT_GROUND].add_movelist_entry("taunt", FIGHTER_STATUS_TAUNT,
		BUTTON_6B_BIT, 6, MOVESET_DIR_NEUTRAL, false
	);

	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("jump_lp", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_LP_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("jump_mp", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_MP_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("jump_hp", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_HP_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("jump_lk", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_LK_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("jump_mk", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_MK_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("jump_hk", FIGHTER_STATUS_ATTACK_AIR,
		BUTTON_HK_BIT, 1, MOVESET_DIR_NEUTRAL, false
	);

	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("grab_start", FIGHTER_STATUS_GRAB_AIR_START,
		BUTTON_2L_BIT, 2, MOVESET_DIR_NEUTRAL, false
	);
	move_list[FIGHTER_CONTEXT_AIR].add_movelist_entry("parry_start", FIGHTER_STATUS_PARRY_START,
		BUTTON_2M_BIT, 2, MOVESET_DIR_NEUTRAL, false
	);
}

void Rowan::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::SET_RATE, 0.5);
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
	});
	script("dash_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_THROW);
		});
		execute_frame(14, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_DODGE_COUNTER, false);
			push_function(&Fighter::SET_HURTBOX_INTANGIBLE_KIND, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_HURTBOX_INTANGIBLE_KIND, 1, INTANGIBLE_KIND_NONE);
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
	script("jump", [this]() {
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
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_block", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0, 131), glm::vec2(200, 170));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::PLAY_RESERVED_SOUND, "rowan_attack_01");
			push_function(&Fighter::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.4), glm::vec4(0.0), "HaveL", glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(0, 120), glm::vec2(210, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 131),
				glm::vec2(200, 170), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(30).meter(18).scale(0.2f).hit(8, 12).block(8, 9).j_min(1)
				.anims("light_high", "light_high", "high", "high"), HIT_STATUS_NORMAL, 
				HitMove().ground(30.0f, 30.0f).air(8.0, 8.0).frames(7), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		push_condition("whiff_anim_check", [this]() { 
			return object_flag[FIGHTER_FLAG_ATTACK_HIT] || object_flag[FIGHTER_FLAG_ATTACK_BLOCKED];
		});
		execute_wait(2, [this]() {
			//We use ANY instead of CONTACT for the following line because the stand_lp -> stand_mp link
			//should be manually timed even though it's on contact only; Cancels on contact have an
			//infinite buffer window and in this case we don't want that.
			push_true("whiff_anim_check", &Fighter::ENABLE_CANCEL, "stand_mp", CANCEL_KIND_ANY);
			push_false("whiff_anim_check", &Fighter::ENABLE_CANCEL, "stand_lp", CANCEL_KIND_ANY);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_false("whiff_anim_check", &Fighter::CHANGE_ANIM, "stand_lp_whiff");
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, "stand_mp", CANCEL_KIND_ANY);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 120),
			glm::vec2(240, 170), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
			HitResult().damage(10).meter(6).scale(0.2f).hit(8, 12).block(8, 9).j_min(1)
			.anims("light_high", "light_high", "high", "high"), HIT_STATUS_NORMAL,
				HitMove().ground(80.0, 80.0).air(3.0, 25.0).frames(7), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::ENABLE_CANCEL, "stand_hp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "stand_hk", CANCEL_KIND_CONTACT);
		});
	});
	script("stand_lp_whiff", [this]() {
		execute_frame(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("stand_mp", [this]() {
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
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(100, 110), 
				glm::vec2(210, 165), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(60).meter(36).hit(10, 12).block(12, 11).j_min(2)
				.anims("medium_high", "medium_high", "mid", "mid"), HIT_STATUS_NORMAL, 
				HitMove().ground(80.0, 40.0).air(3.0, 28.0).frames(9), HIT_FLAG_NONE, 
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "stand_mp_crouch_hp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "forward_hp", CANCEL_KIND_CONTACT);
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
	script("stand_hp", [this]() {
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
				HitResult().damage(100).meter(48).hit(11, 23).block(16, 23).j_start(2).j_inc(2).j_max(1)
				.anims("heavy_high", "heavy_high", "mid", "mid"), HIT_STATUS_NORMAL, 
				HitMove().ground(170.0, 80.0).air(5.0, 31.0).frames(11).launch(25.0, 1.6, 15.0, 16.0),
				HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL,
				"", "common_attack_hit_01");
		});
		execute_wait(5, [this]() {
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
	script("stand_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0, 0), glm::vec2(240, 60));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-35, 0), glm::vec2(220, 60), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(30, 45), glm::vec2(160, 95), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 0), 
				glm::vec2(250, 60), COLLISION_KIND_GROUND, HitResult().damage(30).meter(18).hit(8, 13)
				.block(8, 8).anims("light_low", "light_mid", "low", "low"), HIT_STATUS_NORMAL, 
				HitMove().ground(80.0, 80.0).frames(7), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, 
				HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "stand_mk", CANCEL_KIND_CONTACT);
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
	script("stand_mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0.0, 105.0), glm::vec2(320.0, 155.0));
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
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(25, 105), glm::vec2(290, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(30, 105), 
				glm::vec2(330, 155), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
				HitResult().damage(60).meter(36).hit(10, 18).block(12, 12)
				.anims("medium_mid", "medium_mid", "mid", "mid"), HIT_STATUS_NORMAL, 
				HitMove().ground(100.0, 40.0).air(30.0, 28.0).frames(10).launch(10.0, 1.3, 15.0, 1.0),
				HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, 
				"", "common_attack_hit_01");
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
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0.0, 95.0), glm::vec2(280.0, 180.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7.0, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 80), 
				glm::vec2(165, 135), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(50).meter(36).hit(6, 20).block(6, 13).j_start(4).j_inc(2)
				.j_max(2).anims("heavy_high", "heavy_high", "high", "mid"), HIT_STATUS_NORMAL, 
				HitMove().ground(130.0, 60.0).air(2.0, 31.0).frames(5).launch(22.0, 1.5, 15.0, 8.0),
				HIT_FLAG_DISABLE_HITSTUN_PARRY | HIT_FLAG_CONTINUE_LAUNCH, 
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID,  DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
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
				HIT_STATUS_NORMAL, HitMove().ground(170.0, 100.0).air(3.0, 31.0).frames(10)
				.launch(27.0, 1.5, 15.0, 5.0), HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, 
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
	script("crouch_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(50.0, 51.0), glm::vec2(190.0, 89.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(40, 45), glm::vec2(215, 95), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 51), 
				glm::vec2(200, 89), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(30).meter(18).hit(6, 11).block(6, 5).j_min(1)
				.anims("light_mid", "light_mid", "low", "mid"), HIT_STATUS_NORMAL,
				HitMove().ground(9.0, 11.0).air(4.0, 25.0).frames(8), HIT_FLAG_NONE, 
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
			push_function(&Fighter::ENABLE_CANCEL, "crouch_lp", CANCEL_KIND_ANY);
			push_function(&Fighter::ENABLE_CANCEL, "crouch_lk", CANCEL_KIND_ANY);
		});
	});
	script("crouch_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(120.0, 50.0), glm::vec2(270.0, 90.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(80, 40), glm::vec2(260, 100), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(120, 50), 
				glm::vec2(280, 90), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
				HitResult().damage(50).meter(36).hit(10, 21).block(12, 17)
				.anims("medium_mid", "medium_mid", "mid", "mid").j_start(1).j_inc(1).j_max(3), 
				HIT_STATUS_NORMAL, HitMove().ground(80.0, 80.0).air(10.0, 18.0).frames(11), 
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(80, 40), glm::vec2(300, 100), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("crouch_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0.0, 40.0), glm::vec2(170.0, 220.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);			
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 40), 
				glm::vec2(180, 89), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
				HitResult().damage(90).meter(54).hit(12, 23).block(16, 16).crit(32, 0).j_start(1)
				.j_inc(1).j_max(1).anims("forced", "", "mid", "mid"), 
				HIT_STATUS_NORMAL, HitMove().ground(140.0, 80.0).air(130.0, 200.0).frames(10), 
				HIT_FLAG_FORCE_STAND, CRITICAL_CONDITION_NONE, HIT_STATUS_LAUNCH, 
				HitMove().launch(28.0, 1.3, 15.0, 7.0), HIT_FLAG_NONE, HIT_HEIGHT_MID, 
				DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 70),
				glm::vec2(180, 160), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(90).meter(54).hit(12, 23).block(16, 16).crit(32, 0).j_start(1)
				.j_inc(1).j_max(1).anims("forced", "", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().ground(140.0, 80.0).air(130.0, 200.0).frames(10),
				HIT_FLAG_FORCE_STAND, CRITICAL_CONDITION_DODGE_COUNTER, HIT_STATUS_LAUNCH,
				HitMove().launch(28.0, 1.3, 15.0, 7.0), HIT_FLAG_NONE, HIT_HEIGHT_MID,
				DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(20, 50), glm::vec2(130, 170), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(50, 70),
				glm::vec2(180, 220), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(90).meter(54).hit(12, 23).block(16, 16).crit(32, 0).j_start(1)
				.j_inc(1).j_max(1).anims("forced", "", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().ground(140.0, 80.0).air(130.0, 200.0).frames(10),
				HIT_FLAG_FORCE_STAND, CRITICAL_CONDITION_NONE, HIT_STATUS_LAUNCH,
				HitMove().launch(28.0, 1.3, 15.0, 7.0), HIT_FLAG_NONE, HIT_HEIGHT_MID,
				DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
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
	script("crouch_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(130.0, 0.0), glm::vec2(220.0, 40.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(170, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(240, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(130, 0), 
				glm::vec2(230, 40.0), COLLISION_KIND_GROUND, HitResult().damage(30).meter(18).hit(8, 11)
				.block(8, 8).anims("light_low", "light_low", "", "low"),
				HIT_STATUS_NORMAL, HitMove().ground(11.0, 13.0).frames(6),
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_LOW, DAMAGE_KIND_NORMAL, 
				"", "common_attack_hit_01"
			);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(170, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "crouch_lp", CANCEL_KIND_ANY);
			push_function(&Fighter::ENABLE_CANCEL, "crouch_lk", CANCEL_KIND_ANY);
		});
	});
	script("crouch_mk", [this]() {
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
				glm::vec2(295, 40), COLLISION_KIND_GROUND, HitResult().damage(60).meter(36).hit(10, 19)
				.block(12, 13).anims("medium_low", "medium_low", "", "low"), HIT_STATUS_NORMAL,
				HitMove().ground(14.0, 8.0).frames(10), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, 
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
	script("crouch_hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(0.0, 0.0), glm::vec2(240.0, 50.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(120, 0), glm::vec2(190, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 0), 
			glm::vec2(250, 50), COLLISION_KIND_GROUND, HitResult().damage(80).meter(48).hit(12, 0)
			.block(16, 13).anims("", "", "", "low"), HIT_STATUS_KNOCKDOWN, 
			HitMove().ground(0.0, 8.0).frames(10), HIT_FLAG_KNOCKDOWN_FACE_DOWN, 
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
	script("jump_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(20, 85), 
				glm::vec2(145, 135), COLLISION_KIND_AIR, HitResult().damage(20).meter(18).hit(8, 16)
				.j_start(3).j_inc(0).j_max(4), HIT_STATUS_NORMAL, HitMove().air(20.0, -20.0).frames(1), 
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, 
				"", "common_attack_hit_01"
			);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, "jump_lp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "jump_mp", CANCEL_KIND_CONTACT);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 1, glm::vec2(60, 5),
			glm::vec2(185, 85), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, HitResult().damage(20)
			.meter(18).hit(8, 11).block(8, 7).j_start(3).j_inc(0).j_max(4)
			.anims("light_high", "light_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().ground(150.0, 150.0).air(50.0, 20.0).frames(8), HIT_FLAG_NONE,
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
	script("jump_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);			
			push_function(&Fighter::ENABLE_CANCEL, "jump_mk", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "jump_hp", CANCEL_KIND_CONTACT);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(90, 0), 
				glm::vec2(215, 70), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
				HitResult().damage(40).meter(24).hit(10, 14).block(10, 11).j_start(3).j_inc(0).j_max(4)
				.anims("medium_high", "medium_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().ground(220.0, 220.0).air(70.0, -30.0).frames(11), HIT_FLAG_NONE, 
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
	script("jump_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);		
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(100, 75),
				glm::vec2(280, 135), COLLISION_KIND_AIR, 
				HitResult().damage(50).meter(30).hit(12, 0).crit(12, 17).j_start(3).j_inc(0).j_max(4),
				HIT_STATUS_LAUNCH, HitMove().launch(-20.0, 0.0, 20.0, 5.0), HIT_FLAG_NONE, 
				CRITICAL_CONDITION_COUNTERHIT, HIT_STATUS_FLOAT, HitMove().launch(-20.0, 0.0, 20.0, 5.0),
				HIT_FLAG_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(100, 75), 
				glm::vec2(280, 135), COLLISION_KIND_GROUND, 
				HitResult().damage(50).meter(30).hit(12, 17).block(12, 14)
				.anims("heavy_high", "heavy_high", "high", ""), 
				HIT_STATUS_NORMAL, HitMove().ground(150.0, 150.0).frames(12), HIT_FLAG_NONE, 
				CRITICAL_CONDITION_COUNTERHIT, HIT_STATUS_FLOAT, HitMove(), 
				HIT_FLAG_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(30, -10),
				glm::vec2(180, 90), COLLISION_KIND_AIR,
				HitResult().damage(50).meter(30).hit(12, 0).crit(12, 17).j_start(3).j_inc(0).j_max(4),
				HIT_STATUS_LAUNCH, HitMove().launch(-20.0, 0.0, 20.0, 5.0), HIT_FLAG_NONE,
				CRITICAL_CONDITION_COUNTERHIT, HIT_STATUS_FLOAT, HitMove().launch(-20.0, 0.0, 20.0, 5.0),
				HIT_FLAG_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
				);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(30, -10),
				glm::vec2(180, 90), COLLISION_KIND_GROUND,
				HitResult().damage(50).meter(30).hit(12, 17).block(12, 21)
				.anims("heavy_high", "heavy_high", "high", ""),
				HIT_STATUS_NORMAL, HitMove().ground(150.0, 150.0).frames(12), HIT_FLAG_NONE,
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
	script("jump_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(5, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(60, 100), 
				glm::vec2(190, 190), COLLISION_KIND_GROUND, 
				HitResult().damage(30).meter(18).hit(8, 11).block(8, 7)
				.anims("light_high", "light_high", "high", ""), HIT_STATUS_NORMAL, 
				HitMove().ground(20.0, 20.0).frames(7), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, 
				HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(60, 100),
				glm::vec2(190, 190), COLLISION_KIND_AIR, 
				HitResult().damage(30).meter(18).hit(8, 17).j_start(3).j_inc(0).j_max(4),
				HIT_STATUS_NORMAL, HitMove().air(20.0, 60.0).frames(7), HIT_FLAG_NONE, 
				CRITICAL_CONDITION_PUNISH, HIT_STATUS_LAUNCH, HitMove().launch(25.0, 1.0, 10.0, 5.0),
				HIT_FLAG_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("jump_mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "jump_lk", CANCEL_KIND_CONTACT);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(110, -15), 
				glm::vec2(270, 45), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
				HitResult().damage(40).meter(24).hit(10, 14).block(10, 11).j_start(3).j_inc(0).j_max(4)
				.anims("medium_high", "medium_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().ground(150.0, 150.0).air(20.0, 50.0).frames(5), HIT_FLAG_NONE, 
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(-45, 45), 
				glm::vec2(250, 80), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
				HitResult().damage(40).meter(24).hit(10, 14).block(10, 11).j_start(3).j_inc(0).j_max(4)
				.anims("medium_high", "medium_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().ground(150.0, 150.0).air(20.0, 50.0).frames(5), HIT_FLAG_NONE,
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("jump_hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, 20), 
				glm::vec2(250, 80), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(50).meter(30).hit(12, 17).block(12, 14).j_start(3).j_inc(0).j_max(4)
				.anims("heavy_high", "heavy_high", "high", ""), HIT_STATUS_NORMAL,
				HitMove().ground(150.0, 150.0).air(5.0, 15.0).frames(15), HIT_FLAG_NONE, 
				CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_wait(6, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("forward_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::ENABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::ENABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
		});
		execute_frame(14, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(180, 168),
				glm::vec2(12, 128), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
				HitResult().damage(80).meter(48).hit(12, 27).block(16, 23).j_start(0).j_inc(1).j_max(1)
				.anims("overhead", "overhead", "high", ""), HIT_STATUS_NORMAL, 
				HitMove().ground(14.0, 8.0).air(5.0, 30.0).frames(11).launch(20.0, 1.3, 15.0, 25.0),
				HIT_FLAG_CONTINUE_LAUNCH, CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, DAMAGE_KIND_NORMAL, 
				"", "common_attack_hit_01"
			);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, "special_fireball", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_slide", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_uppercut", CANCEL_KIND_CONTACT);
			push_function(&Fighter::DISABLE_CANCEL, "special_install", CANCEL_KIND_CONTACT);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("stand_mp_crouch_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
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
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(51, 253),
				glm::vec2(200, 70), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(90).meter(54).hit(12, 23).block(16, 16).crit(32, 0).j_start(1)
				.j_inc(1).j_max(1).anims("forced", "", "mid", "mid"),
				HIT_STATUS_NORMAL, HitMove().ground(14.0, 8.0).air(4.0, 31.0).frames(11),
				HIT_FLAG_FORCE_STAND, CRITICAL_CONDITION_DODGE_COUNTER, HIT_STATUS_LAUNCH,
				HitMove().launch(28.0, 1.3, 15.0, 7.0), HIT_FLAG_NONE, HIT_HEIGHT_MID,
				DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
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
	script("backdash_attack", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-215, 0), glm::vec2(-55, 160), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(120, 0), glm::vec2(295, 170), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(300, 190), 
				glm::vec2(30, 130), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
				HitResult().damage(50).meter(30).hit(12, 27).block(12, 20).j_start(0).j_inc(1).j_max(4)
				.anims("medium_high", "medium_high", "high", "high"), HIT_STATUS_NORMAL,
				HitMove().ground(15.0, 10.0).air(5.0, 31.0).frames(8), HIT_FLAG_CONTINUE_LAUNCH, 
				CRITICAL_CONDITION_PUNISH, HIT_STATUS_LAUNCH, HitMove().launch(30.0, 1.3, 15.0, 4.0),
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
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

		});
		execute_frame(19, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(226, 114), 
				glm::vec2(65, -2), COLLISION_KIND_GROUND | COLLISION_KIND_AIR,
				HitResult().damage(120).meter(72).hit(14, 60).block(20, 5).j_start(0).j_inc(1).j_max(4)
				.anims("", "", "high", ""), HIT_STATUS_FLOAT, 
				HitMove().ground(0.0, 5.0).frames(3).launch(-10.0, 0.0, 15.0, 0.0),
				HIT_FLAG_KNOCKDOWN_FACE_DOWN, CRITICAL_CONDITION_NONE, HIT_HEIGHT_HIGH, 
				DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_frame(20, [this]() {
			push_function(&Fighter::RESET_WORLD_RATE);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("advance_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);

		});
		execute_frame(19, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(226, 114),
				glm::vec2(65, -2), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
				HitResult().damage(120).meter(72).hit(14, 60).block(20, 5).j_start(0).j_inc(1).j_max(4)
				.anims("", "", "", "low"), HIT_STATUS_FLOAT,
				HitMove().ground(0.0, 5.0).frames(3).launch(-10.0, 0.0, 15.0, 0.0),
				HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_LOW, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
			);
		});
		execute_frame(20, [this]() {
			push_function(&Fighter::RESET_WORLD_RATE);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("advance_forward", [this]() {
	
	});
	script("advance_back", [this]() {

	});
	script("grab_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
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
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_medium_mid", -1.0);
		});
		execute_frame(17, [this]() {
			push_function(&Fighter::SET_DEFINITE_HITBOX, object_manager->fighter[!id], 
				FIGHTER_STATUS_LAUNCH, HIT_FLAG_NONE, 0, 0, 50.0f, 0, 40.0f, 0, 0, "",
				HitMove().launch(15.0, 1.3, 8.0, 15.0), DAMAGE_KIND_NORMAL, 
				"", "common_attack_hit_01"
			);
		});
	});
	script("throw_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec3(0.0));
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_medium_mid", -1.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END, true);
		});
		execute_frame(39, [this]() {
			push_function(&Fighter::SET_DEFINITE_HITBOX, object_manager->fighter[!id],
				FIGHTER_STATUS_LAUNCH, HIT_FLAG_NONE, 0, 0, 50.0f, 0, 40.0f, 0, 0, "",
				HitMove().launch(15.0, 1.3, 8.0, -35.0), DAMAGE_KIND_NORMAL, 
				"", "common_attack_hit_01"
			);
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
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "jump_hitstun", -1.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, true);
		});
		execute_frame(17, [this]() {
			push_function(&Fighter::SET_DEFINITE_HITBOX, object_manager->fighter[!id],
				FIGHTER_STATUS_THROWN, HIT_FLAG_NONE, 0, 0, 50.0f, 0, 40.0f, 0, 0, "",
				HitMove().launch(-15.0, 1.3, 15.0, 15.0), DAMAGE_KIND_NORMAL, 
				"", "common_attack_hit_01"
			);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, false);
		});
	});
	script("throw_b_air", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec3(0.0));
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "jump_hitstun", -1.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END, true);
		});
		execute_frame(17, [this]() {
			push_function(&Fighter::SET_DEFINITE_HITBOX, object_manager->fighter[!id],
				FIGHTER_STATUS_THROWN, HIT_FLAG_NONE, 0, 0, 50.0f, 0, 40.0f, 0, 0, "",
				HitMove().launch(-15.0, 1.3, 15.0, -15.0), DAMAGE_KIND_NORMAL, 
				"", "common_attack_hit_01"
			);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, false);
		});
	});
	script("grabbed", [this]() {

	});
	script("special_fireball_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-127, -10), glm::vec2(82, 153), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::SET_RATE, 1.0f);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-122, 175), glm::vec2(202, 98), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_AERIAL);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ACTIVATE_PROJECTILE, ROWAN_PROJECTILE_ID_FIREBALL, glm::vec3(0.0));
		});
		execute_frame(41.0, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("special_fireball_punch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::CHANGE_PROJECTILE_STATUS, ROWAN_PROJECTILE_ID_FIREBALL, PROJECTILE_STATUS_MOVE);
		});
	});
	script("special_fireball_kick", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::DEACTIVATE_PROJECTILE, ROWAN_PROJECTILE_ID_FIREBALL);
		});
	});
	script("special_slide", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-136, -10), glm::vec2(159, 72), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_PUSHBOX, 1, glm::vec2(-135, 0), glm::vec2(20, 25));
		});
		execute_frame(12, [this]() {
			if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				push_function(&Fighter::CLEAR_PUSHBOX_ALL);
				push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_CORNER_CROSSUP, true);
			}
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(150, -8), glm::vec2(300, 30), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, -12),
					glm::vec2(250, 30), COLLISION_KIND_GROUND,
					HitResult().damage(70).hit(12, 0).block(12, 19).anims("", "", "low", "low"),
					HIT_STATUS_KNOCKDOWN, HitMove().ground(0.0, 15.0).frames(20), 
					HIT_FLAG_HARD_KNOCKDOWN, CRITICAL_CONDITION_NONE, HIT_HEIGHT_LOW, DAMAGE_KIND_CHIP, 
					"", "common_attack_hit_01"
				);
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(0, -12),
					glm::vec2(250, 30), COLLISION_KIND_GROUND, 
					HitResult().damage(70).meter(42).hit(12, 4).block(12, 16).anims("", "", "low", "low"),
					HIT_STATUS_KNOCKDOWN, HitMove().ground(0.0, 15.0).frames(20), HIT_FLAG_NONE,
					CRITICAL_CONDITION_NONE, HIT_HEIGHT_LOW, DAMAGE_KIND_CHIP, 
					"", "common_attack_hit_01"
				);
			}
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HITBOX, 0);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("special_slide_followup", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-139, -2), glm::vec2(175, 79), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3.0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, -2), glm::vec2(152, 125), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_HIGH);
		});
		execute_frame(8.0, [this]() {
			if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(62, 45), 
					glm::vec2(231, 236), COLLISION_KIND_AIR, HitResult().damage(70).hit(12, 0)
					.j_inc(1).j_max(4), HIT_STATUS_LAUNCH, HitMove().launch(28.0, 2.0, 18.0, 0.5), 
					HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
				);
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(62, 45), 
					glm::vec2(231, 236), COLLISION_KIND_GROUND | COLLISION_KIND_DOWN,
					HitResult().damage(70).hit(12, 0).block(16, 30).anims("", "", "mid", "mid"),
					HIT_STATUS_LAUNCH, HitMove().ground(0.0, 15.0).frames(5)
					.launch(35.0, 2.0, 18.0, 0.25), HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, 
					HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", "common_attack_hit_01"
				);
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(62, 45),
					glm::vec2(231, 236), COLLISION_KIND_AIR, HitResult().damage(70).meter(56).hit(12, 0)
					.j_inc(1).j_max(4), HIT_STATUS_LAUNCH, HitMove().launch(28.0, 2.0, 18.0, 0.5), 
					HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
				);
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, glm::vec2(62, 45),
					glm::vec2(201, 236), COLLISION_KIND_GROUND, HitResult().damage(70).meter(56)
					.hit(12, 0).block(16, 30).anims("", "", "mid", "mid"), HIT_STATUS_LAUNCH,
					HitMove().ground(0.0, 15.0).frames(5).launch(35.0, 2.0, 18.0, 0.25),
					HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", "common_attack_hit_01"
				);
			}
		});
		execute_frame(9.0, [this]() {
			if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-70, 130),
					glm::vec2(200, 255), COLLISION_KIND_AIR, HitResult().damage(70).hit(12, 0).j_inc(1)
					.j_max(4), HIT_STATUS_LAUNCH, HitMove().launch(28.0, 2.0, 18.0, 0.5), HIT_FLAG_NONE,
					CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
				);
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(-70, 130),
					glm::vec2(200, 255), COLLISION_KIND_AIR, HitResult().damage(70).meter(56).hit(12, 0)
					.j_inc(1).j_max(4), HIT_STATUS_LAUNCH, HitMove().launch(28.0, 2.0, 18.0, 0.5), 
					HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_NORMAL, "", "common_attack_hit_01"
				);
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
	script("special_slide_enhanced_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
		});
	});
	script("special_slide_enhanced", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
		});
	});
	script("special_uppercut_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-118, -4), glm::vec2(65, 138), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-98, -4), glm::vec2(171, 65), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			}
		});
		execute_frame(4, [this]() {
			if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-114, -12), glm::vec2(187, 127), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(81, 88), glm::vec2(9, 125), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(220, 0), 
					glm::vec2(160, 120), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
					HitResult().damage(30).scale(0).meter(18).hit(12, 38).block(8, 17).j_min(5).j_max(4)
					.anims("light_mid", "light_mid", "mid", "mid"), HIT_STATUS_NORMAL, 
					HitMove().ground(-10.0, 10.0).air(-3.0, -1.0).frames(5),
					HIT_FLAG_DISABLE_HITSTUN_PARRY, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, 
					DAMAGE_KIND_CHIP, "", "common_attack_hit_01"
				);
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(220, 0),
					glm::vec2(160, 120), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
					HitResult().damage(30).scale(0).hit(12, 38).block(8, 17).j_min(7).j_max(6)
					.anims("light_mid", "light_mid", "mid", "mid"), HIT_STATUS_NORMAL,
					HitMove().ground(-10.0, 10.0).air(-3.0, -1.0).frames(5),
					HIT_FLAG_DISABLE_HITSTUN_PARRY, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, 
					DAMAGE_KIND_CHIP, "", "common_attack_hit_01"
				);
			}
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("special_uppercut", [this]() {
		push_condition("pushbox_check", [this]() { return object_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT]; });
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			if (object_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				push_function(&Fighter::SET_RATE, 0.86f);
			}
		});
		execute_frame(5, [this]() {
			push_function(&Fighter::SET_RATE, 1.0f);
			switch (object_int[FIGHTER_INT_SPECIAL_LEVEL]) {
				case (SPECIAL_LEVEL_L): {
					push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(47, 30),
						glm::vec2(177, 194), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
						HitResult().damage(60).meter(36).hit(14, 0).block(20, 20).j_start(6).j_inc(1)
						.j_max(5).anims("", "", "mid", "mid"), HIT_STATUS_LAUNCH,
						HitMove().ground(0.0, 20.0).frames(5).launch(43.0, 2.0, 22.0, 10.0), 
						HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, "", "common_attack_hit_01"
					);
				} break;
				case (SPECIAL_LEVEL_M): {
					push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(47, 30), 
						glm::vec2(177, 194), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
						HitResult().damage(100).meter(60).hit(14, 0).block(20, 20).j_min(7).j_max(5)
						.anims("", "", "mid", "mid"), HIT_STATUS_LAUNCH, 
						HitMove().ground(0.0, 20.0).frames(5).launch(45.0, 2.0, 22.0, 10.0), 
						HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, 
						"", "common_attack_hit_01"
					);
				} break;
				case (SPECIAL_LEVEL_H): {
					push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(47, 30), 
						glm::vec2(177, 194), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
						HitResult().damage(110).meter(66).hit(14, 0).block(20, 20).j_min(7).j_max(5)
						.anims("", "", "mid", "mid"), HIT_STATUS_LAUNCH,
						HitMove().ground(0.0, 20.0).frames(5).launch(47.0, 2.0, 22.0, 10.0),
						HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, 
						"", "common_attack_hit_01"
					);
				} break;
				case (SPECIAL_LEVEL_EX): {
					push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, glm::vec2(47, 30),
						glm::vec2(177, 194), COLLISION_KIND_GROUND | COLLISION_KIND_AIR, 
						HitResult().damage(90).meter(54).hit(14, 0).block(20, 20).j_start(8)
						.j_inc(1).j_max(7).anims("", "", "mid", "mid"), HIT_STATUS_LAUNCH, 
						HitMove().ground(0.0, 20.0).frames(5).launch(49.0, 2.0, 22.0, 10.0),
						HIT_FLAG_NONE, CRITICAL_CONDITION_NONE, HIT_HEIGHT_MID, DAMAGE_KIND_CHIP, 
						"", "common_attack_hit_01"
					);
				} break;
			}
		});
		execute_frame(8, [this]() {
			push_true("pushbox_check", &Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 500.0));
		});
		execute_frame(12, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("special_uppercut_fall", [this]() {
		push_condition("pushbox_check", [this]() { return object_flag[CHARA_ROWAN_FLAG_BLAZING_UPPER_HIT]; });
		execute_frame(0, [this]() {
			push_true("pushbox_check", &Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 500.0));
			push_false("pushbox_check", &Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-76, 115), glm::vec2(123, 182), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("special_uppercut_land", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("special_install", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
		execute_frame(24, [this]() {
			push_function(&Fighter::SET_FLAG, CHARA_ROWAN_FLAG_INSTALL_ACTIVE, true);
		});
	});
	script("throw_tech", [this]() {

	});
	script("throw_tech_air", [this]() {

	});
	script("stand_hitstun_light_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_light_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_light_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_medium_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_medium_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_medium_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_heavy_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_heavy_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_overhead", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_forced", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_critical", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_light_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_light_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_light_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_medium_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_medium_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_medium_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_heavy_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_heavy_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_heavy_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_overhead", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_forced", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_critical", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hitstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 70.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 140), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_blockstun_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_blockstun_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_blockstun_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_blockstun_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_blockstun_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_blockstun_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 130), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("parry_start_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
		});
	});
	script("parry_start_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
		});
	});
	script("parry_start_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
		});
	});
	script("parry_air_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
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
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_FORCE_CRITICAL, true);
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
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_FORCE_CRITICAL, true);
		});
	});
	script("launch_parry_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 110 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_FORCE_CRITICAL, true);
			push_function(&Fighter::SET_RATE, (anim_kind->length - 2.0) / calc_launch_frames());
		});
	});
	script("thrown", [this]() {

	});
	script("hitstun_float_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("hitstun_float_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
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
			if (object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN]) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			}
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
	script("wakeup_forward_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("wakeup_forward_down", [this]() {
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
	script("wakeup_slow_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("wakeup_slow_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
		});
	});
	script("crumple_up", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("crumple_down", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_lp_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_mp_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hp_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_lk_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_mk_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hk_landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_hitstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(110.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 180), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});
	});
	script("taunt", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-50.0, 0.0), glm::vec2(100.0, 90.0));
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