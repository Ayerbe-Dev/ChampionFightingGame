#pragma warning(disable : 4996)
#include "Fighter.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include "ParamAccessor.h"
#include <fstream>
#include <cmath>
#include <glm/gtx/quaternion.hpp>
#include "GLM Helpers.h"
#include "utils.h"

bool Fighter::common_ground_status_act(bool crouch) {
	if (specific_ground_status_act()) {
		return true;
	}
	if (is_actionable()) {
		if (get_stick_dir() < 4) {
			fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_CROUCH;
		}
		else {
			fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
		}

		if (check_button_input(BUTTON_LP) || check_button_input(BUTTON_MP) || check_button_input(BUTTON_HP) || check_button_input(BUTTON_LK) || check_button_input(BUTTON_MK) || check_button_input(BUTTON_HK)) {
			unsigned int grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
			if (check_button_input(grab_buttons, 2)) {
				return (change_status(FIGHTER_STATUS_GRAB));
			}
			if (check_button_input(BUTTON_LP)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CLP;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LP;
				}
			}
			if (check_button_input(BUTTON_LK)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CLK;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LK;
				}
			}
			unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
			if (check_button_input(parry_buttons, 2)) {
				return (change_status(FIGHTER_STATUS_PARRY_START));
			}
			if (check_button_input(BUTTON_MP)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CMP;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MP;
				}
			}
			if (check_button_input(BUTTON_MK)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CMK;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MK;
				}
			}
			unsigned int advance_buttons[2] = { BUTTON_HP, BUTTON_HK };
			if (check_button_input(advance_buttons, 2) && player->control_type == CONTROL_TYPE_ADVANCE) {
				if (get_stick_dir() == 6) {
					if (has_meter(1)) {
						return (change_status(FIGHTER_STATUS_ADVANCE_FORWARD, true, false));
					}
				}
				else if (get_stick_dir() == 4) {
					if (has_meter(1)) {
						return (change_status(FIGHTER_STATUS_ADVANCE_BACK, true, false));
					}
				}
				else {
					if (has_meter(1)) {
						return (change_status(FIGHTER_STATUS_ADVANCE, true, false));
					}
				}
			}
			if (check_button_input(BUTTON_HP)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CHP;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HP;
				}
			}
			if (check_button_input(BUTTON_HK)) {
				if (get_stick_dir() < 4) {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_CHK;
				}
				else {
					fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HK;
				}
			}
			return change_status(FIGHTER_STATUS_ATTACK, true, false);
		}
		if (get_stick_dir() == 6) {
			if (fighter_int[FIGHTER_INT_DASH_F_BUFFER] != 0) {
				return change_status(FIGHTER_STATUS_DASH_F);
			}
			else {
				return change_status(FIGHTER_STATUS_WALK_F);
			}
		}
		if (get_stick_dir() == 4) {
			if (fighter_int[FIGHTER_INT_DASH_B_BUFFER] != 0) {
				return change_status(FIGHTER_STATUS_DASH_B);
			}
			else {
				return change_status(FIGHTER_STATUS_WALK_B);
			}
		}
		if (get_stick_dir() > 6) {
			return change_status(FIGHTER_STATUS_JUMPSQUAT);
		}
		if (crouch && get_stick_dir() < 4 && status_kind != FIGHTER_STATUS_CROUCH) {
			if (status_kind == FIGHTER_STATUS_ATTACK && fighter_int[FIGHTER_INT_ATTACK_KIND] >= ATTACK_KIND_HK) {
				return change_status(FIGHTER_STATUS_CROUCH);
			}
			else {
				return change_status(FIGHTER_STATUS_CROUCH_D);
			}
		}
	}
	return false;
}

bool Fighter::common_air_status_act() {
	if (specific_air_status_act()) {
		return true;
	}
	if (is_actionable()) {
		if (check_button_input(BUTTON_LP) || check_button_input(BUTTON_MP) || check_button_input(BUTTON_HP) || check_button_input(BUTTON_LK) || check_button_input(BUTTON_MK) || check_button_input(BUTTON_HK)) {
			unsigned int grab_buttons[2] = { BUTTON_LP, BUTTON_LK };
			if (check_button_input(grab_buttons, 2)) {
				return (change_status(FIGHTER_STATUS_GRAB_AIR, false, false));
			}
			if (check_button_input(BUTTON_LP)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LP;
			}
			if (check_button_input(BUTTON_LK)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_LK;
			}
			unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
			if (check_button_input(parry_buttons, 2)) {
				return (change_status(FIGHTER_STATUS_PARRY_START, false, false));
			}
			if (check_button_input(BUTTON_MP)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MP;
			}
			if (check_button_input(BUTTON_MK)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_MK;
			}
			if (check_button_input(BUTTON_HP)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HP;
			}
			if (check_button_input(BUTTON_HK)) {
				fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_HK;
			}
			return change_status(FIGHTER_STATUS_ATTACK_AIR, false, false);
		}
		if (fighter_int[FIGHTER_INT_DASH_F_BUFFER] != 0) {
			fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 1;
			fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = true;
			return change_status(FIGHTER_STATUS_DASH_AIR);
		}
		if (fighter_int[FIGHTER_INT_DASH_B_BUFFER] != 0) {
			fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 2;
			fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = true;
			return change_status(FIGHTER_STATUS_DASH_AIR);
		}
	}
	return false;
}

bool Fighter::common_air_status_general() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		apply_gravity(get_local_param_float("gravity"), get_local_param_float("max_fall_speed"));
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
	}
	if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] < 0) {
		fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	}
	return false;
}

void Fighter::status_wait() {
	if (common_ground_status_act()) {
		return;
	}
}


void Fighter::enter_status_wait() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_NONE;
	pos.y = FLOOR_GAMECOORD;
	change_anim("wait");

	situation_kind = FIGHTER_SITUATION_GROUND;
	fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	common_ground_status_act();
}

void Fighter::exit_status_wait() {}

void Fighter::status_walk_f() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	if (is_collide(jostle_box, battle_object_manager->fighter[!id]->jostle_box)) {
		add_pos(get_local_param_float("jostle_walk_f_speed") * facing_dir, 0);
	}
	else {
		add_pos(get_local_param_float("walk_f_speed") * facing_dir, 0);
	}
}

void Fighter::enter_status_walk_f() {
	change_anim("walk_f");
}

void Fighter::exit_status_walk_f() {}

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
				if (is_anim_end) {
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
		add_pos(get_local_param_float("walk_f_speed") * facing_dir * -1, 0);
	}
}

void Fighter::enter_status_walk_b() {
	change_anim("walk_b");
}

void Fighter::exit_status_walk_b() {}

void Fighter::status_dash_f() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_local_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_local_param_int("dash_f_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame) {
		add_pos(get_local_param_float("dash_f_speed") * facing_dir, 0);
	}
	else if (frame < max_frame + 2) {
		add_pos((get_local_param_float("walk_f_speed") + get_local_param_float("dash_f_speed")) * facing_dir / 2, 0);
	}

	if (frame >= get_local_param_int("dash_f_cancel_frame")) {
		if (!fighter_flag[FIGHTER_FLAG_DASH_CANCEL]) {
			if (get_stick_dir(false) == 4) {
				if (get_local_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE) {
					fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = true;
				}
				change_status(FIGHTER_STATUS_DASH_B, false);
				return;
			}
		}
		else if (get_local_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP) {
			//TODO: Decide what the input for dash followups will be, make the status for it, and
			//run the input check for it here
		}
	}
}

void Fighter::enter_status_dash_f() {
	change_anim("dash_f");
}

void Fighter::exit_status_dash_f() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
}

void Fighter::status_dash_b() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_local_param_int("dash_b_accel_frame");
	int max_frame = min_frame + get_local_param_int("dash_b_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame) {
		add_pos(get_local_param_float("dash_b_speed") * facing_dir * -1, 0);
	}
	else if (frame < max_frame + 2) {
		add_pos((get_local_param_float("walk_b_speed") + get_local_param_float("dash_b_speed")) * facing_dir / -2, 0);
	}

	if (frame >= get_local_param_int("dash_b_cancel_frame")) {
		if (!fighter_flag[FIGHTER_FLAG_DASH_CANCEL]) {
			if (get_stick_dir(false) == 6) {
				if (get_local_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE) {
					fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = true;
				}
				change_status(FIGHTER_STATUS_DASH_F, false);
				return;
			}
		}
	}
	else if (get_local_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP) {
		//See note during dash_f status but for backdash followups
	}
}

void Fighter::enter_status_dash_b() {
	change_anim("dash_b");
}

void Fighter::exit_status_dash_b() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
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
	if (is_anim_end) {
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
}

void Fighter::exit_status_crouch() {}

void Fighter::status_crouchu() {
	if (common_ground_status_act()) {
		return;
	}
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_WAIT);
	}
}

void Fighter::enter_status_crouchu() {
	change_anim("crouch_u");
}

void Fighter::exit_status_crouchu() {}

void Fighter::status_jumpsquat() {
	if (get_stick_dir() == 7 || get_stick_dir() == 4 || get_stick_dir() == 1) {
		fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_B;
	}
	else if (get_stick_dir() == 9 || get_stick_dir() == 6 || get_stick_dir() == 3) {
		fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_F;
	}
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_JUMP);
		return;
	}
}

void Fighter::enter_status_jumpsquat() {
	if (get_stick_dir() == 7 || get_stick_dir() == 4 || get_stick_dir() == 1) {
		fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_B;
	}
	else if (get_stick_dir() == 9 || get_stick_dir() == 6 || get_stick_dir() == 3) {
		fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_F;
	}
	else {
		fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_N;
	}
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
	if (is_status_end(FIGHTER_STATUS_FALL, false)) {
		return;
	}
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > 0;
}

void Fighter::enter_status_jump() {
	if (fighter_int[FIGHTER_INT_JUMP_KIND] == JUMP_KIND_B) {
		change_anim("jump_b", 1.0, 0.0);
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("jump_x_speed") * -facing_dir;
	}
	else if (fighter_int[FIGHTER_INT_JUMP_KIND] == JUMP_KIND_F) {
		change_anim("jump_f", 1.0, 0.0);
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_local_param_float("jump_x_speed") * facing_dir;
	}
	else {
		change_anim("jump", 1.0, 0.0);
	}
	if (fighter_flag[FIGHTER_FLAG_SHORT_HOP]) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_local_param_float("jump_y_init_speed_s");
	}
	else {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_local_param_float("jump_y_init_speed");
	}
	if (common_air_status_act()) {
		return;
	}
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
}

void Fighter::exit_status_jump() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_N;
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
		if (is_anim_end) {
			change_status(FIGHTER_STATUS_WAIT);
			return;
		}
		if (get_anim() == "turn_crouch") {
			change_anim_inherit_attributes("turn_stand", false);
		}
	}
	else {
		if (is_anim_end) {
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
	facing_right = internal_facing_right;
	if (facing_right) {
		facing_dir = 1.0;
	}
	else {
		facing_dir = -1.0;
	}
}

void Fighter::status_attack() {
	if (fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED] && fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) {
		unsigned int advance_buttons[2] = { BUTTON_HP, BUTTON_HK };
		if (check_button_input(advance_buttons, 2) && player->control_type == CONTROL_TYPE_ADVANCE) {
			if (has_meter(3)) {
				if (get_stick_dir() == 6) {
					change_status(FIGHTER_STATUS_ADVANCE_FORWARD);
					return;
				}
				else {
					battle_object_manager->set_world_rate(id, fighter_int[FIGHTER_INT_HITLAG_FRAMES] / 20.0);
					change_status(FIGHTER_STATUS_ADVANCE);
					return;
				}
			}
		}
	}
	if (specific_status_attack()) {
		return;
	}
	for (int i = 0; i < 12; i++) {
		if (attack_cancel(i)) {
			return;
		}
	}
	if (is_enable_cancel(CANCEL_KIND_DASH_F) && fighter_int[FIGHTER_INT_DASH_F_BUFFER] > fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		change_status_after_hitlag(FIGHTER_STATUS_DASH_F);
		return;
	}
	if (is_enable_cancel(CANCEL_KIND_DASH_B) && fighter_int[FIGHTER_INT_DASH_B_BUFFER] > fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		change_status_after_hitlag(FIGHTER_STATUS_DASH_B);
		return;
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] > ATTACK_KIND_HK) { //If we're in a crouching attack, change to the crouch animation on animation end. 
		if (is_status_end(FIGHTER_STATUS_CROUCH)) {
			return;
		}
	}
	else { //Otherwise, just transition to wait
		if (is_status_end()) {
			return;
		}
	}
	if (can_kara() && check_button_on(BUTTON_LP) && check_button_on(BUTTON_LK)) {
		change_status(FIGHTER_STATUS_GRAB);
		return;
	}
	switch (fighter_int[FIGHTER_INT_ATTACK_KIND]) {
		case ATTACK_KIND_MP:
		case ATTACK_KIND_MK:
		case ATTACK_KIND_CMP:
		case ATTACK_KIND_CMK: {
			if (check_button_on(BUTTON_MP) && check_button_on(BUTTON_MK)) {
				if (!fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX_IN_STATUS]) {
					change_status(FIGHTER_STATUS_PARRY_START);
					return;
				}
				else if (fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX] && has_meter(1)) {
					change_status(FIGHTER_STATUS_PARRY_START);
					return;
				}
			}
		} break;
		case ATTACK_KIND_HP:
		case ATTACK_KIND_HK:
		case ATTACK_KIND_CHP:
		case ATTACK_KIND_CHK: {
			if (check_button_on(BUTTON_HP) && check_button_on(BUTTON_HK) && player->control_type == CONTROL_TYPE_ADVANCE) {
				if (!fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX_IN_STATUS]) {
					if (get_stick_dir() == 6) {
						if (has_meter(1)) {
							change_status(FIGHTER_STATUS_ADVANCE_FORWARD, true, false);
							return;
						}
					}
					else if (get_stick_dir() == 4) {
						if (has_meter(1)) {
							change_status(FIGHTER_STATUS_ADVANCE_BACK, true, false);
							return;
						}
					}
					else {
						if (has_meter(1)) {
							change_status(FIGHTER_STATUS_ADVANCE, true, false);
							return;
						}
					}
				}
			}
		} break;
		default: {

		} break;
	}
}

void Fighter::enter_status_attack() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP) {
		change_anim("stand_lp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP) {
		change_anim("stand_mp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HP) {
		change_anim("stand_hp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) {
		change_anim("stand_lk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK) {
		change_anim("stand_mk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HK) {
		change_anim("stand_hk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CLP) {
		change_anim("crouch_lp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMP) {
		change_anim("crouch_mp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CHP) {
		change_anim("crouch_hp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CLK) {
		change_anim("crouch_lk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		change_anim("crouch_mk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CHK) {
		change_anim("crouch_hk");
	}
}

void Fighter::exit_status_attack() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	stop_vc_all();
	clear_effect_all();
}

void Fighter::status_attack_air() {
	if (check_landing(FIGHTER_STATUS_LANDING_ATTACK)) {
		return;
	}
	if (specific_status_attack()) {
		return;
	}
	for (int i = 0; i < 6; i++) {
		if (attack_cancel(i)) {
			return;
		}
	}
	if (is_enable_cancel(CANCEL_KIND_DASH_F) && fighter_int[FIGHTER_INT_DASH_F_BUFFER] > fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 1;
		fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = true;
		change_status_after_hitlag(FIGHTER_STATUS_DASH_AIR);
		return;
	}
	if (is_enable_cancel(CANCEL_KIND_DASH_B) && fighter_int[FIGHTER_INT_DASH_B_BUFFER] > fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		fighter_int[FIGHTER_INT_DASH_AIR_DIR] = 2;
		fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = true;
		change_status_after_hitlag(FIGHTER_STATUS_DASH_AIR);
		return;
	}
	if (is_status_end()) {
		return;
	}
	if (can_kara() && check_button_on(BUTTON_LP) && check_button_on(BUTTON_LK)) {
		change_status(FIGHTER_STATUS_GRAB_AIR);
		return;
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		if (check_button_on(BUTTON_MP) && check_button_on(BUTTON_MK)) {
			if (!fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX] && !fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX_IN_STATUS]) {
				change_status(FIGHTER_STATUS_PARRY_START);
				return;
			}
		}
	}
	common_air_status_general();
}

void Fighter::enter_status_attack_air() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP) {
		change_anim("jump_lp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP) {
		change_anim("jump_mp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HP) {
		change_anim("jump_hp");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) {
		change_anim("jump_lk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK) {
		change_anim("jump_mk");
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HK) {
		change_anim("jump_hk");
	}
}

void Fighter::exit_status_attack_air() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
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
	change_anim("advance_high");
}

void Fighter::exit_status_advance() {

}

void Fighter::status_advance_forward() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_advance_forward() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_OTHER;
	change_anim("advance_forward");
}

void Fighter::exit_status_advance_forward() {

}

void Fighter::status_advance_back() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_advance_back() {
	fighter_int[FIGHTER_INT_ATTACK_KIND] = ATTACK_KIND_OTHER;
	change_anim("advance_back");
}

void Fighter::exit_status_advance_back() {

}

void Fighter::status_grab() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_grab() {
	fighter_flag[FIGHTER_FLAG_THROW_TECH] = true;
	change_anim("grab");
}

void Fighter::exit_status_grab() {}

void Fighter::status_throw() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_throw() {
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
	if ((get_stick_dir() == 4 || get_stick_dir() == 1 || get_stick_dir() == 7) && get_local_param_bool("has_throwb")) {
		change_anim("throw_b");
	}
	else {
		change_anim("throw_f");
	}
}

void Fighter::exit_status_throw() {
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
	if (get_anim() == "throw_b") {
		facing_right = !facing_right;
		facing_dir *= -1;
	}
}

void Fighter::status_grab_air() {
	if (check_landing()) {
		return;
	}
	if (is_status_end()) {
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_grab_air() {
	fighter_flag[FIGHTER_FLAG_THROW_TECH] = true;
	change_anim("grab_air");
}

void Fighter::exit_status_grab_air() {}

void Fighter::status_throw_air() {
	if (pos.y <= FLOOR_GAMECOORD) {
		set_pos(pos.x, FLOOR_GAMECOORD);
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
	if ((get_stick_dir() == 4 || get_stick_dir() == 1 || get_stick_dir() == 7) && get_local_param_bool("has_throwb")) {
		change_anim("throw_b_air");
	}
	else {
		change_anim("throw_f_air");
	}
}

void Fighter::exit_status_throw_air() {
	fighter_flag[FIGHTER_FLAG_DISABLE_AIR_GENERAL] = false;
	if (get_anim() == "throw_b_air") {
		facing_right = !facing_right;
		facing_dir *= -1;
		if (fighter_int[FIGHTER_INT_JUMP_KIND] == JUMP_KIND_F) {
			fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_B;
		}
		else if (fighter_int[FIGHTER_INT_JUMP_KIND] == JUMP_KIND_B) {
			fighter_int[FIGHTER_INT_JUMP_KIND] = JUMP_KIND_F;
		}
	}
}

void Fighter::status_grabbed() {
	if (fighter_flag[FIGHTER_FLAG_GRABBED]) {
		Fighter* that = battle_object_manager->fighter[!id];
		glm::vec3 offset = glm::vec3(
			fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_X], 
			fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_Y], 
			0.0
		);
		glm::vec3 offset_bone_pos = get_relative_bone_position(fighter_int[FIGHTER_INT_GRABBED_BONE_ID]);
		glm::vec3 target_pos = that->get_bone_position(fighter_int[FIGHTER_INT_GRAB_BONE_ID], offset);
		if (pos == target_pos) {
			fighter_int[FIGHTER_INT_GRAB_POS_CHANGE_FRAMES] = 0;
		}
		if (fighter_int[FIGHTER_INT_GRAB_POS_CHANGE_FRAMES] > 0) {
			if (battle_object_manager->allow_dec_var(id)) {
				fighter_int[FIGHTER_INT_GRAB_POS_CHANGE_FRAMES]--;
			}
			glm::vec3 distance = target_pos - (pos + offset_bone_pos);
			distance /= fighter_int[FIGHTER_INT_GRAB_INIT_POS_CHANGE_FRAMES];
			add_pos(distance);
		}
		else {
			set_pos(target_pos - offset_bone_pos);
		}
	}
}

void Fighter::enter_status_grabbed() {
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
	fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION] = true;
}

void Fighter::exit_status_grabbed() {
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
	fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION] = false;
	pos.z = 0.0;
}

void Fighter::status_thrown() {
	if (check_landing(FIGHTER_STATUS_KNOCKDOWN)) {
		return;
	}
	apply_gravity(fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY], fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX]);
	add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
}

void Fighter::enter_status_thrown() {
	situation_kind = FIGHTER_SITUATION_AIR;
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = get_local_param_float("gravity");
	}
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = get_local_param_float("max_fall_speed");
	}
	fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y] = fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED];
	change_anim("knockdown_start", calc_launch_frames(), -1.0);
}

void Fighter::exit_status_thrown() {

}

void Fighter::status_hitstun() {
	if (!fighter_flag[FIGHTER_FLAG_USED_HITSTUN_PARRY]) {
		if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
			fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = false;
			if (fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] == HITSTUN_HEIGHT_CROUCH) {
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
		else if (check_hitstun_parry()) {
			change_anim("hitstun_parry_start");
		}
	}
}

void Fighter::enter_status_hitstun() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	if (fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] == HITSTUN_HEIGHT_STAND) {
		if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
			change_anim("stand_hitstun_l", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		}
		else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
			change_anim("stand_hitstun_m", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		}
		else {
			change_anim("stand_hitstun_h", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		}
	}
	else if (fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] == HITSTUN_HEIGHT_CROUCH) {
		if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
			change_anim("crouch_hitstun_l", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		}
		else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
			change_anim("crouch_hitstun_m", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		}
		else {
			change_anim("crouch_hitstun_h", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		}
	}
	else {
		if (get_stick_dir() < 4) {
			if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
				change_anim("crouch_hitstun_l", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
			else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
				change_anim("crouch_hitstun_m", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
			else {
				change_anim("crouch_hitstun_h", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
		}
		else {
			if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
				change_anim("stand_hitstun_l", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
			else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
				change_anim("stand_hitstun_m", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
			else {
				change_anim("stand_hitstun_h", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
			}
		}
	}
}

void Fighter::exit_status_hitstun() {
	fighter_flag[FIGHTER_FLAG_USED_HITSTUN_PARRY] = false;
}

void Fighter::status_hitstun_air() {
	Fighter* that = battle_object_manager->fighter[!id];
	if (check_landing(FIGHTER_STATUS_LANDING_HITSTUN)) {
		return;
	}
	if (!fighter_flag[FIGHTER_FLAG_USED_HITSTUN_PARRY]) {
		if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
			fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = false;
			change_status(FIGHTER_STATUS_FALL, false);
			return;
		}
		else if (check_hitstun_parry()) {
			change_anim("hitstun_parry_air_start");
		}
	}
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		apply_gravity(get_local_param_float("gravity"), get_local_param_float("max_fall_speed"));
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
	}
}

void Fighter::enter_status_hitstun_air() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	std::cout << fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X] << ", " << fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y] << "\n";
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X];
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y];
	change_anim("jump_hitstun", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
}


void Fighter::exit_status_hitstun_air() {
	fighter_flag[FIGHTER_FLAG_USED_HITSTUN_PARRY] = false;
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0.0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0.0;
}

void Fighter::status_blockstun() {
	if (situation_kind == FIGHTER_SITUATION_GROUND) {
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0) {
			unsigned int advance_buttons[2] = { BUTTON_HP, BUTTON_HK };
			if (check_button_input(advance_buttons, 2) && player->control_type == CONTROL_TYPE_ADVANCE) {
				if ((get_stick_dir() == 4 || get_stick_dir() == 1) && has_meter(3)) {
					fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 0;
					fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES] = 0;
					fighter_int[FIGHTER_INT_HITSTUN_FRAMES] = 0;
					change_status(FIGHTER_STATUS_ADVANCE_BACK);
					return;
				}
			}
		}
		else {
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
			else {
				if (get_stick_dir() < 4) {
					if (get_anim() == "stand_blockstun") {
						change_anim_inherit_attributes("crouch_blockstun", false);
					}
				}
				else {
					if (get_anim() == "crouch_blockstun") {
						change_anim_inherit_attributes("stand_blockstun", false);
					}
				}
			}
		}

	}
	else {
		if (check_landing(FIGHTER_STATUS_LANDING_HITSTUN)) {
			return;
		}
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			apply_gravity(get_local_param_float("gravity"), get_local_param_float("max_fall_speed"));
			add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1.5, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
		}
		if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
			change_status(FIGHTER_STATUS_FALL);

		}
	}
}


void Fighter::enter_status_blockstun() {
	if (situation_kind == FIGHTER_SITUATION_GROUND) {
		if (get_stick_dir() < 4) {
			change_anim("crouch_blockstun", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		}
		else if (fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] == ATTACK_HEIGHT_HIGH) {
			change_anim("high_blockstun", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		}
		else {
			change_anim("stand_blockstun", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		}
	}
	else {
		change_anim("jump_blockstun", fighter_int[FIGHTER_INT_HITSTUN_FRAMES], -1.0);
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	}
}

void Fighter::exit_status_blockstun() {}

void Fighter::status_parry_start() {
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		if (check_landing(FIGHTER_STATUS_LANDING)) {
			return;
		}
		fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
		common_air_status_general();
	}
	else {
		if (get_stick_dir() < 4) {
			if (fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] && (get_anim() != "parry_start_low")) {
				change_anim_inherit_attributes("parry_start_low", false);
			}
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_LOW;
		}
		else {
			if (fighter_flag[FIGHTER_FLAG_PARRY_ACTIVE] && (get_anim() != "parry_start")) {
				change_anim_inherit_attributes("parry_start", false);
			}
			if (get_stick_dir() < 7) {
				fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MID;
			}
			else {
				fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
			}
		}
	}
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_parry_start() {
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		change_anim("parry_air_start");
	}
	else if (get_stick_dir() < 4) {
		change_anim("parry_start_low");
	}
	else {
		change_anim("parry_start");
	}
}

void Fighter::exit_status_parry_start() {}

void Fighter::status_parry() {
	if (beginning_hitlag(1) || ending_hitlag(2)) {
		frame += battle_object_manager->world_rate;
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
		if (fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_LOW) {
			change_anim("parry_low");
		}
		else if (fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_HIGH) {
			change_anim("parry_high");
		}
		else {
			change_anim("parry_mid");
		}
	}

}

void Fighter::exit_status_parry() {
	fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MAX;
}

void Fighter::status_crumple() {
	if (is_anim_end) {
		fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = true;
		change_status(FIGHTER_STATUS_KNOCKDOWN);
		return;
	}
}

void Fighter::enter_status_crumple() {
	change_anim("crumple");
}

void Fighter::exit_status_crumple() {
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = false;
}

void Fighter::status_launch_start() {
	if (is_anim_end) {
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
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
		if (check_landing(FIGHTER_STATUS_KNOCKDOWN)) {
			fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = false;
			return;
		}
	}
	if (!fighter_flag[FIGHTER_FLAG_USED_HITSTUN_PARRY]) {
		if (check_hitstun_parry()) {
			change_anim("hitstun_parry_air_start");
		}
	}
}

void Fighter::enter_status_launch() {
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = false;
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X];
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_Y];
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = get_local_param_float("gravity");
	}
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = get_local_param_float("max_fall_speed");
	}
	change_anim("launch", calc_launch_frames(), -1.0);
}

void Fighter::exit_status_launch() {
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = false;
	fighter_flag[FIGHTER_FLAG_USED_HITSTUN_PARRY] = false;
}

void Fighter::status_blowback() {

}

void Fighter::enter_status_blowback() {

}

void Fighter::exit_status_blowback() {

}

void Fighter::status_clank() {

}

void Fighter::enter_status_clank() {

}

void Fighter::exit_status_clank() {

}

void Fighter::status_throw_tech() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 15 || fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 16) {
		frame++;
	}
	else if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] < 15) {
		add_pos(-4 * facing_dir, 0);
	}
	else {
		add_pos(-1 * facing_dir, 0);
	}
	if (is_anim_end) {
		change_status(situation_kind == FIGHTER_SITUATION_GROUND ? FIGHTER_STATUS_WAIT : FIGHTER_STATUS_FALL); //woah i can do this
	}
}

void Fighter::enter_status_throw_tech() {
	change_anim("throw_tech");
	fighter_int[FIGHTER_INT_HITLAG_FRAMES] = 20;
}

void Fighter::exit_status_throw_tech() {}

void Fighter::status_landing() {
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			return;
		}
		else if (is_anim_end) {
			change_status(FIGHTER_STATUS_WAIT);
			return;
		}
	}
	else {
		if (battle_object_manager->allow_dec_var(id)) {
			fighter_int[FIGHTER_INT_LANDING_LAG]--;
		}

	}
} 

void Fighter::enter_status_landing() {
	landing_crossup();
	fighter_int[FIGHTER_INT_LANDING_LAG] = 2;
	change_anim("landing", fighter_int[FIGHTER_INT_LANDING_LAG], -1.0);
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = FLOOR_GAMECOORD;
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
		if (battle_object_manager->allow_dec_var(id)) {
			fighter_int[FIGHTER_INT_LANDING_LAG]--;
		}

	}
}


void Fighter::enter_status_landing_attack() {
	landing_crossup();
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
	}
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = FLOOR_GAMECOORD;
}

void Fighter::exit_status_landing_attack() {}

void Fighter::status_landing_hitstun() {
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
		fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = false;
		if (common_ground_status_act()) {
			return;
		}
		else if (change_status(FIGHTER_STATUS_WAIT)) {
			return;
		}
	}
	else {
		if (battle_object_manager->allow_dec_var(id)) {
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
	pos.y = FLOOR_GAMECOORD;
}

void Fighter::exit_status_landing_hitstun() {

}

void Fighter::status_knockdown_start() {
	if (is_anim_end) {
		change_status(FIGHTER_STATUS_KNOCKDOWN);
	}
}

void Fighter::enter_status_knockdown_start() {
	change_anim("knockdown_start");
}

void Fighter::exit_status_knockdown_start() {

}

void Fighter::status_knockdown() {
	if (is_status_end(FIGHTER_STATUS_WAKEUP)) {
		return;
	}
}

void Fighter::enter_status_knockdown() {
	if (fighter_flag[FIGHTER_FLAG_CAN_TECH] && fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] != 0 && fighter_int[FIGHTER_INT_WAKEUP_TYPE] != WAKEUP_TYPE_DEFAULT) {
		change_status(FIGHTER_STATUS_WAKEUP);
		return;
	}
	if (fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
		change_anim("knockdown_down");
	}
	else {
		change_anim("knockdown_up");
	}
	situation_kind = FIGHTER_SITUATION_DOWN;
}

void Fighter::exit_status_knockdown() {
	situation_kind = FIGHTER_SITUATION_GROUND;
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = true;
}

void Fighter::status_wakeup() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_wakeup() {
	switch (fighter_int[FIGHTER_INT_WAKEUP_TYPE]) {
		case WAKEUP_TYPE_FAST: {
			change_anim("wakeup_fast");
		} break;
		case WAKEUP_TYPE_BACK: {
			change_anim("wakeup_back");
		} break;
		default: {
			if (fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN]) {
				change_anim("wakeup_down");
			}
			else {
				change_anim("wakeup_up");
			}
		} break;
	}
	fighter_int[FIGHTER_INT_WAKEUP_TYPE] = WAKEUP_TYPE_DEFAULT;
	fighter_flag[FIGHTER_FLAG_DOWN_FACE_DOWN] = false;
}

void Fighter::exit_status_wakeup() {

}

void Fighter::load_status_scripts() {
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

	status_script[FIGHTER_STATUS_ADVANCE_FORWARD] = &Fighter::status_advance_forward;
	enter_status_script[FIGHTER_STATUS_ADVANCE_FORWARD] = &Fighter::enter_status_advance_forward;
	exit_status_script[FIGHTER_STATUS_ADVANCE_FORWARD] = &Fighter::exit_status_advance_forward;

	status_script[FIGHTER_STATUS_ADVANCE_BACK] = &Fighter::status_advance_back;
	enter_status_script[FIGHTER_STATUS_ADVANCE_BACK] = &Fighter::enter_status_advance_back;
	exit_status_script[FIGHTER_STATUS_ADVANCE_BACK] = &Fighter::exit_status_advance_back;

	status_script[FIGHTER_STATUS_ADVANCE] = &Fighter::status_advance;
	enter_status_script[FIGHTER_STATUS_ADVANCE] = &Fighter::enter_status_advance;
	exit_status_script[FIGHTER_STATUS_ADVANCE] = &Fighter::exit_status_advance;

	status_script[FIGHTER_STATUS_GRAB] = &Fighter::status_grab;
	enter_status_script[FIGHTER_STATUS_GRAB] = &Fighter::enter_status_grab;
	exit_status_script[FIGHTER_STATUS_GRAB] = &Fighter::exit_status_grab;

	status_script[FIGHTER_STATUS_THROW] = &Fighter::status_throw;
	enter_status_script[FIGHTER_STATUS_THROW] = &Fighter::enter_status_throw;
	exit_status_script[FIGHTER_STATUS_THROW] = &Fighter::exit_status_throw;

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

	status_script[FIGHTER_STATUS_HITSTUN] = &Fighter::status_hitstun;
	enter_status_script[FIGHTER_STATUS_HITSTUN] = &Fighter::enter_status_hitstun;
	exit_status_script[FIGHTER_STATUS_HITSTUN] = &Fighter::exit_status_hitstun;

	status_script[FIGHTER_STATUS_HITSTUN_AIR] = &Fighter::status_hitstun_air;
	enter_status_script[FIGHTER_STATUS_HITSTUN_AIR] = &Fighter::enter_status_hitstun_air;
	exit_status_script[FIGHTER_STATUS_HITSTUN_AIR] = &Fighter::exit_status_hitstun_air;

	status_script[FIGHTER_STATUS_BLOCKSTUN] = &Fighter::status_blockstun;
	enter_status_script[FIGHTER_STATUS_BLOCKSTUN] = &Fighter::enter_status_blockstun;
	exit_status_script[FIGHTER_STATUS_BLOCKSTUN] = &Fighter::exit_status_blockstun;

	status_script[FIGHTER_STATUS_PARRY_START] = &Fighter::status_parry_start;
	enter_status_script[FIGHTER_STATUS_PARRY_START] = &Fighter::enter_status_parry_start;
	exit_status_script[FIGHTER_STATUS_PARRY_START] = &Fighter::exit_status_parry_start;

	status_script[FIGHTER_STATUS_PARRY] = &Fighter::status_parry;
	enter_status_script[FIGHTER_STATUS_PARRY] = &Fighter::enter_status_parry;
	exit_status_script[FIGHTER_STATUS_PARRY] = &Fighter::exit_status_parry;

	status_script[FIGHTER_STATUS_CRUMPLE] = &Fighter::status_crumple;
	enter_status_script[FIGHTER_STATUS_CRUMPLE] = &Fighter::enter_status_crumple;
	exit_status_script[FIGHTER_STATUS_CRUMPLE] = &Fighter::exit_status_crumple;

	status_script[FIGHTER_STATUS_LAUNCH_START] = &Fighter::status_launch_start;
	enter_status_script[FIGHTER_STATUS_LAUNCH_START] = &Fighter::enter_status_launch_start;
	exit_status_script[FIGHTER_STATUS_LAUNCH_START] = &Fighter::exit_status_launch_start;

	status_script[FIGHTER_STATUS_LAUNCH] = &Fighter::status_launch;
	enter_status_script[FIGHTER_STATUS_LAUNCH] = &Fighter::enter_status_launch;
	exit_status_script[FIGHTER_STATUS_LAUNCH] = &Fighter::exit_status_launch;

	status_script[FIGHTER_STATUS_BLOWBACK] = &Fighter::status_blowback;
	enter_status_script[FIGHTER_STATUS_BLOWBACK] = &Fighter::enter_status_blowback;
	exit_status_script[FIGHTER_STATUS_BLOWBACK] = &Fighter::exit_status_blowback;

	status_script[FIGHTER_STATUS_CLANK] = &Fighter::status_clank;
	enter_status_script[FIGHTER_STATUS_CLANK] = &Fighter::enter_status_clank;
	exit_status_script[FIGHTER_STATUS_CLANK] = &Fighter::exit_status_clank;

	status_script[FIGHTER_STATUS_THROW_TECH] = &Fighter::status_throw_tech;
	enter_status_script[FIGHTER_STATUS_THROW_TECH] = &Fighter::enter_status_throw_tech;
	exit_status_script[FIGHTER_STATUS_THROW_TECH] = &Fighter::exit_status_throw_tech;

	status_script[FIGHTER_STATUS_LANDING] = &Fighter::status_landing;
	enter_status_script[FIGHTER_STATUS_LANDING] = &Fighter::enter_status_landing;
	exit_status_script[FIGHTER_STATUS_LANDING] = &Fighter::exit_status_landing;

	status_script[FIGHTER_STATUS_LANDING_ATTACK] = &Fighter::status_landing_attack;
	enter_status_script[FIGHTER_STATUS_LANDING_ATTACK] = &Fighter::enter_status_landing_attack;
	exit_status_script[FIGHTER_STATUS_LANDING_ATTACK] = &Fighter::exit_status_landing_attack;

	status_script[FIGHTER_STATUS_LANDING_HITSTUN] = &Fighter::status_landing_hitstun;
	enter_status_script[FIGHTER_STATUS_LANDING_HITSTUN] = &Fighter::enter_status_landing_hitstun;
	exit_status_script[FIGHTER_STATUS_LANDING_HITSTUN] = &Fighter::exit_status_landing_hitstun;

	status_script[FIGHTER_STATUS_KNOCKDOWN_START] = &Fighter::status_knockdown_start;
	enter_status_script[FIGHTER_STATUS_KNOCKDOWN_START] = &Fighter::enter_status_knockdown_start;
	exit_status_script[FIGHTER_STATUS_KNOCKDOWN_START] = &Fighter::exit_status_knockdown_start;

	status_script[FIGHTER_STATUS_KNOCKDOWN] = &Fighter::status_knockdown;
	enter_status_script[FIGHTER_STATUS_KNOCKDOWN] = &Fighter::enter_status_knockdown;
	exit_status_script[FIGHTER_STATUS_KNOCKDOWN] = &Fighter::exit_status_knockdown;

	status_script[FIGHTER_STATUS_WAKEUP] = &Fighter::status_wakeup;
	enter_status_script[FIGHTER_STATUS_WAKEUP] = &Fighter::enter_status_wakeup;
	exit_status_script[FIGHTER_STATUS_WAKEUP] = &Fighter::exit_status_wakeup;
}