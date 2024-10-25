#pragma warning(disable : 4996)
#include "Fighter.h"
#include "Animation.h"
#include "ParamAccessor.h"
#include <fstream>
#include <cmath>
#include <glm/gtx/quaternion.hpp>
#include "GLM Helpers.h"
#include "utils.h"
#include <array>

bool Fighter::common_ground_status_act() {
	unsigned int stick_dir = get_stick_dir();
	if (stick_dir < 4) {
		object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
	}
	else {
		object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
	}
	switch (stick_dir) {
		case (1):
		case (2):
		case (3): {
			if (status_kind != FIGHTER_STATUS_TURN
			&& status_kind != FIGHTER_STATUS_CROUCH) {
				if (move_list[FIGHTER_CONTEXT_GROUND].is_curr_move_recover_crouching(this)) {
					if (status_kind != FIGHTER_STATUS_ATTACK) {
						change_status(FIGHTER_STATUS_CROUCH);
					}
				}
				else {
					change_status(FIGHTER_STATUS_CROUCH_D);
				}
			}
		} break;
		case (4): {
			change_status(FIGHTER_STATUS_WALK_B);
		} break;
		case (5):
		default: {
	
		} break;
		case (6): {
			change_status(FIGHTER_STATUS_WALK_F);
		} break;
	}
	check_movelist_inputs();
	return object_flag[FIGHTER_FLAG_STATUS_CHANGED];
}

void Fighter::common_air_status_general() {
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
		apply_gravity(object_float[FIGHTER_FLOAT_CURRENT_GRAVITY], object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX]);
		add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
	}
}

void Fighter::status_none() {
	change_status(FIGHTER_STATUS_WAIT);
}

void Fighter::enter_status_none() {

}

void Fighter::exit_status_none() {

}

void Fighter::status_wait() {
	if (!object_flag[FIGHTER_FLAG_ROUND_START]) {
		common_ground_status_act();
	}
}


void Fighter::enter_status_wait() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NON_ACTION;
	object_string[FIGHTER_STRING_MOVE_KIND] = "";
	set_pos_y(0);
	set_pos_z(0);
	change_anim("wait");
	change_script("wait");

	change_context(FIGHTER_CONTEXT_GROUND);
	object_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
	if (!object_flag[FIGHTER_FLAG_ROUND_START]) {
		common_ground_status_act();
	}
}

void Fighter::exit_status_wait() {}

void Fighter::status_walk_f() {
	Fighter* that = object_manager->fighter[!id];
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0, 0));

}

void Fighter::enter_status_walk_f() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NON_ACTION;
	object_string[FIGHTER_STRING_MOVE_KIND] = "";
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("walk_f_speed") * facing_dir;
	add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0, 0));
	change_anim("walk_f");
	change_script("walk_f");
}

void Fighter::exit_status_walk_f() {
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
}

void Fighter::status_walk_b() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	if (object_flag[FIGHTER_FLAG_PROX_GUARD]) {
		if (get_anim() != "stand_block") {
			if (get_anim() == "stand_block_start") {
				if (anim_end) {
					change_anim("stand_block");
					change_script("stand_block");
				}
			}
			else {
				change_anim("stand_block_start");
				change_script("stand_block_start");
			}
		}
	}
	else {
		if (get_anim() != "walk_b") {
			change_anim("walk_b");
			change_script("walk_b");
		}
		add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0, 0));
	}
}

void Fighter::enter_status_walk_b() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NON_ACTION;
	object_string[FIGHTER_STRING_MOVE_KIND] = "";
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("walk_b_speed") * -facing_dir;
	add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0, 0));
	change_anim("walk_b");
	change_script("walk_b");
}

void Fighter::exit_status_walk_b() {
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
}

void Fighter::status_dash_f() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_param_int("dash_f_start_frame");
	int max_frame = get_param_int("dash_f_stop_frame");

	if (frame >= min_frame && frame < max_frame) {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("dash_f_speed") * facing_dir;
	}
	else if (frame < max_frame) {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = (get_param_float("walk_f_speed") + get_param_float("dash_f_speed")) * facing_dir / 2;
	}
	else {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("walk_f_speed") * facing_dir;
	}
	add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0, 0));

	if (is_enable_cancel("dash_b") && get_stick_dir(false) == 4) {
		switch (get_param_int("dash_cancel_kind")) {
			case (DASH_CANCEL_KIND_INFINITE): {
				object_string[FIGHTER_STRING_MOVE_KIND] = "dash_b";
				change_status(FIGHTER_STATUS_DASH_B, false);
				return;
			} break;
			case (DASH_CANCEL_KIND_WHIFF_PUNISH_OFFENSIVE): {
				if (!object_flag[FIGHTER_FLAG_DASH_CANCEL]) {
					object_string[FIGHTER_STRING_MOVE_KIND] = "dash_b";
					object_flag[FIGHTER_FLAG_DASH_CANCEL] = true;
					object_int[FIGHTER_INT_44_STEP] = 0;
					object_int[FIGHTER_INT_44_TIMER] = 0;
					change_status(FIGHTER_STATUS_DASH_B, false);
					return;
				}
			} break;
			case (DASH_CANCEL_KIND_WHIFF_PUNISH_DEFENSIVE):
			case (DASH_CANCEL_KIND_NONE):
			default: {

			} break;
		}
	}
}

void Fighter::enter_status_dash_f() {
	change_anim("dash_f");
	change_script("dash_f");
}

void Fighter::exit_status_dash_f() {
	object_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
}

void Fighter::status_dash_b() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_param_int("dash_b_start_frame");
	int max_frame = get_param_int("dash_b_stop_frame");

	if (frame >= min_frame && frame < max_frame) {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("dash_b_speed") * facing_dir * -1;
	}
	else if (frame < max_frame) {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = (get_param_float("walk_b_speed") + get_param_float("dash_b_speed")) * facing_dir / -2;
	}
	else {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("walk_b_speed") * -facing_dir;
	}
	add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0, 0));
}

void Fighter::enter_status_dash_b() {
	object_flag[FIGHTER_FLAG_ENABLE_DODGE_COUNTER] = true;
	change_anim("dash_b");
	change_script("dash_b");
}

void Fighter::exit_status_dash_b() {
	object_flag[FIGHTER_FLAG_ENABLE_DODGE_COUNTER] = false;
	object_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
}

void Fighter::status_dash_air() {
	if (check_landing()) {
		return;
	}
	if (is_status_end()) {
		return;
	}
	int min_frame = get_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_param_int("dash_f_maintain_speed_frame");

	if (frame < max_frame) {
		object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 0.0;
	}
	else {
		if (object_float[BATTLE_OBJECT_FLOAT_X_SPEED] > get_param_float("jump_x_speed")) {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = clampf(get_param_float("jump_x_speed"), object_float[BATTLE_OBJECT_FLOAT_X_SPEED] * 0.95, object_float[BATTLE_OBJECT_FLOAT_X_SPEED]);
		}
	}
	common_air_status_general();
}

void Fighter::enter_status_dash_air() {
	if (object_int[FIGHTER_INT_DASH_AIR_DIR] == 1) {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("dash_f_speed") * facing_dir;
		object_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_F;
		change_anim("dash_air_f");
		change_script("dash_air_f");
	}
	if (object_int[FIGHTER_INT_DASH_AIR_DIR] == 2) {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("dash_b_speed") * -facing_dir;
		object_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_B;
		change_anim("dash_air_b");
		change_script("dash_air_b");
	}
	object_flag[FIGHTER_FLAG_USED_AIRDASH] = true;
}

void Fighter::exit_status_dash_air() {

}


void Fighter::status_crouchd() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() > 3) {
		change_status(FIGHTER_STATUS_CROUCH_U);
		return;
	}
	if (anim_end) {
		change_status(FIGHTER_STATUS_CROUCH);
		return;
	}
}

void Fighter::enter_status_crouchd() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NON_ACTION;
	object_string[FIGHTER_STRING_MOVE_KIND] = "";
	change_anim("crouch_d");
	change_script("crouch_d");
}

void Fighter::exit_status_crouchd() {}

void Fighter::status_crouch() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_CROUCH_U);
		return;
	}
}

void Fighter::enter_status_crouch() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NON_ACTION;
	object_string[FIGHTER_STRING_MOVE_KIND] = "";
	change_anim("crouch");
	change_script("crouch");
	common_ground_status_act();
}

void Fighter::exit_status_crouch() {}

void Fighter::status_crouchu() {
	if (common_ground_status_act()) {
		return;
	}
	if (anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
	}
}

void Fighter::enter_status_crouchu() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NON_ACTION;
	change_anim("crouch_u");
	change_script("crouch_u");
}

void Fighter::exit_status_crouchu() {}

void Fighter::status_jumpsquat() {
	if (is_status_end(FIGHTER_STATUS_JUMP)) {
		return;
	}
	if (frame < 3.0) {
		unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)) {
			object_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_N;
			change_status(FIGHTER_STATUS_PARRY_START, false);
			change_anim("parry_start_high");
			change_script("parry_start_high");
			object_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
			return;
		}
		check_movelist_inputs();
		if (object_flag[FIGHTER_FLAG_STATUS_CHANGED] || chara_ground_status_act()) {
			return;
		}
	}
	unsigned int stick_dir = get_stick_dir() % 3;
	if (stick_dir == 1) {
		object_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_B;
	}
	else if (stick_dir == 0) {
		object_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_F;
	}
}

void Fighter::enter_status_jumpsquat() {
	object_int[FIGHTER_INT_JUMP_KIND] = get_stick_dir() % 3;
	object_flag[FIGHTER_FLAG_ENABLE_DODGE_COUNTER] = true;
	change_anim("jump_squat");
	change_script("jump_squat");
}

void Fighter::exit_status_jumpsquat() {
	object_flag[FIGHTER_FLAG_ENABLE_DODGE_COUNTER] = false;
}

void Fighter::status_jump() {
	if (check_landing()) {
		return;
	}
	common_air_status_general();
	if (is_status_end(FIGHTER_STATUS_FALL)) {
		return;
	}
}

void Fighter::enter_status_jump() {
	object_flag[FIGHTER_FLAG_SHORT_HOP] = get_stick_dir() < 7;
	object_flag[FIGHTER_FLAG_USED_AIRDASH] = false;
	change_context(FIGHTER_CONTEXT_AIR);
	if (object_flag[FIGHTER_FLAG_SHORT_HOP]) {
		object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = get_param_float("jump_y_init_speed_s");
	}
	else {
		object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = get_param_float("jump_y_init_speed");
	}
	switch (object_int[FIGHTER_INT_JUMP_KIND]) {
		case (JUMP_KIND_F): {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("jump_x_speed") * facing_dir;
		} break;
		case (JUMP_KIND_B): {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("jump_x_speed") * -facing_dir;
		} break;
		case (JUMP_KIND_N): {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0;
		} break;
	}
	object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = get_param_float("gravity");
	object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = get_param_float("max_fall_speed");
	switch (object_int[FIGHTER_INT_JUMP_KIND]) {
		case (JUMP_KIND_F): {
			change_anim("jump_f");
			change_script("jump_f");
		} break;
		case (JUMP_KIND_B): {
			change_anim("jump_b");
			change_script("jump_b");
		} break;
		case (JUMP_KIND_N): {
			change_anim("jump_n");
			change_script("jump_n");
		} break;
	}

	check_movelist_inputs();
}

void Fighter::exit_status_jump() {

}

void Fighter::status_fall() {
	if (check_landing()) {
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_fall() {
	object_string[FIGHTER_STRING_MOVE_KIND] = "";
	change_anim("fall");
	change_script("fall");
}


void Fighter::exit_status_fall() {
	object_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_N;
}

void Fighter::status_turn() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() > 3) {
		if (anim_end) {
			change_status(FIGHTER_STATUS_WAIT);
			return;
		}
		if (get_anim() == "turn_crouch") {
			change_anim_inherit_attributes("turn_stand");
			change_script("turn_stand");
		}
	}
	else {
		if (anim_end) {
			change_status(FIGHTER_STATUS_CROUCH);
			return;
		}
		if (get_anim() == "turn_stand") {
			change_anim_inherit_attributes("turn_crouch");
			change_script("turn_crouch");
		}
	}
}

void Fighter::enter_status_turn() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NON_ACTION;
	object_string[FIGHTER_STRING_MOVE_KIND] = "";
	//First we want to make sure you don't accidentally dash because you inputted
	//one direction, turned around, then inputted the other direction
	facing_right = internal_facing_right;
	if (facing_right) {
		facing_dir = 1.0;
	}
	else {
		facing_dir = -1.0;
	}
	move_list[FIGHTER_CONTEXT_GROUND].swap_buffers("dash_f", "dash_b");
	int dash_f = object_int[FIGHTER_INT_66_STEP];
	int dash_b = object_int[FIGHTER_INT_44_STEP];
	object_int[FIGHTER_INT_66_STEP] = 0;
	object_int[FIGHTER_INT_44_STEP] = 0;
	if (common_ground_status_act()) {
		return;
	}
	object_int[FIGHTER_INT_66_STEP] = dash_b;
	object_int[FIGHTER_INT_44_STEP] = dash_f;
	if (get_stick_dir() < 4) {
		change_anim("turn_crouch");
		change_script("turn_crouch");
	}
	else {
		change_anim("turn_stand");
		change_script("turn_stand");
	}
}


void Fighter::exit_status_turn() {

}

void Fighter::status_attack() {
	if (object_int[FIGHTER_INT_EXTERNAL_FRAME] == 2 
	&& !object_flag[FIGHTER_FLAG_CHANGE_INTO_SAME_STATUS]) {
		switch (get_curr_move().required_buttons) {
			case (BUTTON_LP_BIT):
			case (BUTTON_LK_BIT): {
				disable_cancel("grab_start", CANCEL_KIND_ANY);
			} break;
			case (BUTTON_MP_BIT):
			case (BUTTON_MK_BIT): {
				disable_cancel("parry_start", CANCEL_KIND_ANY);
			} break;
			case (BUTTON_HP_BIT):
			case (BUTTON_HK_BIT): {
				disable_cancel("advance", CANCEL_KIND_ANY);
				disable_cancel("advance_forward", CANCEL_KIND_ANY);
				disable_cancel("advance_back", CANCEL_KIND_ANY);
			} break;
		}
	}
	if (is_status_end()) {
		return;
	}
	if (chara_status_attack()) {
		return;
	}
	add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0.0f));
}

void Fighter::enter_status_attack() {
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
	if (!object_flag[FIGHTER_FLAG_CHANGE_INTO_SAME_STATUS]) {
		switch (get_curr_move().required_buttons) {
			case (BUTTON_LP_BIT):
			case (BUTTON_LK_BIT): {
				enable_cancel("grab_start", CANCEL_KIND_ANY);
			} break;
			case (BUTTON_MP_BIT):
			case (BUTTON_MK_BIT): {
				enable_cancel("parry_start", CANCEL_KIND_ANY);
			} break;
			case (BUTTON_HP_BIT):
			case (BUTTON_HK_BIT): {
				enable_cancel("advance", CANCEL_KIND_ANY);
				enable_cancel("advance_forward", CANCEL_KIND_ANY);
				enable_cancel("advance_back", CANCEL_KIND_ANY);
			} break;
		}
	}
	object_int[FIGHTER_INT_HITSTUN_HEIGHT] = move_list[fighter_context].is_curr_move_recover_crouching(this);
	if (object_string[FIGHTER_STRING_MOVE_KIND] == move_script.name
	&& anim_table.has_anim(object_string[FIGHTER_STRING_MOVE_KIND] + "_chain")) {
		change_anim(object_string[FIGHTER_STRING_MOVE_KIND] + "_chain");
	}
	else {
		change_anim(object_string[FIGHTER_STRING_MOVE_KIND]);
	}
	change_script(object_string[FIGHTER_STRING_MOVE_KIND]);
}

void Fighter::exit_status_attack() {
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
	stop_reserved_sound();
	clear_effect_all();
}

void Fighter::status_attack_air() {
	if (object_int[FIGHTER_INT_EXTERNAL_FRAME] == 2
	&& !object_flag[FIGHTER_FLAG_CHANGE_INTO_SAME_STATUS]) {
		switch (get_curr_move().required_buttons) {
			case (BUTTON_LP_BIT):
			case (BUTTON_LK_BIT): {
				disable_cancel("grab_start", CANCEL_KIND_ANY);
			} break;
			case (BUTTON_MP_BIT):
			case (BUTTON_MK_BIT): {
				disable_cancel("parry_start", CANCEL_KIND_ANY);
			} break;
		}
	}
	if (check_landing(FIGHTER_STATUS_LANDING_ATTACK)) {
		return;
	}
	if (chara_status_attack_air()) {
		return;
	}
	if (is_status_end()) {
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_attack_air() {
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
	if (!object_flag[FIGHTER_FLAG_CHANGE_INTO_SAME_STATUS]) {
		switch (get_curr_move().required_buttons) {
			case (BUTTON_LP_BIT):
			case (BUTTON_LK_BIT): {
				enable_cancel("grab_start", CANCEL_KIND_ANY);
			} break;
			case (BUTTON_MP_BIT):
			case (BUTTON_MK_BIT): {
				enable_cancel("parry_start", CANCEL_KIND_ANY);
			} break;
			default: {
			} break;
		}
	}
	change_anim(object_string[FIGHTER_STRING_MOVE_KIND]);
	change_script(object_string[FIGHTER_STRING_MOVE_KIND]);
}

void Fighter::exit_status_attack_air() {
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
	stop_reserved_sound();
	clear_effect_all();
}

void Fighter::status_backdash_attack() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_backdash_attack() {
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
	change_anim(object_string[FIGHTER_STRING_MOVE_KIND]);
	change_script(object_string[FIGHTER_STRING_MOVE_KIND]);
}

void Fighter::exit_status_backdash_attack() {
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
	stop_reserved_sound();
	clear_effect_all();
}

void Fighter::status_advance() {
	if (is_status_end()) {
		return;
	}
	if (frame < get_param_float("advance_low_transition_frame") && get_stick_dir() < 4) {
		change_anim_inherit_attributes("advance_low");
		change_script("advance_low");
	}
}

void Fighter::enter_status_advance() {
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 0;
	object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 0;
	change_anim("advance_high");
	change_script("advance_high");
}

void Fighter::exit_status_advance() {
	object_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = false;
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
}

void Fighter::status_advance_forward() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_advance_forward() {
	change_anim("advance_forward");
	change_script("advance_forward");
}

void Fighter::exit_status_advance_forward() {
	object_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = false;
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
}

void Fighter::status_advance_back() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_advance_back() {
	change_anim("advance_back");
	change_script("advance_back");
}

void Fighter::exit_status_advance_back() {
	object_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = false;
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
}

void Fighter::status_grab_start() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_grab_start() {
	object_flag[FIGHTER_FLAG_THROW_TECH] = true;
	change_anim("grab_start");
	change_script("grab_start");
}

void Fighter::exit_status_grab_start() {
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
}

void Fighter::status_grab() {
	if (is_status_end(FIGHTER_STATUS_THROW)) {
		return;
	}
}

void Fighter::enter_status_grab() {
	object_int[FIGHTER_INT_HITSTUN_HEIGHT] = move_list[fighter_context].is_curr_move_recover_crouching(this);
	change_anim("grab");
	change_script("grab");
}

void Fighter::exit_status_grab() {

}

void Fighter::status_throw() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_throw() {
	change_anim(throw_map_ground[get_stick_dir()]);
	change_script(throw_map_ground[get_stick_dir()]);
}

void Fighter::exit_status_throw() {

}

void Fighter::status_grab_air_start() {
	if (check_landing()) {
		return;
	}
	if (is_status_end()) {
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_grab_air_start() {
	object_flag[FIGHTER_FLAG_THROW_TECH] = true;
	change_anim("grab_air_start");
	change_script("grab_air_start");
}

void Fighter::exit_status_grab_air_start() {
	object_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
}

void Fighter::status_grab_air() {
	if (is_status_end(FIGHTER_STATUS_THROW_AIR)) {
		return;
	}
}

void Fighter::enter_status_grab_air() {
	change_anim("grab_air");
	change_script("grab_air");
}

void Fighter::exit_status_grab_air() {

}

void Fighter::status_throw_air() {
	if (get_pos().y <= 0.0f) {
		set_pos_y(0.0f);
		change_context(FIGHTER_CONTEXT_GROUND);
	}
	if (is_status_end()) {
		return;
	}
	if (!object_flag[FIGHTER_FLAG_DISABLE_AIR_GENERAL]) {
		common_air_status_general();
	}
}

void Fighter::enter_status_throw_air() {
	change_anim(throw_map_air[get_stick_dir()]);
	change_script(throw_map_air[get_stick_dir()]);
}

void Fighter::exit_status_throw_air() {
	object_flag[FIGHTER_FLAG_DISABLE_AIR_GENERAL] = false;
	if (object_flag[FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END] && object_int[FIGHTER_INT_JUMP_KIND] != JUMP_KIND_N) {
		object_int[FIGHTER_INT_JUMP_KIND] ^= 1; //Flips between JUMP_KIND_F and B
	}
}

void Fighter::status_grabbed() {
	Fighter* that = object_manager->fighter[!id];
	if (object_flag[FIGHTER_FLAG_IN_THROW_TECH_WINDOW] && object_flag[FIGHTER_FLAG_ALLOW_THROW_TECH]) {
		unsigned int grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
		if (check_button_input(grab_buttons, 2)) {
			change_status(FIGHTER_STATUS_THROW_TECH);
			that->change_status(FIGHTER_STATUS_THROW_TECH);
			return;
		}
	}
	glm::vec3 offset = glm::vec3(
		object_float[FIGHTER_FLOAT_GRAB_OFFSET_X], 
		object_float[FIGHTER_FLOAT_GRAB_OFFSET_Y], 
		object_float[FIGHTER_FLOAT_GRAB_OFFSET_Z]
	);
	glm::vec3 offset_bone_pos = get_relative_bone_position(object_int[FIGHTER_INT_GRABBED_BONE_ID]);
	glm::vec3 target_pos_per_frame = that->get_bone_position(object_int[FIGHTER_INT_GRAB_BONE_ID], offset);
	set_pos_validate(target_pos_per_frame - offset_bone_pos);
}

void Fighter::enter_status_grabbed() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_GRABBED;
	object_flag[FIGHTER_FLAG_LOCK_DIRECTION] = true;
	object_flag[FIGHTER_FLAG_ALLOW_THROW_TECH] = !object_flag[FIGHTER_FLAG_DISABLE_THROW_TECH] 
		&& (is_actionable() || object_flag[FIGHTER_FLAG_ATTACK_HITSTUN_PARRIED]);
	change_anim("grabbed");
	change_script("grabbed");
}

void Fighter::exit_status_grabbed() {
	object_flag[FIGHTER_FLAG_LOCK_DIRECTION] = false;
	Fighter* that = object_manager->fighter[!id];
	if (that->get_scaled_pos().x != get_scaled_pos().x) {
		facing_right = that->get_scaled_pos().x > get_scaled_pos().x;
		facing_dir = facing_right ? 1.0 : -1.0;
	}
	set_pos_z(0.0f);
}

void Fighter::status_thrown() {
	if (check_landing(FIGHTER_STATUS_KNOCKDOWN)) {
		return;
	}
	apply_gravity(object_float[FIGHTER_FLOAT_CURRENT_GRAVITY], object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX]);
	if (!add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0)) && get_scaled_pos().y != 0.0) {
		Fighter* that = object_manager->fighter[!id];
		//If you get thrown into the corner, the attacker should be pushed away from you.
		that->add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED] * -1.0, 0.0, 0.0));
	}
}

void Fighter::enter_status_thrown() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_GRABBED;
	Fighter* that = object_manager->fighter[!id];
	if (that->get_scaled_pos().x != get_scaled_pos().x) {
		facing_right = that->get_scaled_pos().x > get_scaled_pos().x;
		facing_dir = facing_right ? 1.0 : -1.0;
	}
	change_context(FIGHTER_CONTEXT_AIR);
	change_anim("thrown", calc_airtime(), -1.0);
	change_script("thrown");
}

void Fighter::exit_status_thrown() {

}

void Fighter::status_throw_tech() {
	if (is_status_end()) {
		return;
	}
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 15 || object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 16) {
		frame++;
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0;
	}
	else if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] < 15) {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = facing_dir * -16;
	}
	else {
		object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = facing_dir * -4;
	}
	add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], 0, 0));
}

void Fighter::enter_status_throw_tech() {
	if (fighter_context == FIGHTER_CONTEXT_GROUND) {
		change_anim("throw_tech");
		change_script("throw_tech");
	}
	else {
		change_anim("throw_tech_air");
		change_script("throw_tech_air");
	}
	object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 20;
	object_flag[FIGHTER_FLAG_IN_THROW_TECH_WINDOW] = false;
}

void Fighter::exit_status_throw_tech() {}

void Fighter::status_blockstun() {
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] != 0) {
		unsigned int advance_buttons[2] = { BUTTON_HP, BUTTON_HK };
		if (check_button_input(advance_buttons, 2) && player->control_type == CONTROL_TYPE_ADVANCE) {
			if ((get_stick_dir() == 4 || get_stick_dir() == 1) && can_spend_ex(300.0)) {
				object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] = 0;
				object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 0;
				object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = 0;
				object_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = true;
				change_status(FIGHTER_STATUS_ADVANCE_BACK);
				return;
			}
		}
	}
	else {
		if (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] == 0) {
			switch (object_int[FIGHTER_INT_HITSTUN_HEIGHT]) {
				case (HITSTUN_HEIGHT_STAND): {
					change_status(FIGHTER_STATUS_WAIT);
					return;
				} break;
				case (HITSTUN_HEIGHT_CROUCH): {
					change_status(FIGHTER_STATUS_CROUCH);
					return;
				} break;
			}
		}
	}
}

void Fighter::enter_status_blockstun() {
	change_anim(object_string[FIGHTER_STRING_HITSTUN_ANIM], object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES], -1.0);
	change_script(object_string[FIGHTER_STRING_HITSTUN_ANIM]);
}

void Fighter::exit_status_blockstun() {}

void Fighter::status_hitstun() {
	if (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] == 0) {
		switch (object_int[FIGHTER_INT_HITSTUN_HEIGHT]) {
			case (HITSTUN_HEIGHT_STAND): {
				change_status(FIGHTER_STATUS_WAIT);
				return;
			} break;
			case (HITSTUN_HEIGHT_CROUCH): {
				change_status(FIGHTER_STATUS_CROUCH);
				return;
			} break;
		}
	}
	else if (check_hitstun_parry()) {
		change_status(FIGHTER_STATUS_HITSTUN_PARRY_START);
		return;
	}
}

void Fighter::enter_status_hitstun() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_HITSTUN;
	change_anim(object_string[FIGHTER_STRING_HITSTUN_ANIM], object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES], -1.0);
	change_script(object_string[FIGHTER_STRING_HITSTUN_ANIM]);
}

void Fighter::exit_status_hitstun() {

}

void Fighter::status_hitstun_air() {
	Fighter* that = object_manager->fighter[!id];
	if (check_landing(FIGHTER_STATUS_LANDING_HITSTUN)) {
		return;
	}
	if (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] == 0) {
		object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = get_param_float("gravity");
		object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = get_param_float("max_fall_speed");
		change_status(FIGHTER_STATUS_FALL);
		return;
	}
	else if (check_hitstun_parry()) {
		change_status(FIGHTER_STATUS_HITSTUN_PARRY_START);
		return;
	}
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
		if (!object_int[FIGHTER_INT_PUSHBACK_FRAMES]) {
			apply_gravity(object_float[FIGHTER_FLOAT_CURRENT_GRAVITY], object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX]);
		}
		add_pos_validate(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
	}
}

void Fighter::enter_status_hitstun_air() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_HITSTUN;
	object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] / object_int[FIGHTER_INT_PUSHBACK_FRAMES];
	object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = 0.0f;
	change_anim("jump_hitstun", object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES], -1.0);
	change_script("jump_hitstun");
}


void Fighter::exit_status_hitstun_air() {

}

void Fighter::status_hitstun_float() {
	if (anim_end) {
		object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = true;
		object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = 0;
		change_status(FIGHTER_STATUS_KNOCKDOWN);
		return;
	}
	if (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
		std::string param_text_add = "_up";
		if (object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
			param_text_add = "_down";
		}
		int end_rate_frame = get_param_int("float_hitstun_end_frame" + param_text_add);
		if (frame == get_param_int("float_hitstun_start_frame" + param_text_add) && rate == 1.0f) {
			set_rate((end_rate_frame - frame) / (float)object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]);
		}
		else if (frame > end_rate_frame) {
			set_rate(1.0);
		}
	}
}

void Fighter::enter_status_hitstun_float() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NONSTANDARD_HITSTUN;
	change_context(FIGHTER_CONTEXT_AIR);
	if (object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("hitstun_float_down");
		change_script("hitstun_float_down");
	}
	else {
		change_anim("hitstun_float_up");
		change_script("hitstun_float_up");
	}

}

void Fighter::exit_status_hitstun_float() {

}

void Fighter::status_launch_start() {
	if (anim_end) {
		change_status(FIGHTER_STATUS_LAUNCH);
	}
}

void Fighter::enter_status_launch_start() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NONSTANDARD_HITSTUN;
	change_anim("launch_start");
	change_script("launch_start");
}

void Fighter::exit_status_launch_start() {
	change_context(FIGHTER_CONTEXT_AIR);
}

void Fighter::status_launch() {
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
		common_air_status_general();
		if (object_flag[FIGHTER_FLAG_LAUNCH_FLOAT]) {
			if (check_landing(FIGHTER_STATUS_HITSTUN_FLOAT)) {
				object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = 0.0f;
				object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = 0.0f;
				return;
			}
		}
		else {
			if (check_landing(FIGHTER_STATUS_KNOCKDOWN)) {
				object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = 0.0f;
				object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = 0.0f;
				return;
			}
		}
		if (check_hitstun_parry()) {
			change_status(FIGHTER_STATUS_LAUNCH_PARRY_START);
		}
	}
}

void Fighter::enter_status_launch() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NONSTANDARD_HITSTUN;
	object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = false;
	if (!object_flag[FIGHTER_FLAG_LAUNCH_FLOAT]) {
		object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = 0;
	}
	if (object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] == 0.0f) {
		object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = get_param_float("gravity");
	}
	if (object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] == 0.0f) {
		object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = get_param_float("max_fall_speed");
	}
	if (object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN]) {
		change_anim("launch_hkd", calc_airtime(), -1.0);
		change_script("launch_hkd");
	}
	else {
		change_anim("launch", calc_airtime(), -1.0);
		change_script("launch");
	}
}

void Fighter::exit_status_launch() {
	object_flag[FIGHTER_FLAG_LAUNCH_FLOAT] = false;
}

void Fighter::status_crumple() {
	if (anim_end) {
		object_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_SLOW;
		object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = 0;
		if (is_ko()) {
			change_status(FIGHTER_STATUS_KO);
		}
		else {
			change_status(FIGHTER_STATUS_WAKEUP);
		}
		return;
	}
	if (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
		std::string param_text_add = "_up";
		if (object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
			param_text_add = "_down";
		}
		int end_rate_frame = get_param_int("crumple_hitstun_end_frame" + param_text_add);
		if (frame == get_param_int("crumple_hitstun_start_frame" + param_text_add) && rate == 1.0f) {
			set_rate((end_rate_frame - frame) / (float)object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]);
		}
		else if (frame > end_rate_frame) {
			set_rate(1.0);
		}
	}
}

void Fighter::enter_status_crumple() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NONSTANDARD_HITSTUN;
	if (object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("crumple_down");
		change_script("crumple_down");
	}
	else {
		change_anim("crumple_up");
		change_script("crumple_up");
	}
}

void Fighter::exit_status_crumple() {

}

void Fighter::status_knockdown_start() {
	if (fighter_context == FIGHTER_CONTEXT_AIR && !object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]) {
		set_pos_y(clampf(0.0f, get_scaled_pos().y - object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], get_scaled_pos().y));
	}
	if (anim_end) {
		change_status(FIGHTER_STATUS_KNOCKDOWN);
	}
}

void Fighter::enter_status_knockdown_start() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NONSTANDARD_HITSTUN;
	if (object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("knockdown_start_down");
		change_script("knockdown_start_down");
		if (fighter_context == FIGHTER_CONTEXT_AIR) {
			int skip_frame = get_param_int("knockdown_start_air_skip_frame_down");
			object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = get_scaled_pos().y / (get_anim_length(get_anim()) -
				skip_frame
			);
			frame = skip_frame;
		}
	}
	else {
		change_anim("knockdown_start_up");
		change_script("knockdown_start_up");
		if (fighter_context == FIGHTER_CONTEXT_AIR) {
			int skip_frame = get_param_int("knockdown_start_air_skip_frame_up");
			object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = get_scaled_pos().y / (get_anim_length(get_anim()) -
				skip_frame
			);
			frame = skip_frame;
		}
	}
}

void Fighter::exit_status_knockdown_start() {

}

void Fighter::status_knockdown() {
	if (!(object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] || object_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW])) {
		switch (get_flick_dir()) {
			case 1:
			case 4:
			case 7: {
				object_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_BACK;
				object_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = get_global_param_int(PARAM_FIGHTER, "tech_window");
			} break;
			case 3:
			case 6:
			case 9: {
				object_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_FORWARD;
				object_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = get_global_param_int(PARAM_FIGHTER, "tech_window");
			} break;
			default: {
				object_int[FIGHTER_INT_WAKEUP_TYPE] = object_int[FIGHTER_INT_AUTO_WAKEUP_TYPE];
			} break;
		}
	}
	if (anim_end) {
		object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = 0;
		if (is_ko()) {
			change_status(FIGHTER_STATUS_KO);
		}
		else {
			change_status(FIGHTER_STATUS_WAKEUP);
		}
		return;
	}
	if (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
		std::string param_text_add = "_up";
		if (object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
			param_text_add = "_down";
		}
		int end_rate_frame = get_param_int("knockdown_hitstun_end_frame" + param_text_add);
		if (frame == get_param_int("knockdown_hitstun_start_frame" + param_text_add) && rate == 1.0f) {
			set_rate((end_rate_frame - frame) / (float)object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]);
		}
		else if (frame > end_rate_frame) {
			set_rate(1.0f);
		}
	}
}

void Fighter::enter_status_knockdown() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NONSTANDARD_HITSTUN;
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 0;
	if (object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN]) {
		object_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_SLOW;
	}
	if (object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("knockdown_down");
		change_script("knockdown_down");
	}
	else {
		change_anim("knockdown_up");
		change_script("knockdown_up");
	}
	change_context(FIGHTER_CONTEXT_DOWN);
}

void Fighter::exit_status_knockdown() {
	object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = false;
}

void Fighter::status_wakeup() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_wakeup() {
	change_context(FIGHTER_CONTEXT_GROUND);
	if (object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		switch (object_int[FIGHTER_INT_WAKEUP_TYPE]) {
			default:
			case WAKEUP_TYPE_DEFAULT: {
				change_anim("wakeup_down");
				change_script("wakeup_down");
			} break;
			case WAKEUP_TYPE_FORWARD: {
				change_anim("wakeup_forward_down");
				change_script("wakeup_forward_down");
			} break;
			case WAKEUP_TYPE_BACK: {
				change_anim("wakeup_back_down");
				change_script("wakeup_back_down");
			} break;
			case WAKEUP_TYPE_SLOW: {
				change_anim("wakeup_slow_down");
				change_script("wakeup_slow_down");
			} break;
		}
	}
	else {
		switch (object_int[FIGHTER_INT_WAKEUP_TYPE]) {
			default:
			case WAKEUP_TYPE_DEFAULT: {
				change_anim("wakeup_up");
				change_script("wakeup_up");
			} break;
			case WAKEUP_TYPE_FORWARD: {
				change_anim("wakeup_forward_up");
				change_script("wakeup_forward_up");
			} break;
			case WAKEUP_TYPE_BACK: {
				change_anim("wakeup_back_up");
				change_script("wakeup_back_up");
			} break;
			case WAKEUP_TYPE_SLOW: {
				change_anim("wakeup_slow_up");
				change_script("wakeup_slow_up");
			} break;
		}
	}
	object_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_DEFAULT;
	object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = false;
}

void Fighter::exit_status_wakeup() {

}

void Fighter::status_parry_start() {
	if (is_status_end()) {
		return;
	}
	if (fighter_context == FIGHTER_CONTEXT_AIR) {
		if (check_landing(FIGHTER_STATUS_LANDING)) {
			return;
		}
		common_air_status_general();
	}
	else {
		if (frame < 2.0 && object_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_MID) {
			if (get_stick_dir() < 4) {
				change_anim_inherit_attributes("parry_start_low");
				change_script("parry_start_low");
				object_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_LOW;
			}
			else if (get_stick_dir() >= 7) {
				change_anim_inherit_attributes("parry_start_high");
				change_script("parry_start_high");
				object_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
			}
		}
	}
}

void Fighter::enter_status_parry_start() {
	if (fighter_context == FIGHTER_CONTEXT_AIR) {
		change_anim("parry_air_start");
		change_script("parry_air_start");
		object_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
	}
	else {
		switch (get_stick_dir()) {
			case 1:
			case 2:
			case 3: {
				change_anim("parry_start_low");
				change_script("parry_start_low");
				object_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_LOW;
			} break;
			case 4:
			case 5:
			case 6: {
				change_anim("parry_start_mid");
				change_script("parry_start_mid");
				object_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MID;
			} break;
			case 7:
			case 8:
			case 9: {
				change_anim("parry_start_high");
				change_script("parry_start_high");
				object_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
			} break;
		}
	}
	if (object_flag[FIGHTER_FLAG_ATTACK_HITSTUN_PARRIED]) {
		object_flag[FIGHTER_FLAG_HITSTUN_COUNTER_PARRY] = true;
	}
	object_int[FIGHTER_INT_HITSTUN_HEIGHT] = move_list[fighter_context].is_curr_move_recover_crouching(this);
}

void Fighter::exit_status_parry_start() {
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
	object_flag[FIGHTER_FLAG_PARRY_ACTIVE] = false;
}

void Fighter::status_hitstun_parry_start() {
	if (fighter_context == FIGHTER_CONTEXT_AIR) {
		common_air_status_general();
		if (check_landing(FIGHTER_STATUS_LANDING)) {
			return;
		}
	}
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_hitstun_parry_start() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_HITSTUN;
	object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = 0;
	object_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	if (fighter_context == FIGHTER_CONTEXT_AIR) {
		change_anim("hitstun_parry_air_start");
		change_script("hitstun_parry_air_start");
	}
	else {
		change_anim("hitstun_parry_start");
		change_script("hitstun_parry_start");
	}
}

void Fighter::exit_status_hitstun_parry_start() {
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
	object_flag[FIGHTER_FLAG_PARRY_ACTIVE] = false;
}

void Fighter::status_launch_parry_start() {
	common_air_status_general();
	if (check_landing(FIGHTER_STATUS_KNOCKDOWN)) {
		object_float[FIGHTER_FLOAT_CURRENT_GRAVITY] = 0.0f;
		object_float[FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX] = 0.0f;
		return;
	}
}

void Fighter::enter_status_launch_parry_start() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NONSTANDARD_HITSTUN;
	object_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	change_anim("launch_parry_start");
	change_script("launch_parry_start");
}

void Fighter::exit_status_launch_parry_start() {
	object_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = true;
	object_flag[FIGHTER_FLAG_FORCE_CRITICAL] = false;
	object_flag[FIGHTER_FLAG_PARRY_ACTIVE] = false;
}

void Fighter::status_parry() {
	player->controller.set_hold_buffer(true);
	if (beginning_hitlag(2) || ending_hitlag(2)) {
		frame++;
	}
	if (fighter_context == FIGHTER_CONTEXT_GROUND && object_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_LOW) {
		if (is_status_end(FIGHTER_STATUS_CROUCH)) {
			return;
		}
	}
	else {
		if (is_status_end()) {
			return;
		}
	}
}

void Fighter::enter_status_parry() {
	if (fighter_context == FIGHTER_CONTEXT_AIR) {
		change_anim("parry_air");
		change_script("parry_air");
	}
	else {
		switch (object_int[FIGHTER_INT_PARRY_HEIGHT]) {
			case (PARRY_HEIGHT_LOW): {
				change_anim("parry_low");
				change_script("parry_low");
			} break;
			case (PARRY_HEIGHT_MID):
			default: {
				change_anim("parry_mid");
				change_script("parry_mid");
			} break;
			case (PARRY_HEIGHT_HIGH): {
				change_anim("parry_high");
				change_script("parry_high");
			} break;
		}
	}

}

void Fighter::exit_status_parry() {
	object_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MAX;
	player->controller.set_hold_buffer(false);
}

void Fighter::status_landing() {
	switch (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
		case (0): {
			check_movelist_inputs();
			if (anim_end) {
				change_status(FIGHTER_STATUS_WAIT);
				return;
			}
			else if (get_stick_dir() >= 4 && common_ground_status_act()) {
				return;
			}
		} break;
		case (1): {
			if (internal_facing_right != facing_right) {
				facing_right = internal_facing_right;
				if (facing_right) {
					facing_dir = 1.0;
				}
				else {
					facing_dir = -1.0;
				}
			}
		} [[fallthrough]];
		default: {
			if (object_manager->is_allow_realtime_process(this)) {
				object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]--;
			}
		} break;
	}
} 

void Fighter::enter_status_landing() {
	object_string[FIGHTER_STRING_MOVE_KIND] = "";
	landing_crossup();
	object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = get_param_int("empty_landing_lag");
	change_anim("landing", object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES], -1.0);
	change_script("landing");
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0f;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 0.0f;
	change_context(FIGHTER_CONTEXT_GROUND);
	set_pos_y(0.0f);
}

void Fighter::exit_status_landing() {}

void Fighter::status_landing_attack() {
	switch (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
		case (0): {
			check_movelist_inputs();
			if (get_stick_dir() < 4) {
				change_status(FIGHTER_STATUS_CROUCH);
				return;
			}
			else {
				if (is_status_end(FIGHTER_STATUS_WAIT)) {
					return;
				}
			}
		} break;
		case (1): {
			if (internal_facing_right != facing_right) {
				facing_right = internal_facing_right;
				if (facing_right) {
					facing_dir = 1.0;
				}
				else {
					facing_dir = -1.0;
				}
			}
		} [[fallthrough]];
		default: {
			if (object_manager->is_allow_realtime_process(this)) {
				object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]--;
			}
		} break;
	}
}

void Fighter::enter_status_landing_attack() {
	landing_crossup();
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = true;

	object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES] = get_param_int(object_string[FIGHTER_STRING_MOVE_KIND] + "_landing_lag");
	change_anim(object_string[FIGHTER_STRING_MOVE_KIND] + "_landing", object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES], -1.0);
	change_script(object_string[FIGHTER_STRING_MOVE_KIND] + "_landing");
	object_string[FIGHTER_STRING_MOVE_KIND] = "";

	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0f;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 0.0f;
	change_context(FIGHTER_CONTEXT_GROUND);
	set_pos_y(0.0f);
}

void Fighter::exit_status_landing_attack() {
	object_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
}

void Fighter::status_landing_hitstun() {
	switch (object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
		case (0): {
			check_movelist_inputs();
			if (get_stick_dir() < 4) {
				change_status(FIGHTER_STATUS_CROUCH);
				return;
			}
			else {
				if (is_status_end(FIGHTER_STATUS_WAIT)) {
					return;
				}
			}
		} break;
		case (1): {
			if (internal_facing_right != facing_right) {
				facing_right = internal_facing_right;
				if (facing_right) {
					facing_dir = 1.0;
				}
				else {
					facing_dir = -1.0;
				}
			}
		} [[fallthrough]];
		default: {
			if (object_manager->is_allow_realtime_process(this)) {
				object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]--;
			}
		} break;
	}
}


void Fighter::enter_status_landing_hitstun() {
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_HITSTUN;
	object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = 0.0f;
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 0.0f;
	change_anim("landing_hitstun", object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES], -1.0);
	change_script("landing_hitstun");
	object_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	change_context(FIGHTER_CONTEXT_GROUND);
	set_pos_y(0.0f);
}

void Fighter::exit_status_landing_hitstun() {

}

void Fighter::status_taunt() {
	if (is_status_end(FIGHTER_STATUS_WAIT)) {
		return;
	}
}

void Fighter::enter_status_taunt() {
	change_anim("taunt");
	change_script("taunt");
}

void Fighter::exit_status_taunt() {

}

void Fighter::status_ko() {

}

void Fighter::enter_status_ko() {
	if (object_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("knockdown_lose_down");
		change_script("knockdown_lose_down");
	}
	else {
		change_anim("knockdown_lose_up");
		change_script("knockdown_lose_up");
	}
}

void Fighter::exit_status_ko() {

}

void Fighter::status_round_end() {

}

void Fighter::enter_status_round_end() {
	if (object_float[FIGHTER_FLOAT_HEALTH] == get_param_float("health")) {
		change_anim("round_win_perfect");
		change_script("round_win_perfect");
	}
	else {
		change_anim("round_win");
		change_script("round_win");
	}
}

void Fighter::exit_status_round_end() {
	
}

void Fighter::load_fighter_status_scripts() {
	SET_STATUS_FUNC(FIGHTER_STATUS_WAIT, &Fighter::status_wait);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_WAIT, &Fighter::enter_status_wait);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_WAIT, &Fighter::exit_status_wait);

	SET_STATUS_FUNC(FIGHTER_STATUS_WALK_F, &Fighter::status_walk_f);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_WALK_F, &Fighter::enter_status_walk_f);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_WALK_F, &Fighter::exit_status_walk_f);

	SET_STATUS_FUNC(FIGHTER_STATUS_WALK_B, &Fighter::status_walk_b);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_WALK_B, &Fighter::enter_status_walk_b);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_WALK_B, &Fighter::exit_status_walk_b);

	SET_STATUS_FUNC(FIGHTER_STATUS_DASH_F, &Fighter::status_dash_f);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_DASH_F, &Fighter::enter_status_dash_f);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_DASH_F, &Fighter::exit_status_dash_f);

	SET_STATUS_FUNC(FIGHTER_STATUS_DASH_B, &Fighter::status_dash_b);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_DASH_B, &Fighter::enter_status_dash_b);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_DASH_B, &Fighter::exit_status_dash_b);

	SET_STATUS_FUNC(FIGHTER_STATUS_DASH_AIR, &Fighter::status_dash_air);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_DASH_AIR, &Fighter::enter_status_dash_air);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_DASH_AIR, &Fighter::exit_status_dash_air);

	SET_STATUS_FUNC(FIGHTER_STATUS_CROUCH_D, &Fighter::status_crouchd);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_CROUCH_D, &Fighter::enter_status_crouchd);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_CROUCH_D, &Fighter::exit_status_crouchd);

	SET_STATUS_FUNC(FIGHTER_STATUS_CROUCH, &Fighter::status_crouch);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_CROUCH, &Fighter::enter_status_crouch);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_CROUCH, &Fighter::exit_status_crouch);

	SET_STATUS_FUNC(FIGHTER_STATUS_CROUCH_U, &Fighter::status_crouchu);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_CROUCH_U, &Fighter::enter_status_crouchu);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_CROUCH_U, &Fighter::exit_status_crouchu);

	SET_STATUS_FUNC(FIGHTER_STATUS_JUMPSQUAT, &Fighter::status_jumpsquat);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_JUMPSQUAT, &Fighter::enter_status_jumpsquat);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_JUMPSQUAT, &Fighter::exit_status_jumpsquat);

	SET_STATUS_FUNC(FIGHTER_STATUS_JUMP, &Fighter::status_jump);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_JUMP, &Fighter::enter_status_jump);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_JUMP, &Fighter::exit_status_jump);

	SET_STATUS_FUNC(FIGHTER_STATUS_FALL, &Fighter::status_fall);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_FALL, &Fighter::enter_status_fall);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_FALL, &Fighter::exit_status_fall);

	SET_STATUS_FUNC(FIGHTER_STATUS_TURN, &Fighter::status_turn);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_TURN, &Fighter::enter_status_turn);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_TURN, &Fighter::exit_status_turn);

	SET_STATUS_FUNC(FIGHTER_STATUS_ATTACK, &Fighter::status_attack);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_ATTACK, &Fighter::enter_status_attack);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_ATTACK, &Fighter::exit_status_attack);

	SET_STATUS_FUNC(FIGHTER_STATUS_ATTACK_AIR, &Fighter::status_attack_air);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_ATTACK_AIR, &Fighter::enter_status_attack_air);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_ATTACK_AIR, &Fighter::exit_status_attack_air);

	SET_STATUS_FUNC(FIGHTER_STATUS_BACKDASH_ATTACK, &Fighter::status_backdash_attack);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_BACKDASH_ATTACK, &Fighter::enter_status_backdash_attack);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_BACKDASH_ATTACK, &Fighter::exit_status_backdash_attack);

	SET_STATUS_FUNC(FIGHTER_STATUS_ADVANCE_FORWARD, &Fighter::status_advance_forward);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_ADVANCE_FORWARD, &Fighter::enter_status_advance_forward);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_ADVANCE_FORWARD, &Fighter::exit_status_advance_forward);

	SET_STATUS_FUNC(FIGHTER_STATUS_ADVANCE_BACK, &Fighter::status_advance_back);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_ADVANCE_BACK, &Fighter::enter_status_advance_back);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_ADVANCE_BACK, &Fighter::exit_status_advance_back);

	SET_STATUS_FUNC(FIGHTER_STATUS_ADVANCE, &Fighter::status_advance);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_ADVANCE, &Fighter::enter_status_advance);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_ADVANCE, &Fighter::exit_status_advance);

	SET_STATUS_FUNC(FIGHTER_STATUS_GRAB_START, &Fighter::status_grab_start);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_GRAB_START, &Fighter::enter_status_grab_start);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_GRAB_START, &Fighter::exit_status_grab_start);

	SET_STATUS_FUNC(FIGHTER_STATUS_GRAB, &Fighter::status_grab);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_GRAB, &Fighter::enter_status_grab);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_GRAB, &Fighter::exit_status_grab);

	SET_STATUS_FUNC(FIGHTER_STATUS_THROW, &Fighter::status_throw);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_THROW, &Fighter::enter_status_throw);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_THROW, &Fighter::exit_status_throw);

	SET_STATUS_FUNC(FIGHTER_STATUS_GRAB_AIR_START, &Fighter::status_grab_air_start);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_GRAB_AIR_START, &Fighter::enter_status_grab_air_start);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_GRAB_AIR_START, &Fighter::exit_status_grab_air_start);

	SET_STATUS_FUNC(FIGHTER_STATUS_GRAB_AIR, &Fighter::status_grab_air);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_GRAB_AIR, &Fighter::enter_status_grab_air);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_GRAB_AIR, &Fighter::exit_status_grab_air);

	SET_STATUS_FUNC(FIGHTER_STATUS_THROW_AIR, &Fighter::status_throw_air);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_THROW_AIR, &Fighter::enter_status_throw_air);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_THROW_AIR, &Fighter::exit_status_throw_air);

	SET_STATUS_FUNC(FIGHTER_STATUS_GRABBED, &Fighter::status_grabbed);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_GRABBED, &Fighter::enter_status_grabbed);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_GRABBED, &Fighter::exit_status_grabbed);

	SET_STATUS_FUNC(FIGHTER_STATUS_THROWN, &Fighter::status_thrown);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_THROWN, &Fighter::enter_status_thrown);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_THROWN, &Fighter::exit_status_thrown);

	SET_STATUS_FUNC(FIGHTER_STATUS_THROW_TECH, &Fighter::status_throw_tech);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_THROW_TECH, &Fighter::enter_status_throw_tech);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_THROW_TECH, &Fighter::exit_status_throw_tech);

	SET_STATUS_FUNC(FIGHTER_STATUS_BLOCKSTUN, &Fighter::status_blockstun);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_BLOCKSTUN, &Fighter::enter_status_blockstun);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_BLOCKSTUN, &Fighter::exit_status_blockstun);

	SET_STATUS_FUNC(FIGHTER_STATUS_HITSTUN, &Fighter::status_hitstun);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_HITSTUN, &Fighter::enter_status_hitstun);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_HITSTUN, &Fighter::exit_status_hitstun);

	SET_STATUS_FUNC(FIGHTER_STATUS_HITSTUN_AIR, &Fighter::status_hitstun_air);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_HITSTUN_AIR, &Fighter::enter_status_hitstun_air);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_HITSTUN_AIR, &Fighter::exit_status_hitstun_air);

	SET_STATUS_FUNC(FIGHTER_STATUS_HITSTUN_FLOAT, &Fighter::status_hitstun_float);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_HITSTUN_FLOAT, &Fighter::enter_status_hitstun_float);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_HITSTUN_FLOAT, &Fighter::exit_status_hitstun_float);

	SET_STATUS_FUNC(FIGHTER_STATUS_LAUNCH_START, &Fighter::status_launch_start);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_LAUNCH_START, &Fighter::enter_status_launch_start);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_LAUNCH_START, &Fighter::exit_status_launch_start);

	SET_STATUS_FUNC(FIGHTER_STATUS_LAUNCH, &Fighter::status_launch);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_LAUNCH, &Fighter::enter_status_launch);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_LAUNCH, &Fighter::exit_status_launch);

	SET_STATUS_FUNC(FIGHTER_STATUS_CRUMPLE, &Fighter::status_crumple);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_CRUMPLE, &Fighter::enter_status_crumple);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_CRUMPLE, &Fighter::exit_status_crumple);

	SET_STATUS_FUNC(FIGHTER_STATUS_KNOCKDOWN_START, &Fighter::status_knockdown_start);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_KNOCKDOWN_START, &Fighter::enter_status_knockdown_start);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_KNOCKDOWN_START, &Fighter::exit_status_knockdown_start);

	SET_STATUS_FUNC(FIGHTER_STATUS_KNOCKDOWN, &Fighter::status_knockdown);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_KNOCKDOWN, &Fighter::enter_status_knockdown);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_KNOCKDOWN, &Fighter::exit_status_knockdown);

	SET_STATUS_FUNC(FIGHTER_STATUS_WAKEUP, &Fighter::status_wakeup);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_WAKEUP, &Fighter::enter_status_wakeup);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_WAKEUP, &Fighter::exit_status_wakeup);

	SET_STATUS_FUNC(FIGHTER_STATUS_PARRY_START, &Fighter::status_parry_start);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_PARRY_START, &Fighter::enter_status_parry_start);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_PARRY_START, &Fighter::exit_status_parry_start);

	SET_STATUS_FUNC(FIGHTER_STATUS_HITSTUN_PARRY_START, &Fighter::status_hitstun_parry_start);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_HITSTUN_PARRY_START, &Fighter::enter_status_hitstun_parry_start);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_HITSTUN_PARRY_START, &Fighter::exit_status_hitstun_parry_start);

	SET_STATUS_FUNC(FIGHTER_STATUS_LAUNCH_PARRY_START, &Fighter::status_launch_parry_start);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_LAUNCH_PARRY_START, &Fighter::enter_status_launch_parry_start);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_LAUNCH_PARRY_START, &Fighter::exit_status_launch_parry_start);

	SET_STATUS_FUNC(FIGHTER_STATUS_PARRY, &Fighter::status_parry);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_PARRY, &Fighter::enter_status_parry);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_PARRY, &Fighter::exit_status_parry);

	SET_STATUS_FUNC(FIGHTER_STATUS_LANDING, &Fighter::status_landing);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_LANDING, &Fighter::enter_status_landing);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_LANDING, &Fighter::exit_status_landing);

	SET_STATUS_FUNC(FIGHTER_STATUS_LANDING_ATTACK, &Fighter::status_landing_attack);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_LANDING_ATTACK, &Fighter::enter_status_landing_attack);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_LANDING_ATTACK, &Fighter::exit_status_landing_attack);

	SET_STATUS_FUNC(FIGHTER_STATUS_LANDING_HITSTUN, &Fighter::status_landing_hitstun);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_LANDING_HITSTUN, &Fighter::enter_status_landing_hitstun);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_LANDING_HITSTUN, &Fighter::exit_status_landing_hitstun);

	SET_STATUS_FUNC(FIGHTER_STATUS_TAUNT, &Fighter::status_taunt);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_TAUNT, &Fighter::enter_status_taunt);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_TAUNT, &Fighter::exit_status_taunt);

	SET_STATUS_FUNC(FIGHTER_STATUS_KO, &Fighter::status_ko);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_KO, &Fighter::enter_status_ko);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_KO, &Fighter::exit_status_ko);

	SET_STATUS_FUNC(FIGHTER_STATUS_ROUND_END, &Fighter::status_round_end);
	SET_ENTRY_STATUS_FUNC(FIGHTER_STATUS_ROUND_END, &Fighter::enter_status_round_end);
	SET_EXIT_STATUS_FUNC(FIGHTER_STATUS_ROUND_END, &Fighter::exit_status_round_end);
}