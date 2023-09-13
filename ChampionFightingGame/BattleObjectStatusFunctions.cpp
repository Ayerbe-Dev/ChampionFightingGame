#include "BattleObject.h"

bool BattleObject::change_status(unsigned int new_status_kind, bool call_end_status, bool require_different_status) {
	if (status_kind != new_status_kind || !require_different_status) {
		clear_hitbox_all();
		clear_hurtbox_all();
		clear_grabbox_all();
		clear_pushbox_all();
		status_kind = new_status_kind;
		return true;
	}
	return false;
}