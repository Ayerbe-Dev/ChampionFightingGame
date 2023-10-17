#include "Projectile.h"
#include "Battle.h"

Projectile::Projectile() {
	owner = nullptr;
	owner_id = 0;
	projectile_kind = 0;
	object_type = BATTLE_OBJECT_TYPE_PROJECTILE;
}

Projectile::~Projectile() {
	for (int i = 0; i < 10; i++) {
		hitboxes[i].rect.destroy();
		hurtboxes[i].rect.destroy();
		grabboxes[i].rect.destroy();
		pushboxes[i].rect.destroy();
	}
	blockbox.rect.destroy();
	stop_se_all();
	stop_vc_all();
	model.unload_model_instance();
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
	if (projectile_int[PROJECTILE_INT_HITLAG_FRAMES] == 0) {
		projectile_unique_main();
		process_status();
	}
	process_post_animate();
	if (battle_object_manager->allow_dec_var(id)) {
		decrease_common_variables();
	}
}

void Projectile::projectile_post() {
	process_post_position();
}

void Projectile::process_status() {
	(this->*status_script[status_kind])();
	active_move_script.execute(this, frame);
}

void Projectile::process_animate() {	
	if (projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] != 0) {
		if (anim_kind != nullptr && !anim_kind->flag_no_hitlag_interp) {
			frame += (0.2 / (float)(projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES])) * battle_object_manager->get_world_rate(id);
		}
	}
	else {
		frame += rate * battle_object_manager->get_world_rate(id);
	}

	if (anim_kind != nullptr) {
		if (frame >= anim_kind->length && anim_kind->length != -1) {
			frame = 0.0;
			active_move_script.activate();
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
			model.set_bones(frame, anim_kind);
		}
		else {
			model.reset_bones();
		}
	}
}

void Projectile::process_position() {
	prev_pos = pos;
	if (pos.x < stage->stage_bounds.x || pos.x > stage->stage_bounds.y) {
		change_status(PROJECTILE_STATUS_DEACTIVATE);
	}
}

void Projectile::process_post_position() {
	update_hitbox_pos();
	update_hurtbox_pos();
	update_grabbox_pos();
	update_pushbox_pos();
	update_blockbox_pos();
}

void Projectile::decrease_common_variables() {
	if (projectile_int[PROJECTILE_INT_HITLAG_FRAMES] != 0) {
		projectile_int[PROJECTILE_INT_HITLAG_FRAMES] --;
		if (projectile_int[PROJECTILE_INT_HITLAG_FRAMES] == 0 && anim_kind != nullptr
			&& !anim_kind->flag_no_hitlag_interp) {
			projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = 0;
			frame -= 0.2;
		}
	}
	else {
		if (projectile_int[PROJECTILE_INT_ACTIVE_TIME] > 0) {
			projectile_int[PROJECTILE_INT_ACTIVE_TIME] --;
		}
	}
}