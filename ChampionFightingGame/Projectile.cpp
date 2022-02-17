#include "Projectile.h"
#include "Battle.h"

extern SDL_Renderer* g_renderer;

void Projectile::projectile_main() {
	prev_pos = pos;

	if (canStep()) {
		stepAnimation();
	}

	playoutStatus();

	decrease_common_projectile_variables(this);
	tickOnceProjectileUnique();

	update_hitbox_pos(false);
	update_grabbox_pos();
	update_hurtbox_pos();
}

void Projectile::superInit() {
	load_anim_list();
	load_stats();
	loadStatusScripts();
	for (int i = 0; i < HITBOX_COUNT_MAX; i++) {
		hitboxes[i].init(this);
	}
	change_anim("default", 2, 0);
	change_status(PROJECTILE_STATUS_DEFAULT, false, false);
}

void Projectile::playoutStatus() {
	if (status_kind < PROJECTILE_STATUS_MAX) {
		(this->*status_script[status_kind])();
	}
	else {
		projectile_unique_status();
	}
	move_script.move_script();
}