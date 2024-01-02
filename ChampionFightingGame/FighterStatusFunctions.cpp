#include "Fighter.h"

bool Fighter::change_status(unsigned int new_status_kind, bool call_end_status, bool require_different_status) {
	if (new_status_kind != status_kind || !require_different_status) {
		clear_hitbox_all();
		clear_hurtbox_all();
		clear_grabbox_all();
		clear_pushbox_all();
		fighter_flag[FIGHTER_FLAG_ATTACK_HIT] = false;
		fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED] = false;
		fighter_flag[FIGHTER_FLAG_PROJECTILE_HIT_DURING_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_PROJECTILE_BLOCKED_DURING_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX_IN_STATUS] = false;
		fighter_flag[FIGHTER_FLAG_ENABLE_PUNISH] = false;
		fighter_flag[FIGHTER_FLAG_THROW_TECH] = false;
		fighter_flag[FIGHTER_FLAG_ALLOW_FREE_CANCEL] = false;
		fighter_flag[FIGHTER_FLAG_ALLOW_VERTICAL_PUSHBACK] = false;
		fighter_flag[FIGHTER_FLAG_HITSTUN_COUNTER_PARRY] = false;
		fighter_flag[FIGHTER_FLAG_CHANGE_INTO_SAME_STATUS] = status_kind == new_status_kind;
		fighter_int[FIGHTER_INT_ARMOR_HITS] = 0;
		fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL] = 0;
		disable_all_cancels();
		if (call_end_status) {
			(this->*exit_status_script[status_kind])();
		}
		if (fighter_flag[FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END]) {
			facing_right = !facing_right;
			facing_dir *= -1;
		}
		fighter_flag[FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END] = false;
		status_kind = new_status_kind;
		(this->*enter_status_script[status_kind])();
		fighter_flag[FIGHTER_FLAG_ATTACK_HITSTUN_PARRIED] = false;
		fighter_flag[FIGHTER_FLAG_DISABLE_THROW_TECH] = false;
		return true;
	}
	else {
		return false;
	}
}

void Fighter::change_situation(unsigned int new_situation_kind) {
	if (situation_kind != new_situation_kind) {
		move_list[situation_kind].disable_all_cancels();
		situation_kind = new_situation_kind;
		check_movelist_inputs();
	}
}

unsigned int Fighter::get_status_group() {
	switch (status_kind) {
		case (FIGHTER_STATUS_HITSTUN):
		case (FIGHTER_STATUS_HITSTUN_AIR):
		case (FIGHTER_STATUS_HITSTUN_FLOAT):
		case (FIGHTER_STATUS_LANDING_HITSTUN):
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
		case (FIGHTER_STATUS_LANDING_ATTACK): {
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
	if (anim_end) {
		if (post_status_kind == FIGHTER_STATUS_NONE) {
			switch (situation_kind) {
			case (FIGHTER_SITUATION_GROUND): {
				if (move_list[FIGHTER_SITUATION_GROUND].is_curr_move_recover_crouching(this)) {
					post_status_kind = FIGHTER_STATUS_CROUCH;
				}
				else {
					post_status_kind = FIGHTER_STATUS_WAIT;
				}
			} break;
			case (FIGHTER_SITUATION_AIR): {
				post_status_kind = FIGHTER_STATUS_FALL;
			} break;
			default: {

			} break;
			}
		}
		return change_status(post_status_kind, call_end_status, require_different_status);
	}
	else if (is_actionable() && situation_kind == FIGHTER_SITUATION_GROUND) {
		return common_ground_status_act();
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
	if (fighter_int[FIGHTER_INT_DAMAGE_SCALE] >= 3 //Damage scale has to be at least 3, so you can't
		//hitstun parry the first 3 hits of a combo (4 on counterhit, 5 on special counterhits)
		&& fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 //Can't start the hitstun parry during hitlag
		&& !fighter_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY]) {
		unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)) {
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

bool Fighter::is_ko() {
	return fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0f && GameManager::get_instance()->get_game_state()->game_context != GAME_CONTEXT_TRAINING;
}