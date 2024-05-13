#include "Fighter.h"
#include "ParamAccessor.h"

bool Fighter::is_actionable() {
	if (object_manager->world_rate == 0.0f && object_manager->real_time_object != this) {
		return false; //Disable all actions in super freeze no matter what
	}
	if (anim_kind == nullptr) {
		return true;
	}
	if (object_flag[FIGHTER_FLAG_IN_THROW_TECH_WINDOW]
		|| object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES]
		|| status_kind == FIGHTER_STATUS_ROUND_END
		|| object_int[FIGHTER_INT_STATUS_GROUP] & STATUS_GROUP_NON_END_WITH_ANIM
		|| object_int[FIGHTER_INT_FORCE_RECOVERY_FRAMES]) {
		return false;
	}

	if (anim_kind->faf == -1) {
		return std::ceil(frame) >= anim_kind->length;
	}
	else {
		return std::ceil(frame) >= anim_kind->faf;
	}
}

bool Fighter::is_enable_free_cancel() {
	return object_flag[FIGHTER_FLAG_ALLOW_FREE_CANCEL] || is_actionable();
}

void Fighter::enable_free_cancel() {
	object_flag[FIGHTER_FLAG_ALLOW_FREE_CANCEL] = true;
}

void Fighter::enable_cancel(std::string move_kind, CancelKind cancel_kind) {
	move_list[fighter_context].enable_cancel(move_kind, cancel_kind);
}

void Fighter::disable_all_cancels() {
	for (int i = 0; i < FIGHTER_CONTEXT_MAX; i++) {
		move_list[i].disable_all_cancels();
	}
}

void Fighter::disable_cancel(std::string move_kind, CancelKind cancel_kind) {
	move_list[fighter_context].disable_cancel(move_kind, cancel_kind);
}

bool Fighter::is_enable_cancel(std::string move_kind) {
	return move_list[fighter_context].is_enable_cancel(this, move_kind);
}

FighterMoveListEntry Fighter::get_curr_move() {
	return move_list[fighter_context].get_move(object_string[FIGHTER_STRING_MOVE_KIND]);
}

void Fighter::check_movelist_inputs() {
	if (!object_flag[FIGHTER_FLAG_ROUND_START]) {
		move_list[fighter_context].check_inputs(this);
	}
	if (!object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]) {
		move_list[fighter_context].try_changing_to_buffered_status(this);
	}
}