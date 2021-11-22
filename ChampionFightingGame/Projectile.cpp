#include "Projectile.h"
#include "Game.h"

extern SDL_Renderer* g_renderer;

void Projectile::projectile_main() {
	prevpos = pos;

	if (canStep()) {
		stepAnimation();
	}

	playoutStatus();

	decrease_common_projectile_variables(this);
	tickOnceProjectileUnique();

	int width;
	int height;
	SDL_QueryTexture(base_texture, NULL, NULL, &width, &height);
	pos.x_spr_offset = width / 2;
	pos.y_spr_offset = height;

	update_hitbox_pos(false);
	update_grabbox_pos();
	update_hurtbox_pos();
}

void Projectile::superInit() {
	load_anim_list();
	load_stats();
	loadStatusScripts();
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
	move_script();
}