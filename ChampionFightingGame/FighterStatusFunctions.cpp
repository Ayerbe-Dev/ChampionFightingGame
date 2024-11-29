#include "Fighter.h"
#include "GameManager.h"

void Fighter::change_status(unsigned int new_status_kind, bool call_end_status) {
	if (new_status_kind == status_kind
		&& (object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_NON_ACTION)) {
		return;	
	}
	clear_hitbox_all();
	clear_hurtbox_all();
	clear_grabbox_all();
	clear_pushbox_all();
	object_flag[BATTLE_OBJECT_FLAG_ACTIVE_EX_SUPER] = false;
	object_flag[BATTLE_OBJECT_FLAG_ACTIVE_CHAMPION_SUPER] = false;
	object_flag[FIGHTER_FLAG_ATTACK_HIT] = false;
	object_flag[FIGHTER_FLAG_ATTACK_BLOCKED] = false;
	object_flag[FIGHTER_FLAG_PROJECTILE_HIT_DURING_STATUS] = false;
	object_flag[FIGHTER_FLAG_PROJECTILE_BLOCKED_DURING_STATUS] = false;
	object_flag[FIGHTER_FLAG_ACTIVE_HITBOX_IN_STATUS] = false;
	object_flag[FIGHTER_FLAG_THROW_TECH] = false;
	object_flag[FIGHTER_FLAG_ALLOW_FREE_CANCEL] = false;
	object_flag[FIGHTER_FLAG_HITSTUN_COUNTER_PARRY] = false;
	object_flag[FIGHTER_FLAG_ALLOW_CORNER_CROSSUP] = false;
	object_flag[FIGHTER_FLAG_CHANGE_INTO_SAME_STATUS] = status_kind == new_status_kind;
	object_int[FIGHTER_INT_ARMOR_HITS] = 0;
	object_int[FIGHTER_INT_SUCCESS_COUNTERHIT_VAL] = COUNTERHIT_VAL_NONE;
	object_int[FIGHTER_INT_STATUS_GROUP] = STATUS_GROUP_NONE;
	disable_all_cancels();
	if (call_end_status) {
		(this->*exit_status_script[status_kind])();
	}
	if (object_flag[FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END]) {
		facing_right = !facing_right;
		facing_dir *= -1;
	}
	object_flag[FIGHTER_FLAG_FLIP_FACING_ON_STATUS_END] = false;
	status_kind = new_status_kind;
	(this->*enter_status_script[status_kind])();
	object_flag[FIGHTER_FLAG_ATTACK_HITSTUN_PARRIED] = false;
	object_flag[FIGHTER_FLAG_DISABLE_THROW_TECH] = false;
	object_flag[FIGHTER_FLAG_STATUS_CHANGED] = true;
}

void Fighter::change_context(unsigned int fighter_context) {
	if (this->fighter_context == fighter_context) return;
	object_int[FIGHTER_INT_HITSTUN_HEIGHT] = HITSTUN_HEIGHT_STAND;
	move_list[fighter_context].disable_all_cancels();
	this->fighter_context = fighter_context;
	check_movelist_inputs();
}

/// When it's time to transition out of the current status, this changes to the proper status. If no arg is given, this will change status to wait or
/// fall based on your fighter_context
bool Fighter::is_status_end(unsigned int post_status_kind, bool call_end_status, bool require_different_status) {
	if (anim_end) {
		if (post_status_kind == BATTLE_OBJECT_STATUS_NONE) {
			switch (fighter_context) {
				case (FIGHTER_CONTEXT_GROUND): {
					if (move_list[FIGHTER_CONTEXT_GROUND].is_curr_move_recover_crouching(this)) {
						post_status_kind = FIGHTER_STATUS_CROUCH;
					}
					else {
						post_status_kind = FIGHTER_STATUS_WAIT;
					}
				} break;
				case (FIGHTER_CONTEXT_AIR): {
					post_status_kind = FIGHTER_STATUS_FALL;
				} break;
				default: {

				} break;
			}
		}
		change_status(post_status_kind, call_end_status);
		return true;
	}
	else if (is_actionable() && fighter_context == FIGHTER_CONTEXT_GROUND) {
		return common_ground_status_act();
	}
	return false;
}

bool Fighter::check_landing(unsigned int post_status_kind, bool call_end_status) {
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0 && get_pos().y <= 0.0f && object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] < 0.0) {
		change_status(post_status_kind, call_end_status);
		(this->*status_script[status_kind])();
		return true;
	}
	return false;
}

bool Fighter::check_hitstun_parry() {
	if (object_float[FIGHTER_FLOAT_DAMAGE_SCALE] < 0.8f //Damage scale has to be lower than 80%, so you can't
		//hitstun parry the first 3 hits of a combo (4 on counterhit/punish, 5 on crit)
		&& object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0 //Can't start the hitstun parry during hitlag
		&& !object_flag[FIGHTER_FLAG_DISABLE_HITSTUN_PARRY]) {
		unsigned int parry_buttons[2] = { BUTTON_MP, BUTTON_MK };
		if (check_button_input(parry_buttons, 2)) {
			player->controller.reset_buffer();
			return true;
		}
	}
	return false;
}

bool Fighter::is_ko() {
	return object_float[FIGHTER_FLOAT_HEALTH] == 0.0f && GameManager::get_instance()->get_scene()->game_context != SCENE_CONTEXT_TRAINING;
}