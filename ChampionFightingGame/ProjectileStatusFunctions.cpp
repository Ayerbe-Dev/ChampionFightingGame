#include "Projectile.h"

void Projectile::change_status(unsigned int new_status_kind, bool call_end_status) {
	clear_hitbox_all();
	clear_grabbox_all();
	clear_hurtbox_all();
	clear_pushbox_all();
	object_flag[PROJECTILE_FLAG_ATTACK_HIT] = false;
	if (call_end_status) {
		(this->*exit_status_script[status_kind])();
	}
	prev_status_kind = status_kind;
	status_kind = new_status_kind;
	(this->*enter_status_script[status_kind])();
}