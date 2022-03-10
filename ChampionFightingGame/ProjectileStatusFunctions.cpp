#include "Projectile.h"

bool Projectile::change_status(unsigned int new_status_kind, bool call_end_status, bool require_different_status) {
	if (new_status_kind != status_kind || !require_different_status) {
		clear_hitbox_all();
		clear_grabbox_all();
		clear_hurtbox_all();
		projectile_flag[PROJECTILE_FLAG_HIT_IN_STATUS] = false;
		if (call_end_status) {
			if (status_kind < PROJECTILE_STATUS_MAX) {
				(this->*exit_status_script[status_kind])();
			}
			else {
				projectile_unique_exit_status();
			}
		}
		status_kind = new_status_kind;
		if (status_kind < PROJECTILE_STATUS_MAX) {
			(this->*enter_status_script[status_kind])();
		}
		else {
			projectile_unique_enter_status();
		}
		return true;
	}
	else {
		return false;
	}
}