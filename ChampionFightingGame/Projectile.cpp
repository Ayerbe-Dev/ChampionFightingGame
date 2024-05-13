#include "Projectile.h"

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
	stop_sound_all();
	stop_reserved_sound();
	model.unload_model_instance();
	object_int.clear();
	object_float.clear();
	object_flag.clear();
	status_script.clear();
	enter_status_script.clear();
	exit_status_script.clear();
	anim_table.unload_animations();
	move_script_table.clear_scripts();
	param_table.unload_params();
}

void Projectile::projectile_main() {
	process_animate();
	process_position();
	process_pre_common_projectile_vars();
	projectile_unique_main();
	process_status();
	process_post_animate();
	process_post_common_projectile_vars();
}

void Projectile::projectile_post() {
	process_post_position();
}

void Projectile::process_status() {
	(this->*status_script[status_kind])();
	execute_active_script();
}

void Projectile::process_animate() {
	frame_delta = 0.0f;
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] != 0) {
		if (anim_kind != nullptr && !anim_kind->flag_no_hitlag_interp) {
			frame_delta = (0.2 / (float)(object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES])) * object_manager->get_world_rate(this);
		}
	}
	else {
		frame_delta = rate * object_manager->get_world_rate(this);
	}
	frame += frame_delta;

	if (anim_kind != nullptr) {
		if (frame >= anim_kind->length && anim_kind->length != -1) {
			frame = 0.0;
			active_move_script.activate();
			clear_hitbox_all();
			anim_end = true;
		}
		else {
			anim_end = false;
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
}

void Projectile::process_post_position() {
	update_hitbox_pos();
	update_hurtbox_pos();
	update_grabbox_pos();
	update_pushbox_pos();
	update_blockbox_pos();
	if (!is_in_camera_range() && object_flag[PROJECTILE_FLAG_DESPAWN_ON_OOB]) {
		deactivate();
	}
}

void Projectile::process_pre_common_projectile_vars() {
	if (!object_manager->is_allow_realtime_process(this)) return;
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] != 0) {
		object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES]--;
		if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0 && anim_kind != nullptr
			&& !anim_kind->flag_no_hitlag_interp) {
			object_int[BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES] = 0;
			frame = object_float[BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME];
		}
	}
	else {
		if (object_int[PROJECTILE_INT_ACTIVE_TIME] > 0) {
			object_int[PROJECTILE_INT_ACTIVE_TIME]--;
		}
	}
}

void Projectile::process_post_common_projectile_vars() {
	if (!object_manager->is_allow_realtime_process(this)) return;
}