#include "Fighter.h"

void Fighter::change_status(unsigned int new_status_kind, bool call_end_status) {
	if (new_status_kind == status_kind
		&& (fighter_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_NON_ACTION)) {
		return;	
	}
	clear_hitbox_all();
	clear_hurtbox_all();
	clear_grabbox_all();
	clear_pushbox_all();
	fighter_flag[FIGHTER_FLAG_ATTACK_HIT] = false;
	fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED] = false;
	fighter_flag[FIGHTER_FLAG_PROJECTILE_HIT_DURING_STATUS] = false;
	fighter_flag[FIGHTER_FLAG_PROJECTILE_BLOCKED_DURING_STATUS] = false;
	fighter_flag[FIGHTER_FLAG_ACTIVE_HITBOX_IN_STATUS] = false;
	fighter_flag[FIGHTER_FLAG_THROW_TECH] = false;
	fighter_flag[FIGHTER_FLAG_ALLOW_FREE_CANCEL] = false;
	fighter_flag[FIGHTER_FLAG_HITSTUN_COUNTER_PARRY] = false;
	fighter_flag[FIGHTER_FLAG_CHANGE_INTO_SAME_STATUS] = status_kind == new_status_kind;
	fighter_int[FIGHTER_INT_ARMOR_HITS] = 0;
	fighter_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL] = COUNTERHIT_VAL_NONE;
	fighter_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NONE;
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
	fighter_flag[FIGHTER_FLAG_STATUS_CHANGED] = true;
}

void Fighter::change_situation(unsigned int new_situation_kind) {
	if (situation_kind != new_situation_kind) {
		fighter_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
		move_list[situation_kind].disable_all_cancels();
		situation_kind = new_situation_kind;
		check_movelist_inputs();
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
		change_status(post_status_kind, call_end_status);
		return true;
	}
	else if (is_actionable() && situation_kind == FIGHTER_SITUATION_GROUND) {
		return common_ground_status_act();
	}
	return false;
}

bool Fighter::check_landing(unsigned int post_status_kind, bool call_end_status) {
	if (fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && pos.y <= 0.0f && fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] < 0.0) {
		change_status(post_status_kind, call_end_status);
		(this->*status_script[status_kind])();
		return true;
	}
	return false;
}

bool Fighter::check_hitstun_parry() {
	if (fighter_float[FIGHTER_FLOAT_DAMAGE_SCALE] <= 0.7f //Damage scale has to be 70% or lower, so you can't
		//hitstun parry the first 3 hits of a combo (4 on counterhit/punish, 5 on crit)
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

bool Fighter::is_ko() {
	return fighter_float[FIGHTER_FLOAT_HEALTH] == 0.0f && GameManager::get_instance()->get_game_state()->game_context != GAME_CONTEXT_TRAINING;
}