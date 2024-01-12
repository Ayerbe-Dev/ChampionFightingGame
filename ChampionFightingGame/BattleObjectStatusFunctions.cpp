#include "BattleObject.h"

void BattleObject::change_status(unsigned int new_status_kind, bool call_end_status) {
	clear_hitbox_all();
	clear_hurtbox_all();
	clear_grabbox_all();
	clear_pushbox_all();
	status_kind = new_status_kind;
}