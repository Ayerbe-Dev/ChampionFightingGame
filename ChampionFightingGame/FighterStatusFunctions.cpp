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
		fighter_flag[FIGHTER_FLAG_THROW_TECH] = false;
		if (call_end_status) {
			(this->*exit_status_script[status_kind])();
		}
		status_kind = new_status_kind;
		(this->*enter_status_script[status_kind])();
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

bool Fighter::check_landing(unsigned int post_status_kind, bool call_end_status, bool require_different_status) {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && pos.y <= FLOOR_GAMECOORD && fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] < 0.0) {
		return change_status(post_status_kind, call_end_status, require_different_status);
	}
	return false;
}

bool Fighter::check_hitstun_parry() {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && !fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY]) {
		unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)) {
			fighter_int[FIGHTER_INT_DAMAGE_SCALE] = -5;
			fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = true;
			fighter_flag[FIGHTER_FLAG_USED_HITSTUN_PARRY] = true;
			player->controller.reset_buffer();
			return true;
		}
	}
	return false;
}

bool Fighter::is_status_delay() {
	switch (status_kind) {
		case (FIGHTER_STATUS_GRABBED): {
			return true;
		}
		break;
		default: {
			return false;
		}
		break;
	}
}