#include "Rowan.h"
#include "RowanFireball.h"
#include "RowanFireballConstants.h"
#include "SoundConstants.h"
#include <glm/gtx/string_cast.hpp>
#include "RenderManager.h"

void Rowan::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::SET_RATE, 0.5);
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
			
	});
	script("walk_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("walk_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("dash_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("dash_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("dash_air_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(196, 107), glm::vec2(61, 28), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(142, 37), glm::vec2(13, -14), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(16, 39), glm::vec2(186, 85), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(92, 85), glm::vec2(317, 138), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("dash_air_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(118, -16), glm::vec2(-100, 47), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(60, 41), glm::vec2(-81, 152), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-171, 47), glm::vec2(-88, 171), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-100, 22), glm::vec2(-6, 67), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(23, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-153, 53), glm::vec2(71, 118), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-119, 118), glm::vec2(31, 190), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_d", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX_ALL);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_u", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
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
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-76, 115), glm::vec2(123, 182), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-90, 49), glm::vec2(106, 161), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-98, 17), glm::vec2(8, 62), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-90, 49), glm::vec2(106, 161), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("fall", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-76, 115), glm::vec2(123, 182), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("turn_stand", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-70.0, 0.0), glm::vec2(30.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
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
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -42, 0 }, glm::vec2{ 62, 38 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ -29, 39 }, glm::vec2{ 33, 141 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ 17, 82 }, glm::vec2{ 54, 104 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_block_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_block", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("high_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_blockstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("parry_start_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			});
		});
	script("parry_start_mid", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("parry_start_low", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("parry_air_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
		});
		execute_wait(7, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
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
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("hitstun_parry_air_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
		});
	});
	script("launch_parry_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 110 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, true);
			push_function(&Fighter::SET_INT, FIGHTER_INT_PARRY_HEIGHT, PARRY_HEIGHT_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_PARRY_ACTIVE, false);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, true);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_PUNISH, true);
			push_function(&Fighter::SET_RATE, (anim_kind->length - 2.0) / calc_launch_frames());
		});
	});
	script("stand_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::PLAY_VC, "rowan_attack_01");
			push_function(&Fighter::NEW_EFFECT, "flame", glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.4), glm::vec4(0.0), "HaveL", glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.0), glm::vec4(0.0), &fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES]);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 120 }, glm::vec2{ 115, 145 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2{ 5,145 }, glm::vec2{ 240, 165 });
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_GRAB);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 4, glm::vec2(151, 164), glm::vec2(1, 132), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, glm::vec2(5, 145), glm::vec2(200, 165), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_MID, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 14, /*Blockstun*/ 12, /*Hit Pushback*/ 10.0, /*Block Pushback*/ 10.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 0.0, 0.0, 0.0, 0.0);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_LP);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_LK);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_MP);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_CHP);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_MP);
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_CHP);
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 4);
		});
	});
	script("stand_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_236P);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2{ -130, 0 }, glm::vec2{ 0, 50 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2{ 0, 0 }, glm::vec2{ 125, 80 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2{ 35,115 }, glm::vec2{ 240, 135 });
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_PARRY_START);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 105, 160 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2{ 35, 110 }, glm::vec2{ 155, 150 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 4.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, glm::vec2(103, 165), glm::vec2(213, 113), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 18, /*Blockstun*/ 11, /*Hit Pushback*/ 10.0, /*Block Pushback*/ 4.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 2, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_236P);
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 3);
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_HP);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_HK);
		});
	});
	script("stand_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_236P);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-105, 0), glm::vec2(35, 90), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(30, 55), glm::vec2(95, 95), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2{ -35, 35 }, glm::vec2{ 65, 160 }, HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_BLOCKBOX, glm::vec2(217, 168), glm::vec2(12, 128));
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_ADVANCE);
		});
		execute_frame(5, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(30, 0), glm::vec2(107, 50), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(85, 167), glm::vec2(0, 108), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 50.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, glm::vec2(180, 168), glm::vec2(12, 128), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 23, /*Blockstun*/ 23, /*Hit Pushback*/ 14.0, /*Block Pushback*/ 8.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 2, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 20.0, 1.3, 15.0, 25.0);
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
	});
	script("stand_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_236P);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(45, 185), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-100, 0), glm::vec2(120, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-100, 35), glm::vec2(100, 80), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_GRAB);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-100, 35), glm::vec2(100, 80), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-70, 70), glm::vec2(25, 185), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-35, 0), glm::vec2(220, 60), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-45, 40), glm::vec2(65, 155), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2(30, 45), glm::vec2(160, 95), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 4, glm::vec2(20, 70), glm::vec2(130, 140), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, glm::vec2(250, 57), glm::vec2(13, 4), HIT_KIND_GROUND, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_MID, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 13, /*Blockstun*/ 10, /*Hit Pushback*/ 10.0, /*Block Pushback*/ 10.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
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
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-10, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_PARRY_START);
			});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(25, 60), glm::vec2(150, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(25, 60), glm::vec2(180, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(7, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(25, 105), glm::vec2(260, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 8.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, glm::vec2(30, 105), glm::vec2(310, 155), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 15, /*Blockstun*/ 10, /*Hit Pushback*/ 19.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 2, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_DASH_F);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_DASH_F);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
	});
	script("stand_hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-91, 44), glm::vec2(180, -8), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(15, 25), glm::vec2(155, 160), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_PARRY_START);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-130.0, 0.0), glm::vec2(-30.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-191, 44), glm::vec2(80, -8), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-85, 25), glm::vec2(55, 160), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-80.0, 0.0), glm::vec2(20.0, 160.0));
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(50.0, 0.0), glm::vec2(150.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(160, 170), glm::vec2(-25, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
		execute_frame(12, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(70.0, 0.0), glm::vec2(170.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(65, 0), glm::vec2(248, 110), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(335, 170), glm::vec2(150, 100), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 50.0, /*Chip Damage*/ 25.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, glm::vec2(370, 190), glm::vec2(180, 130), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 27, /*Blockstun*/ 20, /*Hit Pushback*/ 15.0, /*Block Pushback*/ 10.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_PUNISH, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 30.000000, 0.000000, 0.000000, 4.000000);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
		execute_wait(9, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(50.0, 0.0), glm::vec2(150.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(160, 170), glm::vec2(-25, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_236P);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_GRAB);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(43, 108), glm::vec2(136, 79), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 30.0, /*Chip Damage*/ 3.0, /*Damage Scale*/ 1, /*Meter Gain*/ 20.0, glm::vec2(51, 112), glm::vec2(199, 82), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_MID, /*Hitlag*/ 6, /*Blocklag*/ 6, /*Hitstun*/ 15, /*Blockstun*/ 12, /*Hit Pushback*/ 11.0, /*Block Pushback*/ 13.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_CLP);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_CLK);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_236P);
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
	});
	script("crouch_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_236P);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-100, 0), glm::vec2(165, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(75, 20), glm::vec2(-115, 100), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_PARRY_START);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(125, 20), glm::vec2(-75, 100), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(260, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 20), glm::vec2(215, 90), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(175, 35), glm::vec2(270, 120), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 4.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, glm::vec2(145, 40), glm::vec2(320, 120), HIT_KIND_GROUND, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 22, /*Blockstun*/ 17, /*Hit Pushback*/ 8.0, /*Block Pushback*/ 8.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 4.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, glm::vec2(255, 120), glm::vec2(355, 165), HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 10, /*Blocklag*/ 0, /*Hitstun*/ 22, /*Blockstun*/ 0, /*Hit Pushback*/ 24.0, /*Block Pushback*/ 8.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 3, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(4, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 2);
		});
		execute_frame(23, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_236P);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(146, 58), glm::vec2(-67, -7), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-23, 114), glm::vec2(79, 8), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_PARRY_START);
			});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(185, -9), glm::vec2(-97, 48), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-53, 114), glm::vec2(49, 8), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX_ALL);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-31, 0), glm::vec2(132, 113), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 50.0, /*Chip Damage*/ 5.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, glm::vec2(51, 253), glm::vec2(149, 70), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 22, /*Blockstun*/ 16, /*Hit Pushback*/ 6.0, /*Block Pushback*/ 4.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_JUMP_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ true, /*Launch Info*/ 28.0, 0.0, 0.0, 7.0);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(22, 158), glm::vec2(129, 64), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		}); 
		execute_wait(1, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_frame(25, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("crouch_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(190, 36), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, 26), glm::vec2(75, 93), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_GRAB);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(235, 36), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 30.0, /*Chip Damage*/ 3.0, /*Damage Scale*/ 2, /*Meter Gain*/ 24.0, glm::vec2(230, 36), glm::vec2(129, -7), HIT_KIND_GROUND, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_LOW, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 11, /*Blockstun*/ 13, /*Hit Pushback*/ 15.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 0, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_CLP);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_CLK);
		});
		execute_wait(1, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(180, 36), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_236P);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_214K);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_623P);
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(190, 36), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, 26), glm::vec2(75, 93), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_PARRY_START);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, glm::vec2(293, -10), glm::vec2(14, 42), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_LOW, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 19, /*Blockstun*/ 13, /*Hit Pushback*/ 14.0, /*Block Pushback*/ 8.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 0, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("crouch_hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-104, -7), glm::vec2(190, 36), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, 26), glm::vec2(75, 93), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(2, [this]() {
			push_function(&Fighter::DISABLE_CANCEL, CANCEL_CAT_ALL, CANCEL_KIND_PARRY_START);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 50.0, /*Chip Damage*/ 5.0, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, glm::vec2(-15, 0), glm::vec2(250, 50), HIT_KIND_GROUND, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_LOW, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 0, /*Blockstun*/ 8, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 20.0, /*Hit Status*/ HIT_STATUS_KNOCKDOWN, /*Counterhit Status*/ HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 0.0, 0.0, 0.0, 0.0);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ENABLE_COUNTERHIT, false);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});

	});
	script("jump_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_VERTICAL_PUSHBACK, true);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_MP);
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-105, -30), glm::vec2(65, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-45, 40), glm::vec2(135, 90), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-80, 75), glm::vec2(145, 125), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-95, 15), glm::vec2(100, 80), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-55, 80), glm::vec2(85, 125), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::CLEAR_HURTBOX, 2);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 30.0, /*Chip Damage*/ 3.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, glm::vec2(60, 15), glm::vec2(185, 65), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 16, /*Blockstun*/ 13, /*Hit Pushback*/ 15.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 3, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(8, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
	});
	script("jump_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_VERTICAL_PUSHBACK, true);
			push_function(&Fighter::ENABLE_CANCEL, CANCEL_CAT_CONTACT, CANCEL_KIND_HP);
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-86, 34), glm::vec2(86, 140), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-73, 34), glm::vec2(136, 103), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(110, 18), glm::vec2(175, 62), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 4.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, glm::vec2(90, 5), glm::vec2(215, 50), HIT_KIND_GROUND, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 21, /*Blockstun*/ 26, /*Hit Pushback*/ 22.0, /*Block Pushback*/ 22.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 4.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, glm::vec2(90, 5), glm::vec2(215, 50), HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 21, /*Blockstun*/ 26, /*Hit Pushback*/ 18.0, /*Block Pushback*/ 18.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(8, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::DISABLE_ALL_CANCELS);
		});
		execute_frame(22, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-86, 34), glm::vec2(86, 140), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-75, 28), glm::vec2(116, 130), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-83, 74), glm::vec2(316, 120), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 50.0, /*Chip Damage*/ 7.5, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, glm::vec2(-5, 102), glm::vec2(277, 178), HIT_KIND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 25, /*Blockstun*/ 25, /*Hit Pushback*/ 15.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 5, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ -20.0, 0.0, 0.0, 5.0);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, /*Damage*/ 50.0, /*Chip Damage*/ 7.5, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, glm::vec2(105, 74), glm::vec2(298, 137), HIT_KIND_GROUND, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 25, /*Blockstun*/ 25, /*Hit Pushback*/ 15.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 5, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ -20.0, 0.0, 0.0, 5.0);
		});
		execute_wait(2, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("jump_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(4, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 30.0, /*Chip Damage*/ 4.5, /*Damage Scale*/ 2, /*Meter Gain*/ 24.0, glm::vec2(87, 129), glm::vec2(194, 225), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 8, /*Blocklag*/ 8, /*Hitstun*/ 13, /*Blockstun*/ 17, /*Hit Pushback*/ 20.0, /*Block Pushback*/ 20.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("jump_mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(6, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, glm::vec2(-103, 59), glm::vec2(55, 10), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 23, /*Blockstun*/ 18, /*Hit Pushback*/ 15.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, /*Damage*/ 40.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 32.0, glm::vec2(-46, 90), glm::vec2(248, 43), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 10, /*Blocklag*/ 12, /*Hitstun*/ 23, /*Blockstun*/ 18, /*Hit Pushback*/ 15.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 1, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 10.0, 0.0, 0.0, 1.0);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-71, 48), glm::vec2(185, 90), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_wait(5, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_HURTBOX, 1);
		});
	});
	script("jump_hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(9, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 50.0, /*Chip Damage*/ 7.5, /*Damage Scale*/ 1, /*Meter Gain*/ 40.0, glm::vec2(250, 60), glm::vec2(-6, 114), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 15, /*Blockstun*/ 11, /*Hit Pushback*/ 15.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 1, /*Juggle Increase*/ 1, /*Juggle Max*/ 2, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 0.000000, 0.000000, 0.000000, 0.000000);
		});
		execute_wait(6, [this]() {
			push_function(&Fighter::CLEAR_HITBOX_ALL);
		});
	});
	script("forward_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
	});
	script("advance_high", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
		execute_frame(19, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 100.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ -5, /*Meter Gain*/ 80.0, glm::vec2(226, 114), glm::vec2(65, -2), HIT_KIND_GROUND, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_HIGH, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 180, /*Blockstun*/ 5, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 5.0, /*Hit Status*/ HIT_STATUS_FLOAT, /*Counterhit Status*/ HIT_STATUS_FLOAT, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 0, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_CHIP, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 0.0, 0.0, 0.0, 0.0);
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
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 100.0, /*Chip Damage*/ 10.0, /*Damage Scale*/ -5, /*Meter Gain*/ 80.0, glm::vec2(226, 114), glm::vec2(65, -2), HIT_KIND_GROUND, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_LOW, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 180, /*Blockstun*/ 5, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 5.0, /*Hit Status*/ HIT_STATUS_FLOAT, /*Counterhit Status*/ HIT_STATUS_FLOAT, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 0, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_CHIP, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 0.0, 0.0, 0.0, 0.0);
		});
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
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_GRABBOX, 0, glm::vec2(0, 55), glm::vec2(150, 100), GRABBOX_KIND_NORMAL, HIT_KIND_GROUND, FIGHTER_STATUS_THROW, FIGHTER_STATUS_GRABBED);
		});
		execute_wait(3, [this]() {
			push_function(&Fighter::CLEAR_GRABBOX_ALL);
		});
	});
	script("throw_f", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec2(0.0), 3);
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
		});
		execute_frame(24, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 50.0, 15.0, 8.0, 1.3, 15.0);
		});
	});
	script("throw_b", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec2(0.0), 3);
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
		});
		execute_frame(40, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 50.0, -35.0, 8.0, 1.3, 15.0);
		});
	});
	script("grab_air", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-105, -30), glm::vec2(65, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-45, 40), glm::vec2(135, 90), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-80, 75), glm::vec2(145, 125), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3, [this]() {
			push_function(&Fighter::NEW_GRABBOX, 0, glm::vec2(-70, 55), glm::vec2(100, 100), GRABBOX_KIND_HITSTUN, HIT_KIND_AIR, FIGHTER_STATUS_THROW_AIR, FIGHTER_STATUS_GRABBED);
		});
	});
	script("throw_f_air", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec2(0.0), 3);
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
			push_function(&Fighter::GRAB_OPPONENT, "Throw", "Bust", glm::vec2(0.0), 3);
			push_function(&Fighter::CHANGE_OPPONENT_ANIM, "stand_hitstun_m", -1.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, true);
		});
		execute_frame(17, [this]() {
			push_function(&Fighter::THROW_OPPONENT, 50.0, -15.0, -15.0, 0.0, -15.0);
			push_function(&Fighter::SET_FLAG, FIGHTER_FLAG_DISABLE_AIR_GENERAL, false);
		});
	});
	script("special_fireball_start", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
		execute_frame(8, [this]() {
			push_function(&Fighter::ACTIVATE_PROJECTILE, 0, glm::vec3(0.0));
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
		execute_frame(28, [this]() {
			push_condition([this]() {
				return fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H
				|| fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX;
			});
			push_true(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_CANCEL_RECOVERY, true);
		});
		execute_frame(31, [this]() {
			push_condition([this]() {
				return fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M;
			});
			push_true(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_CANCEL_RECOVERY, true);
		});
		execute_frame(34, [this]() {
			push_condition([this]() {
				return fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L;
			});
			push_true(&Fighter::SET_FLAG, FIGHTER_FLAG_ALLOW_CANCEL_RECOVERY, true);
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
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-136, -10), glm::vec2(159, 72), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
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
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(150, -8), glm::vec2(300, 30), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, glm::vec2(0, -12), glm::vec2(250, 30), HIT_KIND_GROUND, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_LOW, /*Hitlag*/ 12, /*Blocklag*/ 12, /*Hitstun*/ 40, /*Blockstun*/ 19, /*Hit Pushback*/ 0.000000, /*Block Pushback*/ 15.000000, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 0, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ true, /*Launch Init Y Speed*/ 0.000000, /*Launch Gravity*/ 0.000000, /*Launch Max Fall Speed*/ 0.000000, /*Launch X Speed*/ 0.000000);
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 56.0, glm::vec2(0, -12), glm::vec2(250, 30), HIT_KIND_GROUND, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_LOW, /*Hitlag*/ 12, /*Blocklag*/ 12, /*Hitstun*/ 40, /*Blockstun*/ 16, /*Hit Pushback*/ 0.000000, /*Block Pushback*/ 15.000000, /*Hit Status*/ HIT_STATUS_KNOCKDOWN, /*Counterhit Status*/ HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 0, /*Juggle Max*/ 0, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ true, /*Launch Init Y Speed*/ 0.000000, /*Launch Gravity*/ 0.000000, /*Launch Max Fall Speed*/ 0.000000, /*Launch X Speed*/ 0.000000);
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
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-139, -2), glm::vec2(175, 79), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
		execute_frame(3.0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, -2), glm::vec2(152, 125), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_HIGH);
		});
		execute_frame(8.0, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_EX) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, glm::vec2(62, 45), glm::vec2(201, 236), HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 52, /*Blockstun*/ 30, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Init Y Speed*/ 38.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch X Speed*/ 4.0);
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, glm::vec2(62, 45), glm::vec2(201, 236), HIT_KIND_GROUND, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 52, /*Blockstun*/ 30, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Init Y Speed*/ 45.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch X Speed*/ 3.5);
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 56.0, glm::vec2(62, 45), glm::vec2(201, 236), HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 52, /*Blockstun*/ 30, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Init Y Speed*/ 38.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch X Speed*/ 4.0);
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 1, /*Multihit ID*/ 0, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 56.0, glm::vec2(62, 45), glm::vec2(201, 236), HIT_KIND_GROUND, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 52, /*Blockstun*/ 30, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Init Y Speed*/ 38.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch X Speed*/ 4.0);
			}
		});
		execute_frame(9.0, [this]() {
			push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 70.0, /*Chip Damage*/ 14.0, /*Damage Scale*/ 1, /*Meter Gain*/ 56.0, glm::vec2(202, 129), glm::vec2(-72, 256), HIT_KIND_AIR, ATTACK_LEVEL_MEDIUM, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 16, /*Hitstun*/ 52, /*Blockstun*/ 30, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 15.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 0, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_NORMAL, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Init Y Speed*/ 45.0, /*Launch Gravity*/ 2.0, /*Launch Max Fall Speed*/ 18.0, /*Launch X Speed*/ 3.5);
			push_function(&Fighter::CLEAR_HITBOX, 1);
		});
		execute_frame(14.0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-73, -2), glm::vec2(152, 200), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
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
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-118, -4), glm::vec2(65, 138), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-98, -4), glm::vec2(171, 65), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			}
		});
		execute_frame(4, [this]() {
			if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_L) {
				push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-114, -12), glm::vec2(187, 127), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(81, 88), glm::vec2(9, 125), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 24.0, glm::vec2(220, 0), glm::vec2(160, 120), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 8, /*Hitstun*/ 38, /*Blockstun*/ 17, /*Hit Pushback*/ -10.0, /*Block Pushback*/ 10.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 5, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_CONTINUE, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ true, /*Launch Init Y Speed*/ 0.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch X Speed*/ 0.0);
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 30.0, /*Chip Damage*/ 6.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, glm::vec2(220, 0), glm::vec2(160, 120), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_LIGHT, ATTACK_HEIGHT_MID, /*Hitlag*/ 12, /*Blocklag*/ 8, /*Hitstun*/ 38, /*Blockstun*/ 17, /*Hit Pushback*/ -10.0, /*Block Pushback*/ 10.0, /*Hit Status*/ HIT_STATUS_NORMAL, /*Counterhit Status*/ HIT_STATUS_NORMAL, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 5, /*Juggle Increase*/ 1, /*Juggle Max*/ 7, CLANK_KIND_CONTINUE, DAMAGE_KIND_NORMAL, /*Continue Launch*/ false, /*Disable Hitstun Parry*/ true, /*Launch Init Y Speed*/ 0.0, /*Launch Gravity*/ 0.0, /*Launch Max Fall Speed*/ 0.0, /*Launch X Speed*/ 0.0);
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
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 60.0, /*Chip Damage*/ 12.0, /*Damage Scale*/ 1, /*Meter Gain*/ 48.0, glm::vec2(47, 30), glm::vec2(177, 194), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 20, /*Blockstun*/ 20, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 20.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 6, /*Juggle Increase*/ 1, /*Juggle Max*/ 5, CLANK_KIND_NORMAL, DAMAGE_KIND_CHIP_KO, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 43.0, 2.0, 22.0, 10.0);
			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_M) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 100.0, /*Chip Damage*/ 20.0, /*Damage Scale*/ 1, /*Meter Gain*/ 80.0, glm::vec2(47, 30), glm::vec2(177, 194), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 20, /*Blockstun*/ 20, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 20.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 5, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_CHIP_KO, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 45.0, 2.0, 22.0, 10.0);
			}
			else if (fighter_int[FIGHTER_INT_SPECIAL_LEVEL] == SPECIAL_LEVEL_H) {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 110.0, /*Chip Damage*/ 22.0, /*Damage Scale*/ 1, /*Meter Gain*/ 88.0, glm::vec2(47, 30), glm::vec2(177, 194), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 20, /*Blockstun*/ 20, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 20.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 5, /*Juggle Increase*/ 1, /*Juggle Max*/ 4, CLANK_KIND_NORMAL, DAMAGE_KIND_CHIP_KO, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 47.0, 2.0, 22.0, 10.0);
			}
			else {
				push_function(&Fighter::NEW_HITBOX, /*ID*/ 0, /*Multihit ID*/ 0, /*Damage*/ 90.0, /*Chip Damage*/ 18.0, /*Damage Scale*/ 1, /*Meter Gain*/ 0.0, glm::vec2(47, 30), glm::vec2(177, 194), HIT_KIND_GROUND | HIT_KIND_AIR, ATTACK_LEVEL_HEAVY, ATTACK_HEIGHT_MID, /*Hitlag*/ 14, /*Blocklag*/ 20, /*Hitstun*/ 20, /*Blockstun*/ 20, /*Hit Pushback*/ 0.0, /*Block Pushback*/ 20.0, /*Hit Status*/ HIT_STATUS_LAUNCH, /*Counterhit Status*/ HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_COUNTER, /*Juggle Start*/ 6, /*Juggle Increase*/ 1, /*Juggle Max*/ 8, CLANK_KIND_NORMAL, DAMAGE_KIND_CHIP_KO, /*Continue Launch*/ true, /*Disable Hitstun Parry*/ false, /*Launch Info*/ 49.0, 2.0, 22.0, 10.0);
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
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-76, 115), glm::vec2(123, 182), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-48, 25), glm::vec2(66, 115), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("special_uppercut_land", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("special_upkick_l", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
	});
	script("special_upkick_m", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
	});
	script("special_upkick_h", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
	});
	script("special_upkick_ex", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
		});
	});
	script("clank", [this]() {

	});
	script("throw_tech", [this]() {

	});
	script("throw_tech_air", [this]() {

	});
	script("stand_hitstun_l", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_m", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("stand_hitstun_h", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_l", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_m", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("crouch_hitstun_h", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("jump_hitstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("thrown", [this]() {

	});
	script("hitstun_float", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("launch_start", [this]() {
		

	});
	script("launch", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("launch_hkd", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 60.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-74, -11), glm::vec2(91, 136), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
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
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_lp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_mp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_hp", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_lk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_mk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_hk", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("landing_hitstun", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(120, 60), glm::vec2(-120, 0), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-50, 130), glm::vec2(55, 40), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("taunt", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-80, 0), glm::vec2(120, 150), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(-30, 150), glm::vec2(70, 180), HURTBOX_KIND_NORMAL, false, INTANGIBLE_KIND_NONE);
		});
	});
	script("intro", [this]() {
		
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
