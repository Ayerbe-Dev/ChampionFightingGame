#include "Eric.h"
#include "EricFireball.h"
#include "EricFireballConstants.h"

void Eric::load_move_list() {

}

void Eric::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		execute_frame(0, [this]() {
			push_function(&Fighter::NEW_PUSHBOX, 0, glm::vec2(-30.0, 0.0), glm::vec2(70.0, 160.0));
			push_function(&Fighter::NEW_HURTBOX, 0, glm::vec2(-130, 0), glm::vec2(0, 50), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 1, glm::vec2(0, 0), glm::vec2(125, 80), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 2, glm::vec2(-35, 35), glm::vec2(65, 160), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
			push_function(&Fighter::NEW_HURTBOX, 3, glm::vec2(35, 120), glm::vec2(115, 145), HURTBOX_KIND_NORMAL, 0, INTANGIBLE_KIND_NONE);
		});

	});
	script("walk_f", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("walk_b", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("dash_f", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("dash_b", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_d", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_u", [this]() {
		execute_frame(0, [this]() {
		});
	});
	script("jump_squat", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("jump", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("jump_f", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("jump_b", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("fall", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("turn_stand", [this]() {
		execute_frame(0, [this]() {
;
		});
	});
	script("turn_crouch", [this]() {
		execute_frame(0, [this]() {

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
	script("jump_blockstun", [this]() {
		execute_frame(0, [this]() {
;
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
	script("stand_lp", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("stand_mp", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("stand_hp", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("stand_lk", [this]() {

	});
	script("stand_mk", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("stand_hk", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_lp", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_mp", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_hp", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_lk", [this]() {

	});
	script("crouch_mk", [this]() {

	});
	script("crouch_hk", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("jump_lp", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("jump_mp", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("jump_hp", [this]() {
		execute_frame(0, [this]() {

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
	script("stand_hitstun_l", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("stand_hitstun_m", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("stand_hitstun_h", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_hitstun_l", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_hitstun_m", [this]() {
		execute_frame(0, [this]() {

		});
	});
	script("crouch_hitstun_h", [this]() {
		execute_frame(0, [this]() {

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

		});
	});
	script("landing_hitstun", [this]() {
		execute_frame(0, [this]() {

		});
	});
}