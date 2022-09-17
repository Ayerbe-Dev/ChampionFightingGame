#include "EricFireball.h"
#include "EricFireballConstants.h"

void EricFireball::projectile_unique_main() {
	if (projectile_int[PROJECTILE_INT_HEALTH] == 0) {
		change_status(PROJECTILE_STATUS_HIT);
	}
}

void EricFireball::status_default() {
	change_status(PROJECTILE_STATUS_MOVE);
}

void EricFireball::load_projectile_unique_status_scripts() {
	status_script.resize(PROJECTILE_ERIC_FIREBALL_STATUS_MAX, nullptr);
	enter_status_script.resize(PROJECTILE_ERIC_FIREBALL_STATUS_MAX, nullptr);
	exit_status_script.resize(PROJECTILE_ERIC_FIREBALL_STATUS_MAX, nullptr);
}