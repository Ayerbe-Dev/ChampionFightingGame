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

bool Fighter::common_ground_status_act(bool crouch) {
	unsigned int stick_dir = get_stick_dir();
	unsigned int buffer_stick_dir = get_buffer_stick_dir();
	if (stick_dir < 4) {
		fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
	}
	else {
		fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
	}
	unsigned int grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
	if (check_button_input(grab_buttons, 2)
		|| check_button_input(BUTTON_LP) && check_button_on(BUTTON_LK)
		|| check_button_input(BUTTON_LK) && check_button_on(BUTTON_LP)) {
		return change_status(FIGHTER_STATUS_GRAB_START);
	}
	unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
	if (check_button_input(parry_buttons, 2)
		|| check_button_input(BUTTON_MP) && check_button_on(BUTTON_MK)
		|| check_button_input(BUTTON_MK) && check_button_on(BUTTON_MP)) {
		return change_status(FIGHTER_STATUS_PARRY_START);
	}
	unsigned int advance_buttons[2] = { BUTTON_HP, BUTTON_HK };
	if ((check_button_input(advance_buttons, 2)
		|| check_button_input(BUTTON_HP) && check_button_on(BUTTON_HK)
		|| check_button_input(BUTTON_HK) && check_button_on(BUTTON_HP))
		&& player->control_type == CONTROL_TYPE_ADVANCE) {
		switch (stick_dir) {
			case (6): {
				if (can_spend_ex(100.0)) {
					return change_status(FIGHTER_STATUS_ADVANCE_FORWARD, true, false);
				}
			} break;
			case (4): {
				if (can_spend_ex(100.0)) {
					return change_status(FIGHTER_STATUS_ADVANCE_BACK, true, false);
				}
			} break;
			default: {
				if (can_spend_ex(100.0)) {
					return change_status(FIGHTER_STATUS_ADVANCE, true, false);
				}
			} break;
		}
	}
	if (chara_ground_status_act()) {
		return true;
	}
	std::array<bool, 6> inputs = { 
		check_button_input(BUTTON_LP),
		check_button_input(BUTTON_MP),
		check_button_input(BUTTON_HP),
		check_button_input(BUTTON_LK),
		check_button_input(BUTTON_MK),
		check_button_input(BUTTON_HK) 
	};
	if (std::find(std::begin(inputs), std::end(inputs), true) != std::end(inputs)) {
		unsigned int all_buttons[6] = { BUTTON_LP, BUTTON_LK, BUTTON_MP, BUTTON_MK, BUTTON_HP, BUTTON_HK };
		if (check_button_input(all_buttons, 6)) {
			return change_status(FIGHTER_STATUS_TAUNT);
		}
		for (int i = 5; i >= 0; i--) {
			if (inputs[i]) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = i;
				break;
			}
		}
		if (buffer_stick_dir < 4) {
			fighter_int[FIGHTER_INT_ATTACK_KIND] += ATTACK_KIND_CLP;
		}
	
		if (change_status(FIGHTER_STATUS_ATTACK, true, false)) {
			//We don't enable these for chained normals, so it can't just go in the move scripts
			//or the attack status.
			switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
				case (ATTACK_KIND_LP):
				case (ATTACK_KIND_LK):
				case (ATTACK_KIND_CLP):
				case (ATTACK_KIND_CLK): {
					enable_cancel(CANCEL_CAT_WHIFF, CANCEL_KIND_GRAB);
				} break;
				case (ATTACK_KIND_MP):
				case (ATTACK_KIND_MK):
				case (ATTACK_KIND_CMP):
				case (ATTACK_KIND_CMK): {
					enable_cancel(CANCEL_CAT_WHIFF, CANCEL_KIND_PARRY_START);
				} break;
				case (ATTACK_KIND_HP):
				case (ATTACK_KIND_HK):
				case (ATTACK_KIND_CHP):
				case (ATTACK_KIND_CHK): {
					enable_cancel(CANCEL_CAT_WHIFF, CANCEL_KIND_ADVANCE);
					enable_cancel(CANCEL_CAT_WHIFF, CANCEL_KIND_ADVANCE_FORWARD);
					enable_cancel(CANCEL_CAT_WHIFF, CANCEL_KIND_ADVANCE_BACK);
				} break;
				default: {
				} break;
			}
			return true;
		}
		else {
			return false;
		}
	}
	switch (stick_dir) {
		case (1):
		case (2):
		case (3): {
			if (crouch && status_kind != FIGHTER_STATUS_CROUCH) {
				if (status_kind == FIGHTER_STATUS_ATTACK && fighter_int[FIGHTER_INT_ATTACK_KIND] >= ATTACK_KIND_HK) {
					return change_status(FIGHTER_STATUS_CROUCH);
				}
				else {
					return change_status(FIGHTER_STATUS_CROUCH_D);
				}
			}
		} break;
		case (4): {
			if (fighter_int[FIGHTER_INT_DASH_B_BUFFER] != 0) {
				return change_status(FIGHTER_STATUS_DASH_B);
			}
			else {
				return change_status(FIGHTER_STATUS_WALK_B);
			}
		} break;
		case (5):
		default: {
	
		} break;
		case (6): {
			if (fighter_int[FIGHTER_INT_DASH_F_BUFFER] != 0) {
				return change_status(FIGHTER_STATUS_DASH_F);
			}
			else {
				return change_status(FIGHTER_STATUS_WALK_F);
			}
		} break;
		case (7):
		case (8):
		case (9): {
			return change_status(FIGHTER_STATUS_JUMPSQUAT);
		} break;
	}
	return false;
}

bool Fighter::common_air_status_act() {
	if (chara_air_status_act()) {
		return true;
	}
	std::array<bool, 6> inputs = {
		check_button_input(BUTTON_LP),
		check_button_input(BUTTON_MP),
		check_button_input(BUTTON_HP),
		check_button_input(BUTTON_LK),
		check_button_input(BUTTON_MK),
		check_button_input(BUTTON_HK)
	};
	if (std::find(std::begin(inputs), std::end(inputs), true) != std::end(inputs)) {
		unsigned int grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
		if (check_button_input(grab_buttons, 2)
			|| check_button_input(BUTTON_LP) && check_button_on(BUTTON_LK)
			|| check_button_input(BUTTON_LK) && check_button_on(BUTTON_LP)) {
			return change_status(FIGHTER_STATUS_GRAB_AIR_START);
		}
		unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)
			|| check_button_input(BUTTON_MP) && check_button_on(BUTTON_MK)
			|| check_button_input(BUTTON_MK) && check_button_on(BUTTON_MP)) {
			return change_status(FIGHTER_STATUS_PARRY_START);
		}
		for (int i = 5; i >= 0; i--) {
			if (inputs[i]) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = i;
				break;
			}
		}
		if (change_status(FIGHTER_STATUS_ATTACK_AIR, false, false)) {
			switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
				case (ATTACK_KIND_LP):
				case (ATTACK_KIND_LK): {
					enable_cancel(CANCEL_CAT_WHIFF, CANCEL_KIND_GRAB);
				} break;
				case (ATTACK_KIND_MP):
				case (ATTACK_KIND_MK): {
					enable_cancel(CANCEL_CAT_WHIFF, CANCEL_KIND_PARRY_START);
				} break;
				default: {

				} break;
			}
			return true;
		}
	}
	if (fighter_int[FIGHTER_INT_DASH_F_BUFFER] != 0) {
		fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 1;
		return change_status(FIGHTER_STATUS_DASH_AIR);
	}
	if (fighter_int[FIGHTER_INT_DASH_B_BUFFER] != 0) {
		fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 2;
		return change_status(FIGHTER_STATUS_DASH_AIR);
	}
	return false;
}

bool Fighter::common_air_status_general() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		apply_gravity(get_local_param_float("gravity"), get_local_param_float("max_fall_speed"));
		add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED], 0));
	}
	if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] < 0) {
		fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT] = false;
	}
	return false;
}

void Fighter::status_none() {
	change_status(FIGHTER_STATUS_WAIT);
}

void Fighter::enter_status_none() {

}

void Fighter::exit_status_none() {

}

void Fighter::status_wait() {
	if (!fighter_flag[FIGHTER_FLAG_ROUND_START]) {
		common_ground_status_act();
	}
}


void Fighter::enter_status_wait() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_NONE;
	pos.y = 0.0f;
	pos.z = 0.0f;
	change_anim("wait");

	situation_kind = FIGHTER_SITUATION_GROUND;
	fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
	if (!fighter_flag[FIGHTER_FLAG_ROUND_START]) {
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
	add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], 0, 0));

}

void Fighter::enter_status_walk_f() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("walk_f_speed") * facing_dir;
	change_anim("walk_f");
}

void Fighter::exit_status_walk_f() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Fighter::status_walk_b() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	if (fighter_flag[FIGHTER_FLAG_PROX_GUARD]) {
		if (get_anim() != "stand_block") {
			if (get_anim() == "stand_block_start") {
				if (anim_end) {
					change_anim("stand_block");
				}
			}
			else {
				change_anim("stand_block_start");
			}
		}
	}
	else {
		if (get_anim() != "walk_b") {
			change_anim("walk_b");
		}
		add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], 0, 0));
	}
}

void Fighter::enter_status_walk_b() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("walk_b_speed") * -facing_dir;
	change_anim("walk_b");
}

void Fighter::exit_status_walk_b() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Fighter::status_dash_f() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_local_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_local_param_int("dash_f_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("dash_f_speed") * facing_dir;
	}
	else if (frame < max_frame) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = (get_local_param_float("walk_f_speed") + get_local_param_float("dash_f_speed")) * facing_dir / 2;
	}
	else {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("walk_f_speed") * facing_dir;
	}
	add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], 0, 0));

	if (frame >= get_local_param_int("dash_f_cancel_frame") && get_stick_dir(false) == 4) {
		switch (get_local_param_int("dash_cancel_kind")) {
			case (DASH_CANCEL_KIND_INFINITE): {
				change_status(FIGHTER_STATUS_DASH_B, false);
				return;
			} break;
			case (DASH_CANCEL_KIND_WHIFF_PUNISH_OFFENSIVE): {
				if (!fighter_flag[FIGHTER_FLAG_DASH_CANCEL]) {
					fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = true;
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
}

void Fighter::exit_status_dash_f() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Fighter::status_dash_b() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_local_param_int("dash_b_accel_frame");
	int max_frame = min_frame + get_local_param_int("dash_b_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("dash_b_speed") * facing_dir * -1;
	}
	else if (frame < max_frame) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = (get_local_param_float("walk_b_speed") + get_local_param_float("dash_b_speed")) * facing_dir / -2;
	}
	else {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("walk_b_speed") * -facing_dir;
	}
	add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], 0, 0));

	if (frame >= get_local_param_int("dash_b_cancel_frame") && get_stick_dir(false) == 6) {
		switch (get_local_param_int("dash_cancel_kind")) {
			case (DASH_CANCEL_KIND_INFINITE): {
				change_status(FIGHTER_STATUS_DASH_F);
				return;
			} break;
			case (DASH_CANCEL_KIND_WHIFF_PUNISH_OFFENSIVE): {
				std::array<bool, 6> inputs = {
					check_button_input(BUTTON_LP),
					check_button_input(BUTTON_MP),
					check_button_input(BUTTON_HP),
					check_button_input(BUTTON_LK),
					check_button_input(BUTTON_MK),
					check_button_input(BUTTON_HK)
				};
				if (std::find(std::begin(inputs), std::end(inputs), true) != std::end(inputs)) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_OTHER;
					change_status(FIGHTER_STATUS_BACKDASH_ATTACK);
					return;
				}
			} break;
			case (DASH_CANCEL_KIND_WHIFF_PUNISH_DEFENSIVE): {
				if (check_button_input(BUTTON_HP) || check_button_input(BUTTON_HK)) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HP;
					change_status(FIGHTER_STATUS_BACKDASH_ATTACK);
					return;
				}
				if (check_button_input(BUTTON_MP) || check_button_input(BUTTON_MK)) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MP;
					change_status(FIGHTER_STATUS_BACKDASH_ATTACK);
					return;
				}
				if (check_button_input(BUTTON_LP) || check_button_input(BUTTON_LK)) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LP;
					change_status(FIGHTER_STATUS_BACKDASH_ATTACK);
					return;
				}
			} break;
			case (DASH_CANCEL_KIND_NONE):
			default: {

			} break;
		}
	}
}

void Fighter::enter_status_dash_b() {
	change_anim("dash_b");
}

void Fighter::exit_status_dash_b() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
}

void Fighter::status_dash_air() {
	if (check_landing()) {
		return;
	}
	if (is_status_end()) {
		return;
	}
	int min_frame = get_local_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_local_param_int("dash_f_maintain_speed_frame");

	if (frame < max_frame) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0.0;
	}
	else {
		if (fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] > get_local_param_float("jump_x_speed")) {
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = clampf(get_local_param_float("jump_x_speed"), fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * 0.95, fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED]);
		}
	}
	common_air_status_general();
}

void Fighter::enter_status_dash_air() {
	if (fighter_int[FIGHTER_INT_DASH_AIR_DIR] == 1) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("dash_f_speed") * facing_dir;
		fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_F;
		change_anim("dash_air_f");
	}
	if (fighter_int[FIGHTER_INT_DASH_AIR_DIR] == 2) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("dash_b_speed") * -facing_dir;
		fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_B;
		change_anim("dash_air_b");
	}
	fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = true;
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
	change_anim("crouch_d");
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
	change_anim("crouch");
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
	change_anim("crouch_u");
}

void Fighter::exit_status_crouchu() {}

void Fighter::status_jumpsquat() {
	if (is_status_end(FIGHTER_STATUS_JUMP)) {
		return;
	}
	if (frame < 3.0) {
		unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)) {
			fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_N;
			change_status(FIGHTER_STATUS_PARRY_START, false);
			//Since jumpsquat is mostly intangible and instant shorthop parry would otherwise beat 
			//all meaties, we use this check to make instant shorthop parry a 2f input against
			//meaty antiairs. If you mess it up, you're forced to parry in the wrong direction and
			//are susceptible to the same counterhits as whiffing a jump.
			change_anim("parry_start_high");
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
			fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT] = true;
			return;
		}
		if (chara_ground_status_act()) {
			situation_kind = FIGHTER_SITUATION_GROUND;
			return;
		}
	}
	unsigned int stick_dir = get_stick_dir() % 3;
	if (stick_dir == 1) {
		fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_B;
	}
	else if (stick_dir == 0) {
		fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_F;
	}
}

void Fighter::enter_status_jumpsquat() {
	fighter_int[FIGHTER_INT_JUMP_KIND] = get_stick_dir() % 3;
	change_anim("jump_squat");
}

void Fighter::exit_status_jumpsquat() {
	fighter_flag[FIGHTER_FLAG_SHORT_HOP] = get_stick_dir() < 7;
	fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = false;
	situation_kind = FIGHTER_SITUATION_AIR;
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
	fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT] = true;
	if (fighter_flag[FIGHTER_FLAG_SHORT_HOP]) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_local_param_float("jump_y_init_speed_s");
	}
	else {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_local_param_float("jump_y_init_speed");
	}
	switch (fighter_int[FIGHTER_INT_JUMP_KIND]) {
		case (JUMP_KIND_F): {
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("jump_x_speed") * facing_dir;
		} break;
		case (JUMP_KIND_B): {
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("jump_x_speed") * -facing_dir;
		} break;
		case (JUMP_KIND_N): {
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
		} break;
	}
	if (common_air_status_act()) {
		return;
	}
	switch (fighter_int[FIGHTER_INT_JUMP_KIND]) {
		case (JUMP_KIND_F): {
			change_anim("jump_f", 1.0, 0.0);
		} break;
		case (JUMP_KIND_B): {
			change_anim("jump_b", 1.0, 0.0);
		} break;
		case (JUMP_KIND_N): {
			change_anim("jump", 1.0, 0.0);
		} break;
	}
}

void Fighter::exit_status_jump() {
	fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT] = false;
}

void Fighter::status_fall() {
	if (common_air_status_act()) {
		return;
	}
	if (check_landing()) {
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_fall() {
	change_anim("fall");
}


void Fighter::exit_status_fall() {
	fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_N;
}

void Fighter::status_turn() {
	if (common_ground_status_act(false)) {
		return;
	}
	if (get_stick_dir() > 3) {
		if (anim_end) {
			change_status(FIGHTER_STATUS_WAIT);
			return;
		}
		if (get_anim() == "turn_crouch") {
			change_anim_inherit_attributes("turn_stand", false);
		}
	}
	else {
		if (anim_end) {
			change_status(FIGHTER_STATUS_CROUCH);
			return;
		}
		if (get_anim() == "turn_stand") {
			change_anim_inherit_attributes("turn_crouch", false);
		}
	}
}

void Fighter::enter_status_turn() {
	//First we want to make sure you don't accidentally dash because you inputted
	//one direction, turned around, then inputted the other direction
	facing_right = internal_facing_right;
	if (facing_right) {
		facing_dir = 1.0;
	}
	else {
		facing_dir = -1.0;
	}
	int dash_f = fighter_int[FIGHTER_INT_DASH_F_WINDOW];
	int dash_b = fighter_int[FIGHTER_INT_DASH_B_WINDOW];
	fighter_int[FIGHTER_INT_DASH_F_WINDOW] = 0;
	fighter_int[FIGHTER_INT_DASH_B_WINDOW] = 0;
	if (common_ground_status_act(false)) { //Rare footage of recursive change_status calls
		return;
	}
	fighter_int[FIGHTER_INT_DASH_F_WINDOW] = dash_b;
	fighter_int[FIGHTER_INT_DASH_B_WINDOW] = dash_f;
	if (get_stick_dir() < 4) {
		change_anim("turn_crouch", 1.0, 0.0);
	}
	else {
		change_anim("turn_stand", 1.0, 0.0);
	}
}


void Fighter::exit_status_turn() {

}

void Fighter::status_attack() {
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] > ATTACK_KIND_HK 
		&& fighter_int[FIGHTER_INT_ATTACK_KIND] != ATTACK_KIND_OTHER) { //If we're in a crouching attack, 
		//change to the crouch animation on animation end. 
		if (is_status_end(FIGHTER_STATUS_CROUCH)) {
			return;
		}
	}
	else if (is_status_end()) {
		return;
	}
	unsigned int grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
	if (is_enable_cancel(CANCEL_KIND_GRAB)) {
		bool input = check_button_input(grab_buttons, 2);
		if (fighter_int[FIGHTER_INT_EXTERNAL_FRAME] < 3) {
			switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
				case (ATTACK_KIND_LP):
				case (ATTACK_KIND_CLP): {
					if (check_button_input(BUTTON_LK) && check_button_on(BUTTON_LP)) {
						input = true;
					}
				} break;
				case (ATTACK_KIND_LK):
				case (ATTACK_KIND_CLK): {
					if (check_button_input(BUTTON_LP) && check_button_on(BUTTON_LK)) {
						input = true;
					}
				} break;
				default: {

				} break;
			}
		}
		if (input) {
			buffer_change_status(FIGHTER_STATUS_GRAB_START);
			return;
		}
	}
	unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
	if (is_enable_cancel(CANCEL_KIND_PARRY_START)) {
		bool input = check_button_input(parry_buttons, 2);
		if (fighter_int[FIGHTER_INT_EXTERNAL_FRAME] < 3) {
			switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
			case (ATTACK_KIND_MP):
			case (ATTACK_KIND_CMP): {
				if (check_button_input(BUTTON_MK) && check_button_on(BUTTON_MP)) {
					input = true;
				}
			} break;
			case (ATTACK_KIND_MK):
			case (ATTACK_KIND_CMK): {
				if (check_button_input(BUTTON_MP) && check_button_on(BUTTON_MK)) {
					input = true;
				}
			} break;
			default: {

			} break;
			}
		}
		if (input) {
			buffer_change_status(FIGHTER_STATUS_PARRY_START);
			return;
		}
	}
	bool advance_price_met = can_spend_ex(100.0f);
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT] || fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED]) {
		advance_price_met = can_spend_ex(300.0f);
	}
	unsigned int advance_buttons[2] = { BUTTON_HP, BUTTON_HK };
	if ((check_button_input(advance_buttons, 2)
		|| check_button_input(BUTTON_HP) && check_button_on(BUTTON_HK)
		|| check_button_input(BUTTON_HK) && check_button_on(BUTTON_HP))
		&& player->control_type == CONTROL_TYPE_ADVANCE) {
		switch (get_stick_dir()) {
			case (6): {
				if (is_enable_cancel(CANCEL_KIND_ADVANCE_FORWARD) && advance_price_met) {
					fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = fighter_int[FIGHTER_INT_EXTERNAL_FRAME] >= 3;
					if (buffer_change_status(FIGHTER_STATUS_ADVANCE_FORWARD)) {
						return;
					}
				}
			} break;
			case (4): {
				if (is_enable_cancel(CANCEL_KIND_ADVANCE_BACK) && advance_price_met) {
					fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = fighter_int[FIGHTER_INT_EXTERNAL_FRAME] >= 3;
					if (buffer_change_status(FIGHTER_STATUS_ADVANCE_BACK)) {
						return;
					}
				}
			} break;
			default: {
				if (is_enable_cancel(CANCEL_KIND_ADVANCE) && advance_price_met) {
					fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = fighter_int[FIGHTER_INT_EXTERNAL_FRAME] >= 3;
					if (buffer_change_status(FIGHTER_STATUS_ADVANCE)) {
						return;
					}
				}
			} break;
		}
	}
	if (chara_status_attack()) {
		return;
	}
	for (int i = 0; i < 12; i++) {
		if (attack_cancel(i)) {
			return;
		}
	}
	if (get_stick_dir() > 6) {
		buffer_change_status(FIGHTER_STATUS_JUMPSQUAT, CANCEL_KIND_JUMP);
	}
	if (fighter_int[FIGHTER_INT_DASH_F_BUFFER]) {
		buffer_change_status(FIGHTER_STATUS_DASH_F, CANCEL_KIND_DASH_F);
		return;
	}
	if (fighter_int[FIGHTER_INT_DASH_B_BUFFER]) {
		buffer_change_status(FIGHTER_STATUS_DASH_B, CANCEL_KIND_DASH_B);
		return;
	}
}

void Fighter::enter_status_attack() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_OTHER) {
		chara_enter_status_attack_other();
	}
	else {
		fighter_int[FIGHTER_INT_ATTACK_OTHER_KIND] = 0;
		switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
			case (ATTACK_KIND_LP): {
				change_anim("stand_lp");
			} break;
			case (ATTACK_KIND_MP): {
				change_anim("stand_mp");
			} break;
			case (ATTACK_KIND_HP): {
				change_anim("stand_hp");
			} break;
			case (ATTACK_KIND_LK): {
				change_anim("stand_lk");
			} break;
			case (ATTACK_KIND_MK): {
				change_anim("stand_mk");
			} break;
			case (ATTACK_KIND_HK): {
				change_anim("stand_hk");
			} break;
			case (ATTACK_KIND_CLP): {
				change_anim("crouch_lp");
			} break;
			case (ATTACK_KIND_CMP): {
				change_anim("crouch_mp");
			} break;
			case (ATTACK_KIND_CHP): {
				change_anim("crouch_hp");
			} break;
			case (ATTACK_KIND_CLK): {
				change_anim("crouch_lk");
			} break;
			case (ATTACK_KIND_CMK): {
				change_anim("crouch_mk");
			} break;
			case (ATTACK_KIND_CHK): {
				change_anim("crouch_hk");
			} break;
			default: {

			} break;
		}
	}
}

void Fighter::exit_status_attack() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT] = false;
	stop_reserved_sound();
	clear_effect_all();
}

void Fighter::status_attack_air() {
	fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT] = fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT];
	if (check_landing(FIGHTER_STATUS_LANDING_ATTACK)) {
		return;
	}
	if (chara_status_attack_air()) {
		return;
	}
	if (is_status_end()) {
		return;
	}
	unsigned int grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
	if (is_enable_cancel(CANCEL_KIND_GRAB)) {
		bool input = check_button_input(grab_buttons, 2);
		if (fighter_int[FIGHTER_INT_EXTERNAL_FRAME] < 3) {
			switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
				case (ATTACK_KIND_LP): {
					if (check_button_input(BUTTON_LK) && check_button_on(BUTTON_LP)) {
						input = true;
					}
				} break;
				case (ATTACK_KIND_LK): {
					if (check_button_input(BUTTON_LP) && check_button_on(BUTTON_LK)) {
						input = true;
					}
				} break;
				default: {

				} break;
			}
		}
		if (input) {
			buffer_change_status(FIGHTER_STATUS_GRAB_AIR_START);
			return;
		}
	}
	unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
	if (is_enable_cancel(CANCEL_KIND_PARRY_START)) {
		bool input = check_button_input(parry_buttons, 2);
		if (fighter_int[FIGHTER_INT_EXTERNAL_FRAME] < 3) {
			switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
				case (ATTACK_KIND_MP): {
					if (check_button_input(BUTTON_MK) && check_button_on(BUTTON_MP)) {
						input = true;
					}
				} break;
				case (ATTACK_KIND_MK): {
					if (check_button_input(BUTTON_MP) && check_button_on(BUTTON_MK)) {
						input = true;
					}
				} break;
				default: {

				} break;
			}
		}
		if (input) {
			buffer_change_status(FIGHTER_STATUS_PARRY_START);
			return;
		}
	}
	for (int i = 0; i < 6; i++) {
		if (attack_cancel(i)) {
			return;
		}
	}
	if (fighter_int[FIGHTER_INT_DASH_F_BUFFER] > fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 1;
		buffer_change_status(FIGHTER_STATUS_DASH_AIR, CANCEL_KIND_DASH_F);
		return;
	}
	if (fighter_int[FIGHTER_INT_DASH_B_BUFFER] > fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 2;
		buffer_change_status(FIGHTER_STATUS_DASH_AIR, CANCEL_KIND_DASH_B);
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_attack_air() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_OTHER) {
		chara_enter_status_attack_air_other();
	}
	else {
		fighter_int[FIGHTER_INT_ATTACK_OTHER_KIND] = 0;
		switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
			case (ATTACK_KIND_LP): {
				change_anim("jump_lp");
			} break;
			case (ATTACK_KIND_MP): {
				change_anim("jump_mp");
			} break;
			case (ATTACK_KIND_HP): {
				change_anim("jump_hp");
			} break;
			case (ATTACK_KIND_LK): {
				change_anim("jump_lk");
			} break;
			case (ATTACK_KIND_MK): {
				change_anim("jump_mk");
			} break;
			case (ATTACK_KIND_HK): {
				change_anim("jump_hk");
			} break;
			default: {

			} break;
		}
	}
}

void Fighter::exit_status_attack_air() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	stop_reserved_sound();
	clear_effect_all();
}

void Fighter::status_backdash_attack() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_backdash_attack() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
	switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
		case (ATTACK_KIND_LP): {
			change_anim("backdash_attack_l");
		} break;
		case (ATTACK_KIND_MP): {
			change_anim("backdash_attack_m");
		} break;
		case (ATTACK_KIND_HP): {
			change_anim("backdash_attack_h");
		} break;
		case (ATTACK_KIND_OTHER): {
			change_anim("backdash_attack");
		} break;
		default: {

		} break;
	}
}

void Fighter::exit_status_backdash_attack() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	stop_reserved_sound();
	clear_effect_all();
}

void Fighter::status_advance() {
	if (is_status_end()) {
		return;
	}
	if (frame < get_local_param_float("advance_low_transition_frame") && get_stick_dir() < 4) {
		change_anim_inherit_attributes("advance_low", false);
	}
}

void Fighter::enter_status_advance() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_OTHER;
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 0;
	fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 0;
	if (fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE]) {
		spend_ex(300.0);
	}
	else {
		spend_ex(100.0);
	}
	change_anim("advance_high");
}

void Fighter::exit_status_advance() {
	fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = false;
}

void Fighter::status_advance_forward() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_advance_forward() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_OTHER;
	if (fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE]) {
		spend_ex(300.0);
	}
	else {
		spend_ex(100.0);
	}
	change_anim("advance_forward");
}

void Fighter::exit_status_advance_forward() {
	fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = false;
}

void Fighter::status_advance_back() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_advance_back() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_OTHER;
	if (fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE]) {
		spend_ex(300.0);
	}
	else {
		spend_ex(100.0);
	}
	change_anim("advance_back");
}

void Fighter::exit_status_advance_back() {
	fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = false;
}

void Fighter::status_grab_start() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_grab_start() {
	fighter_flag[FIGHTER_FLAG_THROW_TECH] = true;
	change_anim("grab_start");
}

void Fighter::exit_status_grab_start() {}

void Fighter::status_grab() {
	if (is_status_end(FIGHTER_STATUS_THROW)) {
		return;
	}
}

void Fighter::enter_status_grab() {
	change_anim("grab");
}

void Fighter::exit_status_grab() {

}

void Fighter::status_throw() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_throw() {
	fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = true;
	change_anim(throw_map_ground[get_stick_dir()]);
}

void Fighter::exit_status_throw() {
	fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = false;
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
	fighter_flag[FIGHTER_FLAG_THROW_TECH] = true;
	change_anim("grab_air_start");
}

void Fighter::exit_status_grab_air_start() {}

void Fighter::status_grab_air() {
	if (is_status_end(FIGHTER_STATUS_THROW_AIR)) {
		return;
	}
}

void Fighter::enter_status_grab_air() {
	change_anim("grab_air");
}

void Fighter::exit_status_grab_air() {

}

void Fighter::status_throw_air() {
	if (pos.y <= 0.0f) {
		pos.y = 0.0f;
		situation_kind = FIGHTER_SITUATION_GROUND;
	}
	if (is_status_end()) {
		return;
	}
	if (!fighter_flag[FIGHTER_FLAG_DISABLE_AIR_GENERAL]) {
		common_air_status_general();
	}
}

void Fighter::enter_status_throw_air() {
	change_anim(throw_map_air[get_stick_dir()]);
}

void Fighter::exit_status_throw_air() {
	fighter_flag[FIGHTER_FLAG_DISABLE_AIR_GENERAL] = false;
	if (fighter_flag[FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END] && fighter_int[FIGHTER_INT_JUMP_KIND] != JUMP_KIND_N) {
		fighter_int[FIGHTER_INT_JUMP_KIND] = !((bool)(fighter_int[FIGHTER_INT_JUMP_KIND]));
	}
}

void Fighter::status_grabbed() {
	Fighter* that = object_manager->fighter[!id];
	if (fighter_flag[FIGHTER_FLAG_IN_THROW_TECH_WINDOW] && fighter_flag[FIGHTER_FLAG_ALLOW_THROW_TECH]) {
		unsigned int grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
		if (check_button_input(grab_buttons, 2)) {
			change_status(FIGHTER_STATUS_THROW_TECH);
			that->change_status(FIGHTER_STATUS_THROW_TECH);
			return;
		}
	}
	glm::vec3 offset = glm::vec3(
		fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_X], 
		fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_Y], 
		fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_Z]
	);
	glm::vec3 offset_bone_pos = get_relative_bone_position(fighter_int[FIGHTER_INT_GRABBED_BONE_ID]);
	glm::vec3 target_pos_per_frame = that->get_bone_position(fighter_int[FIGHTER_INT_GRAB_BONE_ID], offset);
	set_pos(target_pos_per_frame - offset_bone_pos);
}

void Fighter::enter_status_grabbed() {
	fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = true;
	fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION] = true;
	fighter_flag[FIGHTER_FLAG_ALLOW_THROW_TECH] = !fighter_flag[FIGHTER_FLAG_DISABLE_THROW_TECH] 
		&& (is_actionable() || fighter_flag[FIGHTER_FLAG_ATTACK_HITSTUN_PARRIED]);
	change_anim("grabbed");
}

void Fighter::exit_status_grabbed() {
	fighter_flag[FIGHTER_FLAG_ALLOW_CROSSUP] = false;
	fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION] = false;
	pos.z = 0.0;
}

void Fighter::status_thrown() {
	if (check_landing(FIGHTER_STATUS_KNOCKDOWN)) {
		return;
	}
	apply_gravity(fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY], fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX]);
	if (!add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED], 0)) && pos.y != 0.0) {
		Fighter* that = object_manager->fighter[!id];
		//If you get thrown into the corner, the attacker should be pushed away from you.
		that->add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * -1.0, 0.0, 0.0));
	}
}

void Fighter::enter_status_thrown() {
	situation_kind = FIGHTER_SITUATION_AIR;
	fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y] = fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED];
	change_anim("thrown", calc_launch_frames(), -1.0);
}

void Fighter::exit_status_thrown() {

}

void Fighter::status_throw_tech() {
	if (is_status_end()) {
		return;
	}
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 15 || fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 16) {
		frame++;
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	}
	else if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] < 15) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = facing_dir * -16;
	}
	else {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = facing_dir * -4;
	}
	add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], 0, 0));
}

void Fighter::enter_status_throw_tech() {
	if (situation_kind == FIGHTER_SITUATION_GROUND) {
		change_anim("throw_tech");
	}
	else {
		change_anim("throw_tech_air");
	}
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 20;
	fighter_flag[FIGHTER_FLAG_IN_THROW_TECH_WINDOW] = false;
}

void Fighter::exit_status_throw_tech() {}

void Fighter::status_blockstun() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) {
		unsigned int advance_buttons[2] = { BUTTON_HP, BUTTON_HK };
		if (check_button_input(advance_buttons, 2) && player->control_type == CONTROL_TYPE_ADVANCE) {
			if ((get_stick_dir() == 4 || get_stick_dir() == 1) && can_spend_ex(300.0)) {
				fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 0;
				fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 0;
				fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = 0;
				fighter_flag[FIGHTER_FLAG_CANCEL_INTO_ADVANCE] = true;
				change_status(FIGHTER_STATUS_ADVANCE_BACK);
				return;
			}
		}
	}
	else {
		if (get_stick_dir() < 4) {
			fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
		}
		else {
			fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
		}
		if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
			if (get_stick_dir() < 4) {
				if (change_status(FIGHTER_STATUS_CROUCH)) {
					return;
				}
			}
			else {
				if (change_status(FIGHTER_STATUS_WAIT)) {
					return;
				}
			}
		}
	}

}


void Fighter::enter_status_blockstun() {
	if (get_stick_dir() < 4) {
		change_anim("crouch_blockstun", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
	}
	else if (fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] == HIT_HEIGHT_HIGH) {
		change_anim("high_blockstun", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
	}
	else {
		change_anim("stand_blockstun", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
	}
}

void Fighter::exit_status_blockstun() {}

void Fighter::status_hitstun() {
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
		switch (fighter_int[FIGHTER_INT_HITSTUN_HEIGHT]) {
			case (HITSTUN_HEIGHT_STAND): {
				if (change_status(FIGHTER_STATUS_WAIT)) {
					return;
				}
			} break;
			case (HITSTUN_HEIGHT_CROUCH): {
				if (change_status(FIGHTER_STATUS_CROUCH)) {
					return;
				}
			} break;
		}
	}
	else if (check_hitstun_parry()) {
		change_status(FIGHTER_STATUS_HITSTUN_PARRY_START);
		return;
	}
}

void Fighter::enter_status_hitstun() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	switch (fighter_int[FIGHTER_INT_HITSTUN_HEIGHT]) {
		case (HITSTUN_HEIGHT_STAND): {
			if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == HIT_LEVEL_LIGHT) {
				change_anim("stand_hitstun_l", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
			else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == HIT_LEVEL_MEDIUM) {
				change_anim("stand_hitstun_m", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
			else {
				change_anim("stand_hitstun_h", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
		} break;
		case (HITSTUN_HEIGHT_CROUCH): {
			if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == HIT_LEVEL_LIGHT) {
				change_anim("crouch_hitstun_l", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
			else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == HIT_LEVEL_MEDIUM) {
				change_anim("crouch_hitstun_m", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
			else {
				change_anim("crouch_hitstun_h", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
		} break;
		case (HITSTUN_HEIGHT_FORCE_STAND): {
			fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
		} break;
		case (HITSTUN_HEIGHT_FORCE_CROUCH): {
			fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
		} break;
		case (HITSTUN_HEIGHT_NONE): 
		default: {
			if (get_stick_dir() < 4) {
				if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == HIT_LEVEL_LIGHT) {
					change_anim("crouch_hitstun_l", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
				}
				else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == HIT_LEVEL_MEDIUM) {
					change_anim("crouch_hitstun_m", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
				}
				else {
					change_anim("crouch_hitstun_h", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
				}
			}
			else {
				if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == HIT_LEVEL_LIGHT) {
					change_anim("stand_hitstun_l", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
				}
				else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == HIT_LEVEL_MEDIUM) {
					change_anim("stand_hitstun_m", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
				}
				else {
					change_anim("stand_hitstun_h", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
				}
			}
		} break;
	}
}

void Fighter::exit_status_hitstun() {

}

void Fighter::status_hitstun_air() {
	Fighter* that = object_manager->fighter[!id];
	if (check_landing(FIGHTER_STATUS_LANDING_HITSTUN)) {
		return;
	}
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
		change_status(FIGHTER_STATUS_FALL, false);
		return;
	}
	else if (check_hitstun_parry()) {
		change_status(FIGHTER_STATUS_HITSTUN_PARRY_START);
		return;
	}
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		apply_gravity(get_local_param_float("gravity") * 2.0, get_local_param_float("max_fall_speed"));
		add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED], 0));
	}
}

void Fighter::enter_status_hitstun_air() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	change_anim("jump_hitstun", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
}


void Fighter::exit_status_hitstun_air() {

}

void Fighter::status_hitstun_float() {
	if (anim_end) {
		fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = true;
		fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = 0;
		change_status(FIGHTER_STATUS_KNOCKDOWN);
		return;
	}
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES]) {
		std::string param_text_add = "_up";
		if (fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
			param_text_add = "_down";
		}
		int end_rate_frame = get_local_param_int("float_hitstun_end_frame" + param_text_add);
		if (frame == get_local_param_int("float_hitstun_start_frame" + param_text_add) && rate == 1.0f) {
			set_rate((end_rate_frame - frame) / (float)fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else if (frame > end_rate_frame) {
			set_rate(1.0);
		}
	}
}

void Fighter::enter_status_hitstun_float() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	situation_kind = FIGHTER_SITUATION_AIR;
	if (fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("hitstun_float_down");
	}
	else {
		change_anim("hitstun_float_up");
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
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	change_anim("launch_start");
}

void Fighter::exit_status_launch_start() {
	situation_kind = FIGHTER_SITUATION_AIR;
}

void Fighter::status_launch() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		apply_gravity(fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY], fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX]);
		add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED], 0));
		if (fighter_flag[FIGHTER_FLAG_LAUNCH_FLOAT]) {
			if (check_landing(FIGHTER_STATUS_HITSTUN_FLOAT)) {
				fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = 0;
				fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = 0;
				return;
			}
		}
		else {
			if (check_landing(FIGHTER_STATUS_KNOCKDOWN)) {
				fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = 0;
				fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = 0;
				return;
			}
		}
	}
	if (check_hitstun_parry()) {
		change_status(FIGHTER_STATUS_LAUNCH_PARRY_START);
	}
}

void Fighter::enter_status_launch() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = false;
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X];
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y];
	if (!fighter_flag[FIGHTER_FLAG_LAUNCH_FLOAT]) {
		fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = 0;
	}
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = get_local_param_float("gravity");
	}
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = get_local_param_float("max_fall_speed");
	}
	if (fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN]) {
		change_anim("launch_hkd", calc_launch_frames(), -1.0);
	}
	else {
		change_anim("launch", calc_launch_frames(), -1.0);
	}
}

void Fighter::exit_status_launch() {
	fighter_flag[FIGHTER_FLAG_LAUNCH_FLOAT] = false;
}

void Fighter::status_crumple() {
	if (anim_end) {
		fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = true;
		fighter_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_DEFAULT;
		fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = 0;
		if (is_ko()) {
			change_status(FIGHTER_STATUS_KO);
		}
		else {
			change_status(FIGHTER_STATUS_WAKEUP);
		}
		return;
	}
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES]) {
		int end_rate_frame = get_local_param_int("crumple_hitstun_end_frame");
		if (frame == get_local_param_int("crumple_hitstun_start_frame") && rate == 1.0f) {
			set_rate((end_rate_frame - frame) / (float)fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else if (frame > end_rate_frame) {
			set_rate(1.0);
		}
	}
}

void Fighter::enter_status_crumple() {
	change_anim("crumple");
}

void Fighter::exit_status_crumple() {

}

void Fighter::status_knockdown_start() {
	if (situation_kind == FIGHTER_SITUATION_AIR && !fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES]) {
		pos.y = clampf(0.0f, pos.y - fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED], pos.y);
	}
	if (anim_end) {
		change_status(FIGHTER_STATUS_KNOCKDOWN);
	}
}

void Fighter::enter_status_knockdown_start() {
	if (fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("knockdown_start_down");
		if (situation_kind == FIGHTER_SITUATION_AIR) {
			int skip_frame = get_local_param_int("knockdown_start_air_skip_frame_down");
			fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = pos.y / (get_anim_length(get_anim()) -
				skip_frame
			);
			frame = skip_frame;
		}
	}
	else {
		change_anim("knockdown_start_up");
		if (situation_kind == FIGHTER_SITUATION_AIR) {
			int skip_frame = get_local_param_int("knockdown_start_air_skip_frame_up");
			fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = pos.y / (get_anim_length(get_anim()) -
				skip_frame
			);
			frame = skip_frame;
		}
	}
}

void Fighter::exit_status_knockdown_start() {

}

void Fighter::status_knockdown() {
	if (!(fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] || fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW])) {
		switch (get_flick_dir()) {
		case 4: {
			fighter_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_BACK;
			fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = get_param_int(PARAM_FIGHTER, "tech_window");
		} break;
		case 1:
		case 2: {
			fighter_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_FAST;
			fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] = get_param_int(PARAM_FIGHTER, "tech_window");
		} break;
		default: {
			fighter_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_DEFAULT;
		} break;
		}
	}
	if (anim_end) {
		fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = 0;
		if (is_ko()) {
			change_status(FIGHTER_STATUS_KO);
		}
		else {
			change_status(FIGHTER_STATUS_WAKEUP);
		}
		return;
	}
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES]) {
		std::string param_text_add = "_up";
		if (fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
			param_text_add = "_down";
		}
		int end_rate_frame = get_local_param_int("knockdown_hitstun_end_frame" + param_text_add);
		if (frame == get_local_param_int("knockdown_hitstun_start_frame" + param_text_add) && rate == 1.0f) {
			set_rate((end_rate_frame - frame) / (float)fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else if (frame > end_rate_frame) {
			set_rate(1.0);
		}
	}
}

void Fighter::enter_status_knockdown() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	if (fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("knockdown_down");
	}
	else {
		change_anim("knockdown_up");
	}
	situation_kind = FIGHTER_SITUATION_DOWN;
}

void Fighter::exit_status_knockdown() {
	fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = false;
}

void Fighter::status_wakeup() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_wakeup() {
	situation_kind = FIGHTER_SITUATION_GROUND;
	if (fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		switch (fighter_int[FIGHTER_INT_WAKEUP_TYPE]) {
		case WAKEUP_TYPE_FAST: {
			change_anim("wakeup_fast_down");
		} break;
		case WAKEUP_TYPE_BACK: {
			change_anim("wakeup_back_down");
		} break;
		default: {
			change_anim("wakeup_down");
		} break;
		}
	}
	else {
		switch (fighter_int[FIGHTER_INT_WAKEUP_TYPE]) {
		case WAKEUP_TYPE_FAST: {
			change_anim("wakeup_fast_up");
		} break;
		case WAKEUP_TYPE_BACK: {
			change_anim("wakeup_back_up");
		} break;
		default: {
			change_anim("wakeup_up");
		} break;
		}
	}
	fighter_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_DEFAULT;
	fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = false;
}

void Fighter::exit_status_wakeup() {

}

void Fighter::status_parry_start() {
	if (is_status_end()) {
		return;
	}
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		if (check_landing(FIGHTER_STATUS_LANDING)) {
			return;
		}
		common_air_status_general();
	}
	else {
		if (frame < 2.0 && fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_MID) {
			if (get_stick_dir() < 4) {
				change_anim_inherit_attributes("parry_start_low", false);
				fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_LOW;
			}
			else if (get_stick_dir() >= 7) {
				change_anim_inherit_attributes("parry_start_high", false);
				fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
			}
		}
	}
}

void Fighter::enter_status_parry_start() {
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		change_anim("parry_air_start");
		fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
	}
	else if (get_stick_dir() < 4) {
		change_anim("parry_start_low");
		fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_LOW;
	}
	else if (get_stick_dir() < 7) {
		change_anim("parry_start_mid");
		fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MID;
	}
	else {
		change_anim("parry_start_high");
		fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
	}
	if (fighter_flag[FIGHTER_FLAG_ATTACK_HITSTUN_PARRIED]) {
		fighter_flag[FIGHTER_FLAG_HITSTUN_COUNTER_PARRY] = true;
	}
}

void Fighter::exit_status_parry_start() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	fighter_flag[FIGHTER_FLAG_ENABLE_JUMP_COUNTERHIT] = false;
}

void Fighter::status_hitstun_parry_start() {
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] && fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX]) {
			apply_gravity(fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY], fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX]);
			add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED], 0));
		}
		else {
			common_air_status_general();
		}
		if (check_landing(FIGHTER_STATUS_LANDING)) {
			return;
		}
	}
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_hitstun_parry_start() {
	fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = 0;
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		change_anim("hitstun_parry_air_start");
	}
	else {
		change_anim("hitstun_parry_start");
	}
}

void Fighter::exit_status_hitstun_parry_start() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
}

void Fighter::status_launch_parry_start() {
	apply_gravity(fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY], fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX]);
	add_pos(glm::vec3(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED], 0));
	if (check_landing(FIGHTER_STATUS_KNOCKDOWN)) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = 0;
		fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = 0;
		return;
	}
}

void Fighter::enter_status_launch_parry_start() {
	fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	change_anim("launch_parry_start");
}

void Fighter::exit_status_launch_parry_start() {
	fighter_flag[FIGHTER_FLAG_HARD_KNOCKDOWN] = true;
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
}

void Fighter::status_parry() {
	player->controller.set_hold_buffer(true);
	if (beginning_hitlag(1) || ending_hitlag(2)) {
		frame++;
	}
	if (situation_kind == FIGHTER_SITUATION_GROUND && fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_LOW) {
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
	fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] = false;
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		change_anim("parry_air");
	}
	else {
		switch (fighter_int[FIGHTER_INT_PARRY_HEIGHT]) {
			case (PARRY_HEIGHT_LOW): {
				change_anim("parry_low");
			} break;
			case (PARRY_HEIGHT_MID):
			default: {
				change_anim("parry_mid");
			} break;
			case (PARRY_HEIGHT_HIGH): {
				change_anim("parry_high");
			} break;
		}
	}

}

void Fighter::exit_status_parry() {
	fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MAX;
	player->controller.set_hold_buffer(false);
}

void Fighter::status_landing() {
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			if (internal_facing_right != facing_right) {
				facing_right = internal_facing_right;
				if (facing_right) {
					facing_dir = 1.0;
				}
				else {
					facing_dir = -1.0;
				}
			}
			return;
		}
		else if (anim_end) {
			change_status(FIGHTER_STATUS_WAIT);
			return;
		}
	}
	else {
		if (object_manager->is_allow_realtime_process(this)) {
			fighter_int[FIGHTER_INT_LANDING_LAG]--;
		}

	}
} 

void Fighter::enter_status_landing() {
	landing_crossup();
	fighter_int[FIGHTER_INT_LANDING_LAG] = get_local_param_int("empty_landing_lag");
	change_anim("landing", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = 0.0f;
}

void Fighter::exit_status_landing() {}

void Fighter::status_landing_attack() {
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			return;
		}
		else if (change_status(FIGHTER_STATUS_WAIT)) {
			return;
		}
	}
	else {
		if (object_manager->is_allow_realtime_process(this)) {
			fighter_int[FIGHTER_INT_LANDING_LAG]--;
		}

	}
}

void Fighter::enter_status_landing_attack() {
	landing_crossup();
	fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH] = true;
	switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
		case ATTACK_KIND_LP: {
			fighter_int[FIGHTER_INT_LANDING_LAG] = get_local_param_int("lp_landing_lag");
			change_anim("landing_lp", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
		} break;
		case ATTACK_KIND_MP: {
			fighter_int[FIGHTER_INT_LANDING_LAG] = get_local_param_int("mp_landing_lag");
			change_anim("landing_mp", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
		} break;
		case ATTACK_KIND_HP: {
			fighter_int[FIGHTER_INT_LANDING_LAG] = get_local_param_int("hp_landing_lag");
			change_anim("landing_hp", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
		} break;
		case ATTACK_KIND_LK: {
			fighter_int[FIGHTER_INT_LANDING_LAG] = get_local_param_int("lk_landing_lag");
			change_anim("landing_lk", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
		} break;
		case ATTACK_KIND_MK: {
			fighter_int[FIGHTER_INT_LANDING_LAG] = get_local_param_int("mk_landing_lag");
			change_anim("landing_mk", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
		} break;
		case ATTACK_KIND_HK: {
			fighter_int[FIGHTER_INT_LANDING_LAG] = get_local_param_int("hk_landing_lag");
			change_anim("landing_hk", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
		} break;
		case ATTACK_KIND_OTHER: {
			fighter_int[FIGHTER_INT_LANDING_LAG] = get_local_param_int(get_anim() + "_empty_landing_lag");
			change_anim(get_anim() + "_landing", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
		} break;
		default: {

		} break;
	}
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = 0.0f;
}

void Fighter::exit_status_landing_attack() {}

void Fighter::status_landing_hitstun() {
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			if (internal_facing_right != facing_right) {
				facing_right = internal_facing_right;
				if (facing_right) {
					facing_dir = 1.0;
				}
				else {
					facing_dir = -1.0;
				}
			}
			return;
		}
		else if (change_status(FIGHTER_STATUS_WAIT)) {
			return;
		}
	}
	else {
		if (object_manager->is_allow_realtime_process(this)) {
			fighter_int[FIGHTER_INT_LANDING_LAG]--;
		}

	}
}


void Fighter::enter_status_landing_hitstun() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	fighter_int[FIGHTER_INT_LANDING_LAG] = fighter_int[FIGHTER_INT_HITSTUN_FRAMES];
	change_anim("landing_hitstun", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
	fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = 0.0f;
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
}

void Fighter::exit_status_taunt() {

}

void Fighter::status_ko() {

}

void Fighter::enter_status_ko() {
	if (fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("knockdown_lose_down");
	}
	else {
		change_anim("knockdown_lose_up");
	}
}

void Fighter::exit_status_ko() {

}

void Fighter::status_round_end() {

}

void Fighter::enter_status_round_end() {
	if (fighter_float[FIGHTER_FLOAT_HEALTH] == get_local_param_float("health")) {
		change_anim("round_win_perfect");
	}
	else {
		change_anim("round_win");
	}
}

void Fighter::exit_status_round_end() {

}

void Fighter::load_fighter_status_scripts() {
	status_script[FIGHTER_STATUS_NONE] = &Fighter::status_none;
	enter_status_script[FIGHTER_STATUS_NONE] = &Fighter::enter_status_none;
	exit_status_script[FIGHTER_STATUS_NONE] = &Fighter::exit_status_none;

	status_script[FIGHTER_STATUS_WAIT] = &Fighter::status_wait;
	enter_status_script[FIGHTER_STATUS_WAIT] = &Fighter::enter_status_wait;
	exit_status_script[FIGHTER_STATUS_WAIT] = &Fighter::exit_status_wait;

	status_script[FIGHTER_STATUS_WALK_F] = &Fighter::status_walk_f;
	enter_status_script[FIGHTER_STATUS_WALK_F] = &Fighter::enter_status_walk_f;
	exit_status_script[FIGHTER_STATUS_WALK_F] = &Fighter::exit_status_walk_f;

	status_script[FIGHTER_STATUS_WALK_B] = &Fighter::status_walk_b;
	enter_status_script[FIGHTER_STATUS_WALK_B] = &Fighter::enter_status_walk_b;
	exit_status_script[FIGHTER_STATUS_WALK_B] = &Fighter::exit_status_walk_b;

	status_script[FIGHTER_STATUS_DASH_F] = &Fighter::status_dash_f;
	enter_status_script[FIGHTER_STATUS_DASH_F] = &Fighter::enter_status_dash_f;
	exit_status_script[FIGHTER_STATUS_DASH_F] = &Fighter::exit_status_dash_f;

	status_script[FIGHTER_STATUS_DASH_B] = &Fighter::status_dash_b;
	enter_status_script[FIGHTER_STATUS_DASH_B] = &Fighter::enter_status_dash_b;
	exit_status_script[FIGHTER_STATUS_DASH_B] = &Fighter::exit_status_dash_b;

	status_script[FIGHTER_STATUS_DASH_AIR] = &Fighter::status_dash_air;
	enter_status_script[FIGHTER_STATUS_DASH_AIR] = &Fighter::enter_status_dash_air;
	exit_status_script[FIGHTER_STATUS_DASH_AIR] = &Fighter::exit_status_dash_air;

	status_script[FIGHTER_STATUS_CROUCH_D] = &Fighter::status_crouchd;
	enter_status_script[FIGHTER_STATUS_CROUCH_D] = &Fighter::enter_status_crouchd;
	exit_status_script[FIGHTER_STATUS_CROUCH_D] = &Fighter::exit_status_crouchd;

	status_script[FIGHTER_STATUS_CROUCH] = &Fighter::status_crouch;
	enter_status_script[FIGHTER_STATUS_CROUCH] = &Fighter::enter_status_crouch;
	exit_status_script[FIGHTER_STATUS_CROUCH] = &Fighter::exit_status_crouch;

	status_script[FIGHTER_STATUS_CROUCH_U] = &Fighter::status_crouchu;
	enter_status_script[FIGHTER_STATUS_CROUCH_U] = &Fighter::enter_status_crouchu;
	exit_status_script[FIGHTER_STATUS_CROUCH_U] = &Fighter::exit_status_crouchu;

	status_script[FIGHTER_STATUS_JUMPSQUAT] = &Fighter::status_jumpsquat;
	enter_status_script[FIGHTER_STATUS_JUMPSQUAT] = &Fighter::enter_status_jumpsquat;
	exit_status_script[FIGHTER_STATUS_JUMPSQUAT] = &Fighter::exit_status_jumpsquat;

	status_script[FIGHTER_STATUS_JUMP] = &Fighter::status_jump;
	enter_status_script[FIGHTER_STATUS_JUMP] = &Fighter::enter_status_jump;
	exit_status_script[FIGHTER_STATUS_JUMP] = &Fighter::exit_status_jump;

	status_script[FIGHTER_STATUS_FALL] = &Fighter::status_fall;
	enter_status_script[FIGHTER_STATUS_FALL] = &Fighter::enter_status_fall;
	exit_status_script[FIGHTER_STATUS_FALL] = &Fighter::exit_status_fall;

	status_script[FIGHTER_STATUS_TURN] = &Fighter::status_turn;
	enter_status_script[FIGHTER_STATUS_TURN] = &Fighter::enter_status_turn;
	exit_status_script[FIGHTER_STATUS_TURN] = &Fighter::exit_status_turn;

	status_script[FIGHTER_STATUS_ATTACK] = &Fighter::status_attack;
	enter_status_script[FIGHTER_STATUS_ATTACK] = &Fighter::enter_status_attack;
	exit_status_script[FIGHTER_STATUS_ATTACK] = &Fighter::exit_status_attack;

	status_script[FIGHTER_STATUS_ATTACK_AIR] = &Fighter::status_attack_air;
	enter_status_script[FIGHTER_STATUS_ATTACK_AIR] = &Fighter::enter_status_attack_air;
	exit_status_script[FIGHTER_STATUS_ATTACK_AIR] = &Fighter::exit_status_attack_air;

	status_script[FIGHTER_STATUS_BACKDASH_ATTACK] = &Fighter::status_backdash_attack;
	enter_status_script[FIGHTER_STATUS_BACKDASH_ATTACK] = &Fighter::enter_status_backdash_attack;
	exit_status_script[FIGHTER_STATUS_BACKDASH_ATTACK] = &Fighter::exit_status_backdash_attack;

	status_script[FIGHTER_STATUS_ADVANCE_FORWARD] = &Fighter::status_advance_forward;
	enter_status_script[FIGHTER_STATUS_ADVANCE_FORWARD] = &Fighter::enter_status_advance_forward;
	exit_status_script[FIGHTER_STATUS_ADVANCE_FORWARD] = &Fighter::exit_status_advance_forward;

	status_script[FIGHTER_STATUS_ADVANCE_BACK] = &Fighter::status_advance_back;
	enter_status_script[FIGHTER_STATUS_ADVANCE_BACK] = &Fighter::enter_status_advance_back;
	exit_status_script[FIGHTER_STATUS_ADVANCE_BACK] = &Fighter::exit_status_advance_back;

	status_script[FIGHTER_STATUS_ADVANCE] = &Fighter::status_advance;
	enter_status_script[FIGHTER_STATUS_ADVANCE] = &Fighter::enter_status_advance;
	exit_status_script[FIGHTER_STATUS_ADVANCE] = &Fighter::exit_status_advance;

	status_script[FIGHTER_STATUS_GRAB_START] = &Fighter::status_grab_start;
	enter_status_script[FIGHTER_STATUS_GRAB_START] = &Fighter::enter_status_grab_start;
	exit_status_script[FIGHTER_STATUS_GRAB_START] = &Fighter::exit_status_grab_start;

	status_script[FIGHTER_STATUS_GRAB] = &Fighter::status_grab;
	enter_status_script[FIGHTER_STATUS_GRAB] = &Fighter::enter_status_grab;
	exit_status_script[FIGHTER_STATUS_GRAB] = &Fighter::exit_status_grab;

	status_script[FIGHTER_STATUS_THROW] = &Fighter::status_throw;
	enter_status_script[FIGHTER_STATUS_THROW] = &Fighter::enter_status_throw;
	exit_status_script[FIGHTER_STATUS_THROW] = &Fighter::exit_status_throw;

	status_script[FIGHTER_STATUS_GRAB_AIR_START] = &Fighter::status_grab_air_start;
	enter_status_script[FIGHTER_STATUS_GRAB_AIR_START] = &Fighter::enter_status_grab_air_start;
	exit_status_script[FIGHTER_STATUS_GRAB_AIR_START] = &Fighter::exit_status_grab_air_start;

	status_script[FIGHTER_STATUS_GRAB_AIR] = &Fighter::status_grab_air;
	enter_status_script[FIGHTER_STATUS_GRAB_AIR] = &Fighter::enter_status_grab_air;
	exit_status_script[FIGHTER_STATUS_GRAB_AIR] = &Fighter::exit_status_grab_air;

	status_script[FIGHTER_STATUS_THROW_AIR] = &Fighter::status_throw_air;
	enter_status_script[FIGHTER_STATUS_THROW_AIR] = &Fighter::enter_status_throw_air;
	exit_status_script[FIGHTER_STATUS_THROW_AIR] = &Fighter::exit_status_throw_air;

	status_script[FIGHTER_STATUS_GRABBED] = &Fighter::status_grabbed;
	enter_status_script[FIGHTER_STATUS_GRABBED] = &Fighter::enter_status_grabbed;
	exit_status_script[FIGHTER_STATUS_GRABBED] = &Fighter::exit_status_grabbed;

	status_script[FIGHTER_STATUS_THROWN] = &Fighter::status_thrown;
	enter_status_script[FIGHTER_STATUS_THROWN] = &Fighter::enter_status_thrown;
	exit_status_script[FIGHTER_STATUS_THROWN] = &Fighter::exit_status_thrown;

	status_script[FIGHTER_STATUS_THROW_TECH] = &Fighter::status_throw_tech;
	enter_status_script[FIGHTER_STATUS_THROW_TECH] = &Fighter::enter_status_throw_tech;
	exit_status_script[FIGHTER_STATUS_THROW_TECH] = &Fighter::exit_status_throw_tech;

	status_script[FIGHTER_STATUS_BLOCKSTUN] = &Fighter::status_blockstun;
	enter_status_script[FIGHTER_STATUS_BLOCKSTUN] = &Fighter::enter_status_blockstun;
	exit_status_script[FIGHTER_STATUS_BLOCKSTUN] = &Fighter::exit_status_blockstun;

	status_script[FIGHTER_STATUS_HITSTUN] = &Fighter::status_hitstun;
	enter_status_script[FIGHTER_STATUS_HITSTUN] = &Fighter::enter_status_hitstun;
	exit_status_script[FIGHTER_STATUS_HITSTUN] = &Fighter::exit_status_hitstun;

	status_script[FIGHTER_STATUS_HITSTUN_AIR] = &Fighter::status_hitstun_air;
	enter_status_script[FIGHTER_STATUS_HITSTUN_AIR] = &Fighter::enter_status_hitstun_air;
	exit_status_script[FIGHTER_STATUS_HITSTUN_AIR] = &Fighter::exit_status_hitstun_air;

	status_script[FIGHTER_STATUS_HITSTUN_FLOAT] = &Fighter::status_hitstun_float;
	enter_status_script[FIGHTER_STATUS_HITSTUN_FLOAT] = &Fighter::enter_status_hitstun_float;
	exit_status_script[FIGHTER_STATUS_HITSTUN_FLOAT] = &Fighter::exit_status_hitstun_float;

	status_script[FIGHTER_STATUS_LAUNCH_START] = &Fighter::status_launch_start;
	enter_status_script[FIGHTER_STATUS_LAUNCH_START] = &Fighter::enter_status_launch_start;
	exit_status_script[FIGHTER_STATUS_LAUNCH_START] = &Fighter::exit_status_launch_start;

	status_script[FIGHTER_STATUS_LAUNCH] = &Fighter::status_launch;
	enter_status_script[FIGHTER_STATUS_LAUNCH] = &Fighter::enter_status_launch;
	exit_status_script[FIGHTER_STATUS_LAUNCH] = &Fighter::exit_status_launch;

	status_script[FIGHTER_STATUS_CRUMPLE] = &Fighter::status_crumple;
	enter_status_script[FIGHTER_STATUS_CRUMPLE] = &Fighter::enter_status_crumple;
	exit_status_script[FIGHTER_STATUS_CRUMPLE] = &Fighter::exit_status_crumple;

	status_script[FIGHTER_STATUS_KNOCKDOWN_START] = &Fighter::status_knockdown_start;
	enter_status_script[FIGHTER_STATUS_KNOCKDOWN_START] = &Fighter::enter_status_knockdown_start;
	exit_status_script[FIGHTER_STATUS_KNOCKDOWN_START] = &Fighter::exit_status_knockdown_start;

	status_script[FIGHTER_STATUS_KNOCKDOWN] = &Fighter::status_knockdown;
	enter_status_script[FIGHTER_STATUS_KNOCKDOWN] = &Fighter::enter_status_knockdown;
	exit_status_script[FIGHTER_STATUS_KNOCKDOWN] = &Fighter::exit_status_knockdown;

	status_script[FIGHTER_STATUS_WAKEUP] = &Fighter::status_wakeup;
	enter_status_script[FIGHTER_STATUS_WAKEUP] = &Fighter::enter_status_wakeup;
	exit_status_script[FIGHTER_STATUS_WAKEUP] = &Fighter::exit_status_wakeup;

	status_script[FIGHTER_STATUS_PARRY_START] = &Fighter::status_parry_start;
	enter_status_script[FIGHTER_STATUS_PARRY_START] = &Fighter::enter_status_parry_start;
	exit_status_script[FIGHTER_STATUS_PARRY_START] = &Fighter::exit_status_parry_start;

	status_script[FIGHTER_STATUS_HITSTUN_PARRY_START] = &Fighter::status_hitstun_parry_start;
	enter_status_script[FIGHTER_STATUS_HITSTUN_PARRY_START] = &Fighter::enter_status_hitstun_parry_start;
	exit_status_script[FIGHTER_STATUS_HITSTUN_PARRY_START] = &Fighter::exit_status_hitstun_parry_start;

	status_script[FIGHTER_STATUS_LAUNCH_PARRY_START] = &Fighter::status_launch_parry_start;
	enter_status_script[FIGHTER_STATUS_LAUNCH_PARRY_START] = &Fighter::enter_status_launch_parry_start;
	exit_status_script[FIGHTER_STATUS_LAUNCH_PARRY_START] = &Fighter::exit_status_launch_parry_start;

	status_script[FIGHTER_STATUS_PARRY] = &Fighter::status_parry;
	enter_status_script[FIGHTER_STATUS_PARRY] = &Fighter::enter_status_parry;
	exit_status_script[FIGHTER_STATUS_PARRY] = &Fighter::exit_status_parry;

	status_script[FIGHTER_STATUS_LANDING] = &Fighter::status_landing;
	enter_status_script[FIGHTER_STATUS_LANDING] = &Fighter::enter_status_landing;
	exit_status_script[FIGHTER_STATUS_LANDING] = &Fighter::exit_status_landing;

	status_script[FIGHTER_STATUS_LANDING_ATTACK] = &Fighter::status_landing_attack;
	enter_status_script[FIGHTER_STATUS_LANDING_ATTACK] = &Fighter::enter_status_landing_attack;
	exit_status_script[FIGHTER_STATUS_LANDING_ATTACK] = &Fighter::exit_status_landing_attack;

	status_script[FIGHTER_STATUS_LANDING_HITSTUN] = &Fighter::status_landing_hitstun;
	enter_status_script[FIGHTER_STATUS_LANDING_HITSTUN] = &Fighter::enter_status_landing_hitstun;
	exit_status_script[FIGHTER_STATUS_LANDING_HITSTUN] = &Fighter::exit_status_landing_hitstun;

	status_script[FIGHTER_STATUS_TAUNT] = &Fighter::status_taunt;
	enter_status_script[FIGHTER_STATUS_TAUNT] = &Fighter::enter_status_taunt;
	exit_status_script[FIGHTER_STATUS_TAUNT] = &Fighter::exit_status_taunt;

	status_script[FIGHTER_STATUS_KO] = &Fighter::status_ko;
	enter_status_script[FIGHTER_STATUS_KO] = &Fighter::enter_status_ko;
	exit_status_script[FIGHTER_STATUS_KO] = &Fighter::exit_status_ko;

	status_script[FIGHTER_STATUS_ROUND_END] = &Fighter::status_round_end;
	enter_status_script[FIGHTER_STATUS_ROUND_END] = &Fighter::enter_status_round_end;
	exit_status_script[FIGHTER_STATUS_ROUND_END] = &Fighter::exit_status_round_end;
}