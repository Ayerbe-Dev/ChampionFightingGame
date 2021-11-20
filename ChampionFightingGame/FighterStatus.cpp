#pragma warning(disable : 4996)
#include "Fighter.h"
#include "GameCoordinate.h"
#include "Animation.h"
#include <fstream>
#include <cmath>
extern bool debug;

void Fighter::status_wait() {
	if (common_ground_status_act()) {
		return;
	}
}


void Fighter::enter_status_wait() {
	pos.y = FLOOR_GAMECOORD;
	change_anim("wait");


	situation_kind = FIGHTER_SITUATION_GROUND;
	fighter_int[FIGHTER_INT_JUGGLE_VALUE] = 0;
	common_ground_status_act();
}

void Fighter::exit_status_wait() {}

void Fighter::status_walkf() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	if (is_collide(jostle_box, fighter_accessor->fighter[!id]->jostle_box)) {
		add_pos(get_param_float("jostle_walk_f_speed") * facing_dir, 0);
	}
	else {
		add_pos(get_param_float("walk_f_speed") * facing_dir, 0);
	}
}

void Fighter::enter_status_walkf() {
	change_anim("walk_f");
}

void Fighter::exit_status_walkf() {}

void Fighter::status_walkb() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() == 5) {
		change_status(FIGHTER_STATUS_WAIT);
		return;
	}
	if (fighter_flag[FIGHTER_FLAG_PROX_GUARD]) {
		if (anim_kind->name != "stand_block") {
			change_anim("stand_block");
		}
	}
	else {
		if (anim_kind->name == "stand_block") {
			change_anim("walk_b");
		}
		add_pos(get_param_float("walk_f_speed") * facing_dir * -1, 0);
	}
}

void Fighter::enter_status_walkb() {
	change_anim("walk_b");
}

void Fighter::exit_status_walkb() {}

void Fighter::status_dash() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_param_int("dash_f_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame) {
		add_pos(get_param_float("dash_f_speed") * facing_dir, 0);
	}
	else if (frame < max_frame + 2) {
		add_pos((get_param_float("walk_f_speed") + get_param_float("dash_f_speed")) * facing_dir / 2, 0);
	}

	if (frame >= get_param_int("dash_f_cancel_frame")) {
		if (!fighter_flag[FIGHTER_FLAG_DASH_CANCEL]) {
			if (get_stick_dir() == 4) {
				if (get_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE) {
					fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = true;
				}
				change_status(FIGHTER_STATUS_DASHB, false);
				return;
			}
		}
		else if (get_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP) {
		}
	}
}

void Fighter::enter_status_dash() {
	change_anim("dash_f");
}

void Fighter::exit_status_dash() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
}

void Fighter::status_dashb() {
	if (is_status_end()) {
		return;
	}
	int min_frame = get_param_int("dash_b_accel_frame");
	int max_frame = min_frame + get_param_int("dash_b_maintain_speed_frame");

	if (frame >= min_frame && frame < max_frame) {
		add_pos(get_param_float("dash_b_speed") * facing_dir * -1, 0);
	}
	else if (frame < max_frame + 2) {
		add_pos((get_param_float("walk_b_speed") + get_param_float("dash_b_speed")) * facing_dir / -2, 0);
	}

	if (frame >= get_param_int("dash_f_cancel_frame")) {
		if (!fighter_flag[FIGHTER_FLAG_DASH_CANCEL]) {
			if (get_stick_dir() == 6) {
				if (get_param_int("dash_cancel_kind") != DASH_CANCEL_KIND_INDEFINITE) {
					fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = true;
				}
				change_status(FIGHTER_STATUS_DASH, false);
				return;
			}
		}
	}
	else if (get_param_int("dash_cancel_kind") == DASH_CANCEL_KIND_FOLLOWUP) {
	}
}

void Fighter::enter_status_dashb() {
	change_anim("dash_b");
}

void Fighter::exit_status_dashb() {
	fighter_flag[FIGHTER_FLAG_DASH_CANCEL] = false;
}

void Fighter::status_dash_air() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	if (is_status_end()) {
		return;
	}
	int min_frame = get_param_int("dash_f_accel_frame");
	int max_frame = min_frame + get_param_int("dash_f_maintain_speed_frame");

	if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
	}
	if (frame < max_frame) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0.0;
	}
	else {
		if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
			if (fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir > get_param_float("jump_x_speed")) {
				fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = clampf(get_param_float("jump_x_speed"), fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * 0.95, fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED]);
			}
		}
		else if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_B) {
			if (fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir > get_param_float("jump_x_speed") * -1.0) {
				fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = clampf(get_param_float("jump_x_speed"), fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * 0.95, fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED]);
			}
		}
	}
	if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
	}
	else {
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1.0, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
	}
}

void Fighter::enter_status_dash_air() {
	if (fighter_int[FIGHTER_INT_DASH_AIR_DIR] == 1) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float("dash_f_speed");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
		change_anim("dash_f");
	}
	if (fighter_int[FIGHTER_INT_DASH_AIR_DIR] == 2) {
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float("dash_b_speed");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
		change_anim("dash_b");
	}
}

void Fighter::exit_status_dash_air() {

}


void Fighter::status_crouchd() {
	if (common_ground_status_act()) {
		return;
	}
	if (get_stick_dir() > 3) {
		change_status(FIGHTER_STATUS_CROUCHU);
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
		change_status(FIGHTER_STATUS_CROUCHU);
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
	if (frame == 4 || is_anim_end) {
		change_status(FIGHTER_STATUS_JUMP);
		return;
	}
}

void Fighter::enter_status_jumpsquat() {
	change_anim("jump_squat");
}

void Fighter::exit_status_jumpsquat() {
	fighter_flag[FIGHTER_FLAG_SHORT_HOP] = get_stick_dir() < 7;
	fighter_flag[FIGHTER_FLAG_USED_AIRDASH] = false;
	situation_kind = FIGHTER_SITUATION_AIR;
}

void Fighter::status_jump() {
	if (is_status_end(FIGHTER_STATUS_FALL, false)) {
		return;
	}
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	common_air_status_general();
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > 0;
}

void Fighter::enter_status_jump() {
	if (get_stick_dir() == 7 || get_stick_dir() == 4 || get_stick_dir() == 1) {
		change_anim("jump_b");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
	}
	else if (get_stick_dir() == 9 || get_stick_dir() == 6 || get_stick_dir() == 3) {
		change_anim("jump_f");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
	}
	else {
		change_anim("jump");
		fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
	}
	if (fighter_flag[FIGHTER_FLAG_SHORT_HOP]) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_param_float("jump_y_init_speed_s");
	}
	else {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = get_param_float("jump_y_init_speed");
	}
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = get_param_float("jump_x_speed");
	common_air_status_act();
	fighter_flag[FIGHTER_FLAG_ENABLE_COUNTERHIT] = true;
}

void Fighter::exit_status_jump() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void Fighter::status_fall() {
	if (common_air_status_act()) {
		return;
	}
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_fall() {
	change_anim("fall");
}


void Fighter::exit_status_fall() {
	fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_N;
}

void Fighter::status_attack() {
	if (specific_status_attack()) {
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
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		if (check_button_on(BUTTON_MP) && check_button_on(BUTTON_MK)) {
			if (fighter_flag[FIGHTER_FLAG_HAS_ATTACK] && fighter_float[FIGHTER_FLOAT_SUPER_METER] >= EX_METER_SIZE / EX_METER_BARS) {
				fighter_float[FIGHTER_FLOAT_SUPER_METER] -= EX_METER_SIZE / EX_METER_BARS;
				change_status(FIGHTER_STATUS_PARRY_START);
				return;
			}
			else if (!fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS]) {
				change_status(FIGHTER_STATUS_PARRY_START);
				return;
			}
		}
	}
}

void Fighter::enter_status_attack() {
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
	stopVCAll();
}

void Fighter::status_attack_air() {
	if (specific_status_attack()) {
		return;
	}
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING_ATTACK);
		return;
	}
	if (is_status_end()) {
		return;
	}
	if (can_kara() && check_button_on(BUTTON_LP) && check_button_on(BUTTON_LK)) {
		change_status(FIGHTER_STATUS_GRAB_AIR);
	}
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_CMK) {
		if (check_button_on(BUTTON_MP) && check_button_on(BUTTON_MK)) {
			if (!fighter_flag[FIGHTER_FLAG_HAS_ATTACK] && !fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS]) {
				change_status(FIGHTER_STATUS_PARRY_START);
				return;
			}
		}
	}
	common_air_status_general();
}

void Fighter::enter_status_attack_air() {
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

void Fighter::exit_status_attack_air() {}

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
	if (!fighter_flag[FIGHTER_FLAG_THREW_OPPONENT]) {
		set_opponent_offset(GameCoordinate{ fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X], fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] });
	}
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_throw() {
	fighter_flag[FIGHTER_FLAG_THREW_OPPONENT] = false;
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
	if ((get_stick_dir() == 4 || get_stick_dir() == 1 || get_stick_dir() == 7) && get_param_bool("has_throwb")) {
		change_anim("throw_b");
	}
	else {
		change_anim("throw_f");
	}
	if (!add_pos(get_param_float(anim_kind->name + "_move_offset", param_table) * facing_dir, 0, true)) {
		if (pos.x > 0) {
			fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / 2) - pos.x;
		}
		else {
			fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / -2) + pos.x;
		}
		if (change_anim_inherit_attributes(anim_kind->name + "_stationary", false)) {
			fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = true;
		}
		else {
			fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = false;
		}
	}
}

void Fighter::exit_status_throw() {
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
	if (anim_kind->name == "throw_b") {
		facing_right = !facing_right;
		facing_dir *= -1;
	}
}

void Fighter::status_grab_air() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING);
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
	if (!fighter_flag[FIGHTER_FLAG_THREW_OPPONENT]) {
		set_opponent_offset(GameCoordinate{ fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X], fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] });
	}
	if (pos.y <= FLOOR_GAMECOORD) {
		set_pos(pos.x, FLOOR_GAMECOORD);
		situation_kind = FIGHTER_SITUATION_GROUND;
	}
	if (is_status_end()) {
		return;
	}
	common_air_status_general();
}

void Fighter::enter_status_throw_air() {
	fighter_flag[FIGHTER_FLAG_THREW_OPPONENT] = false;
	if ((get_stick_dir() == 4 || get_stick_dir() == 1 || get_stick_dir() == 7) && get_param_bool("has_throwb")) {
		change_anim("throw_b_air");
	}
	else {
		change_anim("throw_f_air");
	}
	if (!add_pos(get_param_float(anim_kind->name + "_move_offset", param_table) * facing_dir, 0, true)) {
		if (pos.x > 0) {
			fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / 2) - pos.x;
		}
		else {
			fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = (WINDOW_WIDTH / -2) + pos.x;
		}
		if (change_anim_inherit_attributes(anim_kind->name + "_stationary", false)) {
			fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = true;
		}
		else {
			fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = false;
		}
	}
}

void Fighter::exit_status_throw_air() {
	if (anim_kind->name == "throw_b_air") {
		facing_dir *= -1;
		facing_right = !facing_right;
		if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_F) {
			fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_B;
		}
		else if (fighter_int[FIGHTER_INT_JUMP_KIND] == CHARA_JUMP_KIND_B) {
			fighter_int[FIGHTER_INT_JUMP_KIND] = CHARA_JUMP_KIND_F;
		}
	}
}

void Fighter::status_grabbed() {
	if (fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] != 0 && fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] != 0) {
		if (fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES] != 0) {
			add_pos(fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_X] * facing_dir * -1, fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_Y] * -1);
			fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES]--;
		}
		else {
			set_pos(fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X], fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y]);
		}
	}
}

void Fighter::enter_status_grabbed() {
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
}

void Fighter::exit_status_grabbed() {
	angle = 0;
	fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
	fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] = 0;
	fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
}

void Fighter::status_thrown() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_KNOCKDOWN);
		return;
	}
	if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
	}
	add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED], fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
}

void Fighter::enter_status_thrown() {
	situation_kind = FIGHTER_SITUATION_AIR;
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = get_param_float("gravity");
	}
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = get_param_float("max_fall_speed");
	}
	change_anim("knockdown_start", -1, get_launch_ticks());
}

void Fighter::exit_status_thrown() {

}

void Fighter::status_hitstun() {
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

void Fighter::enter_status_hitstun() {
	if (get_stick_dir() < 4) {
		if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
			change_anim("crouch_hitstun_l", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
			change_anim("crouch_hitstun_m", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else {
			change_anim("crouch_hitstun_h", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
	}
	else {
		if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_LIGHT) {
			change_anim("stand_hitstun_l", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else if (fighter_int[FIGHTER_INT_HITSTUN_LEVEL] == ATTACK_LEVEL_MEDIUM) {
			change_anim("stand_hitstun_m", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else {
			change_anim("stand_hitstun_h", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
	}
}

void Fighter::exit_status_hitstun() {}

void Fighter::status_hitstun_air() {
	if (pos.y < FLOOR_GAMECOORD) {
		change_status(FIGHTER_STATUS_LANDING_HITSTUN);
		return;
	}
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0) {
		change_status(FIGHTER_STATUS_FALL, false);
		return;
	}
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
			fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
		}
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
	}
}

void Fighter::enter_status_hitstun_air() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X];
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED];
	change_anim("jump_hitstun", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
}


void Fighter::exit_status_hitstun_air() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] /= 2;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] /= 2;
}

void Fighter::status_blockstun() {
	if (situation_kind == FIGHTER_SITUATION_GROUND) {
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
	else {
		if (pos.y < 50.0) {
			change_status(FIGHTER_STATUS_LANDING_HITSTUN);
			return;
		}
		if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
			if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > get_param_float("max_fall_speed") * -1.0) {
				fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= get_param_float("gravity");
			}
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
			change_anim("crouch_block", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else if (fighter_int[FIGHTER_INT_BLOCKSTUN_HEIGHT] == ATTACK_HEIGHT_HIGH) {
			change_anim("high_block", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
		else {
			change_anim("stand_block", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		}
	}
	else {
		change_anim("jump_block", -1, fighter_int[FIGHTER_INT_HITSTUN_FRAMES]);
		fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
		fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	}
}

void Fighter::exit_status_blockstun() {}

void Fighter::status_parry_start() {
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		if (pos.y < FLOOR_GAMECOORD) {
			change_status(FIGHTER_STATUS_LANDING);
			return;
		}
		fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_ALL;
		common_air_status_general();
	}
	else {
		if (get_stick_dir() < 4) {
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_LOW;
		}
		else if (get_stick_dir() < 7) {
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MID;
		}
		else {
			fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_HIGH;
		}
	}
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_parry_start() {
	if (situation_kind == FIGHTER_SITUATION_AIR) {
		change_anim("parry_start_air");
	}
	else {
		change_anim("parry_start");
	}
}

void Fighter::exit_status_parry_start() {}

void Fighter::status_parry() {
	if (beginning_hitlag(1) || ending_hitlag(2)) {
		forceStepThroughHitlag();
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
		else if (fighter_int[FIGHTER_INT_PARRY_HEIGHT] == PARRY_HEIGHT_MID) {
			change_anim("parry_mid");
		}
		else {
			change_anim("parry_high");
		}
	}
}

void Fighter::exit_status_parry() {
	fighter_int[FIGHTER_INT_PARRY_HEIGHT] = PARRY_HEIGHT_MAX;
}

void Fighter::status_crumple() {
	if (is_anim_end) {
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
	change_anim("launch_start");
}

void Fighter::exit_status_launch_start() {
	situation_kind = FIGHTER_SITUATION_AIR;
}

void Fighter::status_launch() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 1) {
		max_ticks = ceil((float)get_launch_ticks() / (float)(anim_kind->length));
	}
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		if (fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] > fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] * -1.0) {
			fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] -= fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY];
		}
		add_pos(fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] * facing_dir * -1, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
		if (pos.y < FLOOR_GAMECOORD) {
			change_status(FIGHTER_STATUS_KNOCKDOWN);
			return;
		}
	}
}

void Fighter::enter_status_launch() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = fighter_float[FIGHTER_FLOAT_LAUNCH_SPEED_X];
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = fighter_float[FIGHTER_FLOAT_INIT_LAUNCH_SPEED];
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_GRAVITY] = get_param_float("gravity");
	}
	if (fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] == 0) {
		fighter_float[FIGHTER_FLOAT_LAUNCH_FALL_SPEED_MAX] = get_param_float("max_fall_speed");
	}
	change_anim("launch", -1, get_launch_ticks());
}

void Fighter::exit_status_launch() {
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = false;
}

void Fighter::status_clank() {

}

void Fighter::enter_status_clank() {

}

void Fighter::exit_status_clank() {}

void Fighter::status_throw_tech() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 15 || fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 16) {
		forceStepThroughHitlag();
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
		else if (change_status(FIGHTER_STATUS_WAIT)) {
			return;
		}
	}
	else {
		fighter_int[FIGHTER_INT_LANDING_LAG]--;
	}
}

void Fighter::enter_status_landing() {
	Fighter* that = fighter_accessor->fighter[!id];
	if (that->pos.x - that->pos.x_spr_offset / 2 == WINDOW_WIDTH / -2 && pos.x - pos.x_spr_offset / 2 == WINDOW_WIDTH / -2) {
		that->pos.x += 20;
	}
	if (that->pos.x + that->pos.x_spr_offset / 2 == WINDOW_WIDTH / 2 && pos.x + pos.x_spr_offset / 2 == WINDOW_WIDTH / 2) {
		that->pos.x -= 20;
	}
	fighter_int[FIGHTER_INT_LANDING_LAG] = 2;
	change_anim("landing", -1, fighter_int[FIGHTER_INT_LANDING_LAG]);
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
		fighter_int[FIGHTER_INT_LANDING_LAG]--;
	}
}


void Fighter::enter_status_landing_attack() {
	if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("lp_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MP) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("mp_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HP) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("hp_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("lk_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_MK) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("mk_landing_lag");
	}
	else if (fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_HK) {
		fighter_int[FIGHTER_INT_LANDING_LAG] = get_param_int("hk_landing_lag");
	}
	change_anim("landing", -1, fighter_int[FIGHTER_INT_LANDING_LAG]);
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	situation_kind = FIGHTER_SITUATION_GROUND;
	pos.y = FLOOR_GAMECOORD;
}

void Fighter::exit_status_landing_attack() {}

void Fighter::status_landing_hitstun() {
	if (fighter_int[FIGHTER_INT_LANDING_LAG] == 0) {
		if (common_ground_status_act()) {
			return;
		}
		else if (change_status(FIGHTER_STATUS_WAIT)) {
			return;
		}
	}
	else {
		fighter_int[FIGHTER_INT_LANDING_LAG]--;
	}
}


void Fighter::enter_status_landing_hitstun() {
	fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = 0;
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = 0;
	fighter_int[FIGHTER_INT_LANDING_LAG] = 4;
	change_anim("landing_hitstun", -1, fighter_int[FIGHTER_INT_LANDING_LAG]);
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
	if (is_anim_end) {
		if (fighter_int[FIGHTER_INT_WAKEUP_SPEED] == WAKEUP_SPEED_FAST) {
			//Hacky impl for 3 different wakeup speeds, in the future we can just make a different animation for each speed
			change_status(FIGHTER_STATUS_WAKEUP);
		}
		else {
			fighter_int[FIGHTER_INT_WAKEUP_SPEED] --;
		}
	}
}

void Fighter::enter_status_knockdown() {
	if (fighter_flag[FIGHTER_FLAG_CAN_TECH]) {
		if (fighter_int[FIGHTER_INT_KNOCKDOWN_TECH_WINDOW] == 0) {
			fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
		}
		if (fighter_int[FIGHTER_INT_WAKEUP_SPEED] == WAKEUP_SPEED_SLOW) {
			change_anim("knockdown_wait");
		}
		if (fighter_int[FIGHTER_INT_WAKEUP_SPEED] == WAKEUP_SPEED_DEFAULT) {
			change_anim("knockdown_wait");
		}
		if (fighter_int[FIGHTER_INT_WAKEUP_SPEED] == WAKEUP_SPEED_FAST) {
			change_anim("knockdown_wait");
		}
	}
	else {
		change_anim("knockdown_wait");
	}
	situation_kind = FIGHTER_SITUATION_DOWN;
}

void Fighter::exit_status_knockdown() {
	fighter_int[FIGHTER_INT_WAKEUP_SPEED] = WAKEUP_SPEED_DEFAULT;
	situation_kind = FIGHTER_SITUATION_GROUND;
	fighter_flag[FIGHTER_FLAG_CAN_TECH] = true;
}

void Fighter::status_wakeup() {
	if (is_status_end()) {
		return;
	}
}

void Fighter::enter_status_wakeup() {
	change_anim("wakeup");
}

void Fighter::exit_status_wakeup() {

}