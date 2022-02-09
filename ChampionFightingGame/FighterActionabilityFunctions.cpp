#include "Fighter.h"

bool Fighter::is_actionable() {
	if (fighter_int[FIGHTER_INT_HITSTUN_FRAMES] == 0 && fighter_int[FIGHTER_INT_HITLAG_FRAMES] == 0 && status_kind != FIGHTER_STATUS_GRABBED && status_kind != FIGHTER_STATUS_THROWN) {
		if (anim_kind->faf == -1) {
			return frame >= anim_kind->length;
		}
		else {
			return frame >= anim_kind->faf;
		}
	}
	else {
		return false;
	}
}

bool Fighter::can_kara() {
	if (((fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LP || fighter_int[FIGHTER_INT_ATTACK_KIND] == ATTACK_KIND_LK) && !fighter_flag[FIGHTER_FLAG_HAD_ATTACK_IN_STATUS]) || kara_enabled) {
		return true;
	}
	else {
		return false;
	}
}