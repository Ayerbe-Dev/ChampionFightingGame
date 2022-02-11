#include "Atlas.h"

void Atlas::load_move_scripts() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -9, 242 }, GameCoordinate{ 24, 204 }); //head
			new_hurtbox(1, GameCoordinate{ 51,204 }, GameCoordinate{ -45, 3 }); //torso
			new_hurtbox(2, GameCoordinate{ -39, 210 }, GameCoordinate{ -8, 202 }); //extra bit
			new_hurtbox(3, GameCoordinate{ -17, 210 }, GameCoordinate{ -7, 216 }); //extra extra bit
		}
	});
	script("walk_f", [this]() {

	});
	script("walk_b", [this]() {

	});
	script("dash_f", [this]() {

	});
	script("dash_b", [this]() {

	});
	script("crouch_d", [this]() {

	});
	script("crouch", [this]() {

	});
	script("crouch_u", [this]() {

	});
	script("jump_squat", [this]() {

	});
	script("jump", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -9, 242 }, GameCoordinate{ 24, 204 }); //test
			new_hurtbox(1, GameCoordinate{ 51,204 }, GameCoordinate{ -45, 3 });
			new_hurtbox(2, GameCoordinate{ -39, 210 }, GameCoordinate{ -8, 202 });
			new_hurtbox(3, GameCoordinate{ -17, 210 }, GameCoordinate{ -7, 216 });
		}
	});
	script("jump_f", [this]() {

	});
	script("jump_b", [this]() {

	});
	script("fall", [this]() {

	});
	script("stand_block", [this]() {

	});
	script("high_block", [this]() {

	});
	script("crouch_block", [this]() {

	});
	script("jump_block", [this]() {

	});
	script("parry_start", [this]() {

	});
	script("parry_start_air", [this]() {

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

	});
	script("hitstun_parry_air", [this]() {

	});
	script("stand_lp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -52, 32 }, GameCoordinate{ -44, 190 });
			new_hurtbox(1, GameCoordinate{ 44, 182 }, GameCoordinate{ -34, 1980 });
		}
		if (is_excute_frame(2)) {
			new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ -28,183 }, GameCoordinate{ 86, 218 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
		}
		if (is_excute_wait(3)) {
			clear_hitbox_all();
		}
	});
	script("stand_mp", [this]() {

	});
	script("stand_hp", [this]() {

	});
	script("stand_lk", [this]() {

	});
	script("stand_mk", [this]() {

	});
	script("stand_hk", [this]() {

	});
	script("crouch_lp", [this]() {

	});
	script("crouch_mp", [this]() {

	});
	script("crouch_hp", [this]() {

	});
	script("crouch_lk", [this]() {

	});
	script("crouch_mk", [this]() {

	});
	script("crouch_hk", [this]() {

	});
	script("jump_lp", [this]() {

	});
	script("jump_mp", [this]() {

	});
	script("jump_hp", [this]() {

	});
	script("jump_lk", [this]() {

	});
	script("jump_mk", [this]() {

	});
	script("jump_hk", [this]() {

	});
	script("grab", [this]() {

	});
	script("throw_f", [this]() {

	});
	script("throw_b", [this]() {

	});
	script("grab_air", [this]() {

	});
	script("throw_f_air", [this]() {

	});
	script("throw_b_air", [this]() {

	});
	script("stand_hitstun_l", [this]() {

	});
	script("stand_hitstun_m", [this]() {

	});
	script("stand_hitstun_h", [this]() {

	});
	script("crouch_hitstun_l", [this]() {

	});
	script("crouch_hitstun_m", [this]() {

	});
	script("crouch_hitstun_h", [this]() {

	});
	script("jump_hitstun", [this]() {

	});
	script("launch_start", [this]() {

	});
	script("launch", [this]() {

	});
	script("knockdown_start", [this]() {

	});
	script("knockdown_wait", [this]() {

	});
	script("wakeup", [this]() {

	});
	script("crumple", [this]() {

	});
	script("landing", [this]() {

	});
	script("landing_hitstun", [this]() {

	});
}