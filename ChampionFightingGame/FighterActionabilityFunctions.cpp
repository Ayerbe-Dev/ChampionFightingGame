#include "Fighter.h"
#include "ParamAccessor.h"

bool Fighter::is_actionable() {
	if (anim_kind == nullptr) {
		return true;
	}
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0 && fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && status_kind != FIGHTER_STATUS_GRABBED && status_kind != FIGHTER_STATUS_THROWN) {
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

bool Fighter::can_kara() {
	if (((fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) && !fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS]) || fighter_flag[FIGHTER_FLAG_KARA_ENABLED]) {
		return true;
	}
	else {
		return false;
	}
}

bool Fighter::has_meter(int bars) {
	int ex_meter_size = get_param_int("ex_meter_size", PARAM_FIGHTER);
	int ex_meter_bars = get_param_int("ex_meter_bars", PARAM_FIGHTER);
	if (fighter_float[FIGHTER_FLOAT_SUPER_METER] >= ex_meter_size / (ex_meter_bars / bars)) {
		fighter_float[FIGHTER_FLOAT_SUPER_METER] -= ex_meter_size / (ex_meter_bars / bars);
		return true;
	}
	return false;
}