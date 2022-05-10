#include "Projectile.h"
#include "Battle.h"

Projectile::Projectile() {}

Projectile::~Projectile() {
	for (int i = 0; i < 10; i++) {
		hitboxes[i].rect.destroy();
		hurtboxes[i].rect.destroy();
		grabboxes[i].rect.destroy();
	}
	jostle_box.destroy();
	model.unload_model();
	projectile_int.clear();
	projectile_float.clear();
	projectile_flag.clear();
	status_script.clear();
	enter_status_script.clear();
	exit_status_script.clear();
	anim_table.unload_animations();
	move_script_table.wipe_scripts();
	params.unload_params();
	stats.unload_params();
}

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
	
	frame += rate;

	if (anim_kind != nullptr) {
		if (frame >= anim_kind->length && anim_kind->length != -1) {
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
	if (has_model) {
		if (anim_kind != nullptr) {
			model.set_bones(frame, anim_kind, !owner->facing_right);
		}
		else {
			model.reset_bones();
		}
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