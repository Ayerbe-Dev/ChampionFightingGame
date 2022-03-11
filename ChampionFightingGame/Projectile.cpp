#include "Projectile.h"
#include "Battle.h"

extern SDL_Renderer* g_renderer;

void Projectile::projectile_main() {
	process_animate();
	process_position();
	projectile_unique_main();
	process_status();
	process_post_animate();
	decrease_common_variables();
	process_post_position();
}

void Projectile::process_status() {
	(this->*status_script[status_kind])();
	move_script.move_script();
}

void Projectile::process_animate() {
	attempted_excutes = 0;
	
	if (anim_kind != nullptr) {
		frame += rate;
		if (frame >= anim_kind->length) {
			frame = 0.0;
			excute_count = 0;
			clear_hitbox_all();
			is_anim_end = true;
		}
		else {
			is_anim_end = false;
		}
	}
}

void Projectile::process_post_animate() {
	if (anim_kind != nullptr) {
		model.set_bones(frame, anim_kind);
	}
	else {
		model.reset_bones();
	}
}

void Projectile::process_position() {
	prev_pos = pos;
}

void Projectile::process_post_position() {
	update_hitbox_pos();
}

void Projectile::decrease_common_variables() {
	if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] > 0) {
		projectile_int[PROJECTILE_INT_ACTIVE_TIME] --;
	}
	if (projectile_int[PROJECTILE_INT_HITLAG_FRAMES] != 0) {
		projectile_int[PROJECTILE_INT_HITLAG_FRAMES] --;
	}
}