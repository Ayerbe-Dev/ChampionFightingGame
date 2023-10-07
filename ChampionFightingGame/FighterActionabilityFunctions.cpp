#include "Fighter.h"
#include "ParamAccessor.h"

bool Fighter::is_actionable() {
	if (battle_object_manager->world_rate == 0.0f && battle_object_manager->real_time_id != id) {
		return false; //Disable all actions in super freeze no matter what
	}
	if (anim_kind == nullptr) {
		return true;
	}
	if (!fighter_flag[FIGHTER_FLAG_GRABBED]
		&& status_kind != FIGHTER_STATUS_THROWN
		&& (get_status_group() != STATUS_GROUP_HITSTUN || ((status_kind == FIGHTER_STATUS_HITSTUN 
		|| status_kind == FIGHTER_STATUS_HITSTUN_AIR) && !fighter_int[FIGHTER_INT_HITSTUN_FRAMES]))
		&& !fighter_int[FIGHTER_INT_INIT_HITLAG_FRAMES]
		&& status_kind != FIGHTER_STATUS_ROUND_END) {
		if (fighter_flag[FIGHTER_FLAG_ALLOW_CANCEL_RECOVERY]) {
			return true;
		}
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

bool Fighter::has_meter(int bars) {
	int ex_meter_size = get_param_int(PARAM_FIGHTER, "ex_meter_size");
	int ex_meter_bars = get_param_int(PARAM_FIGHTER, "ex_meter_bars");
	if (fighter_float[FIGHTER_FLOAT_EX_METER] >= ex_meter_size / (ex_meter_bars / bars)) {
		return true;
	}
	return false;
}

void Fighter::spend_meter(int bars) {
	int ex_meter_size = get_param_int(PARAM_FIGHTER, "ex_meter_size");
	int ex_meter_bars = get_param_int(PARAM_FIGHTER, "ex_meter_bars");
	fighter_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER] = 60;
	fighter_float[FIGHTER_FLOAT_EX_METER] -= ex_meter_size / (ex_meter_bars / bars);
}

void Fighter::enable_all_cancels() {
	for (int i = 0; i < CANCEL_CAT_MAX; i++) {
		for (int i2 = 0; i2 < CANCEL_KIND_MAX; i2++) {
			cancel_flags[i][i2] = true;
		}
	}
}

void Fighter::enable_cancel(int cat, int kind) {
	if (cat >= CANCEL_CAT_MAX) {
		int max = cat - CANCEL_CAT_MAX;
		for (int i = 0; i < max; i++) {
			cancel_flags[i][kind] = true;
		}
	}
	else {
		cancel_flags[cat][kind] = true;
	}
}

void Fighter::disable_all_cancels() {
	for (int i = 0; i < CANCEL_CAT_MAX; i++) {
		for (int i2 = 0; i2 < CANCEL_KIND_MAX; i2++) {
			cancel_flags[i][i2] = false;
		}
	}
}

void Fighter::disable_cancel(int cat, int kind) {
	if (cat >= CANCEL_CAT_MAX) {
		int max = cat - CANCEL_CAT_MAX;
		for (int i = 0; i < max; i++) {
			cancel_flags[i][kind] = false;
		}
	}
	else {
		cancel_flags[cat][kind] = false;
	}
}

bool Fighter::is_enable_cancel(int cancel_kind) {
	if (cancel_kind == CANCEL_KIND_MAX) {
		return true;
	}
	else if (cancel_kind < CANCEL_KIND_MAX) {
		if (fighter_flag[FIGHTER_FLAG_ATTACK_HIT] || fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED]) {
			return (fighter_flag[FIGHTER_FLAG_ATTACK_HIT] && cancel_flags[CANCEL_CAT_HIT][cancel_kind])
				|| (fighter_flag[FIGHTER_FLAG_ATTACK_BLOCKED] && cancel_flags[CANCEL_CAT_BLOCK][cancel_kind]);
		}
		else {
			return cancel_flags[CANCEL_CAT_WHIFF][cancel_kind];
		}
	}
	return false;
}