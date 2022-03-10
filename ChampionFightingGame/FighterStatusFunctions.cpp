#include "Fighter.h"

bool Fighter::change_status(unsigned int new_status_kind, bool call_end_status, bool require_different_status) {
	if (new_status_kind != status_kind || !require_different_status) {
		clear_hitbox_all();
		clear_grabbox_all();
		clear_hurtbox_all();
		fighter_flag[FIGHTER_FLAG_KARA_ENABLED] = false;
		fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED_DURING_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED_DURING_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_STATIONARY_ANIMATION] = false;
		fighter_flag[FIGHTER_FLAG_THROW_TECH] = false;
		fighter_float[FIGHTER_FLOAT_DISTANCE_TO_WALL] = 0.0;
		if (call_end_status) {
			if (status_kind < FIGHTER_STATUS_MAX) {
				(this->*exit_status_script[status_kind])();
			}
			else {
				chara_exit_status();
			}
		}
		status_kind = new_status_kind;
		if (status_kind < FIGHTER_STATUS_MAX) {
			(this->*enter_status_script[status_kind])();
		}
		else {
			chara_enter_status();
		}
		return true;
	}
	else {
		return false;
	}
}

bool Fighter::change_status_after_hitlag(unsigned int new_status_kind, bool call_end_status, bool require_different_status) {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0) {
		return change_status(new_status_kind, call_end_status, require_different_status);
	}
	else {
		if (new_status_kind != status_kind || !require_different_status) {
			fighter_int[FIGHTER_INT_BUFFER_HITLAG_STATUS] = new_status_kind;
			fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_END] = call_end_status;
			fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_SEPARATE] = require_different_status;
			return true;
		}
		else {
			return false;
		}
	}
}

unsigned int Fighter::get_status_group() {
	switch (status_kind) {
		case (FIGHTER_STATUS_HITSTUN):
		case (FIGHTER_STATUS_HITSTUN_AIR):
		case (FIGHTER_STATUS_LAUNCH_START):
		case (FIGHTER_STATUS_LAUNCH):
		case (FIGHTER_STATUS_CRUMPLE):
		case (FIGHTER_STATUS_THROWN):
		{
			return STATUS_GROUP_HITSTUN;
		}
		break;
		case (FIGHTER_STATUS_CROUCHD):
		case (FIGHTER_STATUS_CROUCH):
		case (FIGHTER_STATUS_CROUCHU):
		{
			return STATUS_GROUP_CROUCH;
		}
		break;
		case (FIGHTER_STATUS_WAIT):
		case (FIGHTER_STATUS_FALL):
		case (FIGHTER_STATUS_WALKF):
		case (FIGHTER_STATUS_WALKB):
		case (FIGHTER_STATUS_JUMP):
		{
			return STATUS_GROUP_NO_RENDER_PRIORITY;
		}
		break;
		case (FIGHTER_STATUS_ATTACK):
		case (FIGHTER_STATUS_ATTACK_AIR):
		{
			return STATUS_GROUP_ATTACK;
		} break;
		default:
		{
			return STATUS_GROUP_NORMAL;
		}
		break;
	}
}

/// When it's time to transition out of the current status, this changes to the proper status. If no arg is given, this will change status to wait or
/// fall based on your situation_kind
bool Fighter::is_status_end(unsigned int post_status_kind, bool call_end_status, bool require_different_status) {
	if (is_anim_end) {
		if (situation_kind == FIGHTER_SITUATION_AIR && post_status_kind == FIGHTER_STATUS_WAIT) {
			post_status_kind = FIGHTER_STATUS_FALL;
		}
		return change_status(post_status_kind, call_end_status, require_different_status);
	}
	else if (is_actionable()) {
		if (situation_kind == FIGHTER_SITUATION_GROUND) {
			return common_ground_status_act();
		}
		else {
			return common_air_status_act();
		}
	}
	return false;
}

bool Fighter::is_status_hitstun_enable_parry() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] != 0 || fighter_accessor->fighter[!id]->fighter_int[FIGHTER_INT_DAMAGE_SCALE] == -5) {
		return false;
	}
	switch (status_kind) {
		case (FIGHTER_STATUS_HITSTUN):
		case (FIGHTER_STATUS_HITSTUN_AIR):
		case (FIGHTER_STATUS_LAUNCH):
		{
			return true;
		}
		break;
		default:
		{
			return false;
		}
		break;
	}
}