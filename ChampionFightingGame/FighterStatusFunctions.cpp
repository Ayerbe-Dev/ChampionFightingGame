#include "Fighter.h"

bool Fighter::change_status(unsigned int new_status_kind, bool call_end_status, bool require_different_status) {
	if (new_status_kind != status_kind || !require_different_status) {
		clear_hitbox_all();
		clear_hurtbox_all();
		clear_grabbox_all();
		clear_pushbox_all();
		fighter_flag[FIGHTER_FLAG_KARA_ENABLED] = false;
		fighter_flag[FIGHTER_FLAG_ATTACK_CONNECTED] = false;
		fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED] = false;
		fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX_IN_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_IN_ENDLAG] = false;
		fighter_flag[FIGHTER_FLAG_THROW_TECH] = false;
		fighter_flag[FIGHTER_FLAG_ALLOW_CANCEL_RECOVERY] = false;
		fighter_flag[FIGHTER_FLAG_ALLOW_VERTICAL_PUSHBACK] = false;
		fighter_int[FIGHTER_INT_PRE_ENABLE_CANCEL_STATUS] = FIGHTER_STATUS_MAX;
		disable_all_cancels();
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
		fighter_int[FIGHTER_INT_BUFFER_HITLAG_STATUS] = new_status_kind;
		fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_END] = call_end_status;
		fighter_flag[FIGHTER_FLAG_BUFFER_HITLAG_STATUS_SEPARATE] = require_different_status;
		return true;
	}
}

bool Fighter::buffer_status_pre_enable_cancel(unsigned int new_status_kind, unsigned int cancel_kind, bool call_end_status, bool require_different_status) {
	if (is_enable_cancel(cancel_kind)) {
		return change_status_after_hitlag(new_status_kind, call_end_status, require_different_status);
	}
	else if (fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		fighter_int[FIGHTER_INT_PRE_ENABLE_CANCEL_STATUS] = new_status_kind;
		fighter_int[FIGHTER_INT_PRE_ENABLE_CANCEL_KIND] = cancel_kind;
		fighter_int[FIGHTER_INT_PRE_ENABLE_CANCEL_TIMER] = get_param_int(PARAM_FIGHTER, "motion_special_timer");
		fighter_flag[FIGHTER_FLAG_PRE_ENABLE_CANCEL_STATUS_END] = call_end_status;
		fighter_flag[FIGHTER_FLAG_PRE_ENABLE_CANCEL_STATUS_SEPARATE] = require_different_status;
		return true;
	}
	return false;
}

unsigned int Fighter::get_status_group() {
	switch (status_kind) {
		case (FIGHTER_STATUS_HITSTUN):
		case (FIGHTER_STATUS_HITSTUN_AIR):
		case (FIGHTER_STATUS_HITSTUN_FLOAT):
		case (FIGHTER_STATUS_KNOCKDOWN_START):
		case (FIGHTER_STATUS_KNOCKDOWN):
		case (FIGHTER_STATUS_LAUNCH_START):
		case (FIGHTER_STATUS_LAUNCH):
		case (FIGHTER_STATUS_CRUMPLE):
		case (FIGHTER_STATUS_THROWN):
		case (FIGHTER_STATUS_HITSTUN_PARRY_START):
		case (FIGHTER_STATUS_LAUNCH_PARRY_START): {
			return STATUS_GROUP_HITSTUN;
		}
		break;
		case (FIGHTER_STATUS_CROUCH_D):
		case (FIGHTER_STATUS_CROUCH):
		case (FIGHTER_STATUS_CROUCH_U):
		{
			return STATUS_GROUP_CROUCH;
		}
		break;
		case (FIGHTER_STATUS_ATTACK):
		case (FIGHTER_STATUS_ATTACK_AIR):
		{
			return STATUS_GROUP_ATTACK;
		} break;
		case (FIGHTER_STATUS_LANDING):
		case (FIGHTER_STATUS_LANDING_ATTACK):
		case (FIGHTER_STATUS_LANDING_HITSTUN): {
			return STATUS_GROUP_LANDING;
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
	bool ret = false;
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && pos.y <= 0.0f && fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] < 0.0) {
		ret = change_status(post_status_kind, call_end_status, require_different_status);
		if (ret) { //The frame we land counts as a frame of landing lag
			(this->*status_script[status_kind])();
		}
	}
	return ret;
}

bool Fighter::check_hitstun_parry() {
	if (fighter_int[FIGHTER_INT_DAMAGE_SCALE] >= 0 //Damage scale has to be at least 0, so you can't
		//hitstun parry the first hit after a counterhit.
		&& fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 //Can't start the hitstun parry during hitlag
		&& !fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] //Can't hitstun parry if you already
		//tried it during this combo
		&& !fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY_HITBOX]) {
		unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)) {
			fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY] = true;
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