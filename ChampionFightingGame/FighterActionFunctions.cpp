#include "Fighter.h"
#include "ParamAccessor.h"

bool Fighter::is_actionable() {
	if (object_manager->world_rate == 0.0f && object_manager->real_time_id != id) {
		return false; //Disable all actions in super freeze no matter what
	}
	if (anim_kind == nullptr) {
		return true;
	}
	if (!fighter_flag[FIGHTER_FLAG_IN_THROW_TECH_WINDOW]
		&& status_kind != FIGHTER_STATUS_THROWN
		&& (get_status_group() != STATUS_GROUP_HITSTUN || ((status_kind == FIGHTER_STATUS_HITSTUN 
		|| status_kind == FIGHTER_STATUS_HITSTUN_AIR) && !fighter_int[FIGHTER_INT_HITSTUN_FRAMES]))
		&& !fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES]
		&& status_kind != FIGHTER_STATUS_ROUND_END) {
		if (anim_kind->faf == -1) {
			return std::ceil(frame) >= anim_kind->length;
		}
		else {
			return std::ceil(frame) >= anim_kind->faf;
		}
	}
	else {
		return false;
	}
}

bool Fighter::is_enable_free_cancel() {
	return fighter_flag[FIGHTER_FLAG_ALLOW_FREE_CANCEL] || is_actionable();
}

void Fighter::enable_free_cancel() {
	fighter_flag[FIGHTER_FLAG_ALLOW_FREE_CANCEL] = true;
}

void Fighter::enable_cancel(std::string move_kind, CancelKind cancel_kind) {
	move_list[situation_kind].enable_cancel(move_kind, cancel_kind);
}

void Fighter::disable_all_cancels() {
	for (int i = 0; i < FIGHTER_SITUATION_MAX; i++) {
		move_list[i].disable_all_cancels();
	}
}

void Fighter::disable_cancel(std::string move_kind, CancelKind cancel_kind) {
	move_list[situation_kind].disable_cancel(move_kind, cancel_kind);
}

bool Fighter::is_enable_cancel(std::string move_kind) {
	return move_list[situation_kind].is_enable_cancel(this, move_kind);
}

FighterMoveListEntry Fighter::get_curr_move() {
	return move_list[situation_kind].get_move(fighter_string[FIGHTER_STRING_MOVE_KIND]);
}

void Fighter::check_movelist_inputs() {
	if (!fighter_flag[FIGHTER_FLAG_ROUND_START]) {
		move_list[situation_kind].check_inputs(this);
	}
	if (!fighter_int[FIGHTER_INT_HITLAG_FRAMES]) {
		move_list[situation_kind].try_changing_to_buffered_status(this);
	}
}