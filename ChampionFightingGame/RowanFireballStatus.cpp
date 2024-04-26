#include "RowanFireball.h"
#include "RowanFireballConstants.h"

void RowanFireball::unique_process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) {
	if (status_kind == PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER) {
		object_int[PROJECTILE_INT_HEALTH]++;
	}
}

void RowanFireball::status_move() {
	if (object_int[BATTLE_OBJECT_INT_HITLAG_FRAMES] == 0) {
		add_pos(glm::vec3(object_float[BATTLE_OBJECT_FLOAT_X_SPEED], object_float[BATTLE_OBJECT_FLOAT_Y_SPEED], 0));
		object_int[PROJECTILE_INT_ELAPSED_FRAMES]++;
		if (object_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
			deactivate();
		}
	}
}

void RowanFireball::enter_status_move() {
	object_int[PROJECTILE_INT_OWNER_ENDLAG] = owner->get_frames_until_actionable();
	object_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("punch_active_time");
	object_flag[PROJECTILE_FLAG_DESPAWN_ON_OOB] = true;
	switch (owner->object_int[CHARA_ROWAN_INT_FIREBALL_LEVEL]) {
		case SPECIAL_LEVEL_L: {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("punch_move_x_speed_l") * facing_dir;
		} break;
		case SPECIAL_LEVEL_M: {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("punch_move_x_speed_m") * facing_dir;
		} break;
		case SPECIAL_LEVEL_H:
		case SPECIAL_LEVEL_EX: {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] = get_param_float("punch_move_x_speed_h") * facing_dir;
		} break;
	}
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = 0.0;
	change_anim("punched");
}

void RowanFireball::exit_status_move() {
	clear_effect("flame");
}

void RowanFireball::status_rowan_fireball_hover() {
	if (object_int[PROJECTILE_INT_ACTIVE_TIME] == 0) {
		if (owner->status_kind != CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH
			&& owner->status_kind != CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK) {
			deactivate();
		}
	}
}

void RowanFireball::enter_status_rowan_fireball_hover() {
	set_pos(owner->get_bone_position("HaveL", glm::vec3(35.0, 0.0, 0.0)));
	object_int[PROJECTILE_INT_ACTIVE_TIME] = get_param_int("hover_active_time");
	change_anim("hover");
}

void RowanFireball::exit_status_rowan_fireball_hover() {
	clear_effect_all();
}

void RowanFireball::load_projectile_unique_status_scripts() {
	SET_STATUS_FUNC(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER, &RowanFireball::status_rowan_fireball_hover);
	SET_ENTRY_STATUS_FUNC(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER, &RowanFireball::enter_status_rowan_fireball_hover);
	SET_EXIT_STATUS_FUNC(PROJECTILE_ROWAN_FIREBALL_STATUS_HOVER, &RowanFireball::exit_status_rowan_fireball_hover);
}