#pragma warning(disable : 4996)
#include "Fighter.h"
#include "ParamAccessor.h"
#include "GameManager.h"
#include "WindowConstants.h"
#include "utils.h"

bool Fighter::add_pos(glm::vec3 pos, bool prev) {
	Fighter* that = object_manager->fighter[!id];
	glm::vec3 prev_pos = this->pos;
	if (isnan(pos.x) || isnan(pos.y) || isnan(pos.z)) {
		GameManager* game_manager = GameManager::get_instance();
		game_manager->add_crash_log("Player: " + std::to_string(id + 1) + " Status: " + 
			std::to_string(status_kind) + ". Pos X: " + std::to_string(prev_pos.x) + ", Pos Y: " + 
			std::to_string(prev_pos.y) + ", Pos Z: " + std::to_string(prev_pos.z) + 
			". Probably tried to divide by 0 somewhere in the status. Opponent Status: " + 
			std::to_string(that->status_kind)
		);

		return false;
	}
	pos *= object_manager->get_world_rate(this);
	bool ret = true;
	this->pos += pos;

	if (this->pos.x > stage->stage_bounds.y) {
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			this->pos.x = stage->stage_bounds.y;
		}
		if (get_param_bool("has_wallbounce") && facing_right && status_kind == FIGHTER_STATUS_JUMP) { //Dunno if I'll keep this but it's sick af
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] *= -1;
		}
		ret = false;
	}
	if (this->pos.x < stage->stage_bounds.x) {
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			this->pos.x = stage->stage_bounds.x;
		}
		if (get_param_bool("has_wallbounce") && !facing_right && status_kind == FIGHTER_STATUS_JUMP) {
			object_float[BATTLE_OBJECT_FLOAT_X_SPEED] *= -1;
		}
		ret = false;
	}
	if (status_kind != FIGHTER_STATUS_GRABBED) {
		if (this->pos.y < 0) {
			if (prev) {
				this->pos.y = prev_pos.y;
			}
			else {
				this->pos.y = 0;
			}
			ret = false;
		}
		if (this->pos.y > WINDOW_HEIGHT) {
			if (prev) {
				this->pos.y = prev_pos.y;
			}
			else {
				this->pos.y = WINDOW_HEIGHT;
			}
			ret = false;
		}
	}
	float x_distance = std::max(this->pos.x, that->pos.x) - std::min(this->pos.x, that->pos.x);
	if (x_distance > get_global_param_float(PARAM_FIGHTER, "max_distance")) {
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			if (this->pos.x < that->pos.x) {
				this->pos.x = that->pos.x - get_global_param_float(PARAM_FIGHTER, "max_distance");
			}
			else {
				this->pos.x = that->pos.x + get_global_param_float(PARAM_FIGHTER, "max_distance");
			}
		}
		ret = false;
	}
	update_pushbox_pos();
	return ret;
}

bool Fighter::set_pos(glm::vec3 pos, bool prev) {
	Fighter* that = object_manager->fighter[!id];
	glm::vec3 prev_pos = this->pos;
	if (isnan(pos.x) || isnan(pos.y) || isnan(pos.z)) {
		GameManager* game_manager = GameManager::get_instance();
		game_manager->add_crash_log("Player: " + std::to_string(id + 1) + " Status: " +
			std::to_string(status_kind) + ". Pos X: " + std::to_string(prev_pos.x) + ", Pos Y: " +
			std::to_string(prev_pos.y) + ", Pos Z: " + std::to_string(prev_pos.z) +
			". Probably tried to divide by 0 somewhere in the status. Opponent Status: " + 
			std::to_string(that->status_kind)
		);

		return false;
	}

	bool ret = true;
	this->pos = pos;

	if (this->pos.x > stage->stage_bounds.y) {
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			this->pos.x = stage->stage_bounds.y;
		}
		ret = false;
	}
	if (this->pos.x < stage->stage_bounds.x) {
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			this->pos.x = stage->stage_bounds.x;
		}
		ret = false;
	}
	if (status_kind != FIGHTER_STATUS_GRABBED) {
		if (this->pos.y < 0) {
			if (prev) {
				this->pos.y = prev_pos.y;
			}
			else {
				this->pos.y = 0;
			}
			ret = false;
		}
		if (this->pos.y > WINDOW_HEIGHT) {
			if (prev) {
				this->pos.y = prev_pos.y;
			}
			else {
				this->pos.y = WINDOW_HEIGHT;
			}
			ret = false;
		}
	}
	float x_distance = std::max(this->pos.x, that->pos.x) - std::min(this->pos.x, that->pos.x);
	if (x_distance > get_global_param_float(PARAM_FIGHTER, "max_distance")) {
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			if (this->pos.x < that->pos.x) {
				this->pos.x = that->pos.x - get_global_param_float(PARAM_FIGHTER, "max_distance");
			}
			else {
				this->pos.x = that->pos.x + get_global_param_float(PARAM_FIGHTER, "max_distance");
			}
		}
		ret = false;
	}
	update_pushbox_pos();
	return ret;
}

bool Fighter::set_pos_anim() {
	if (anim_kind == nullptr) {
		return false;
	}
	if (anim_kind->flag_move) {
		Bone& trans_bone = model.bone_data[model.get_bone_id("Trans")];
		bool ret = add_pos(get_trans_offset());
		prev_anim_offset = glm::vec3(trans_bone.anim_matrix[3].z * facing_dir, trans_bone.anim_matrix[3].y, trans_bone.anim_matrix[3].x);
		return ret;
	}
	else {
		return false;
	}
}

void Fighter::landing_crossup() {
	Fighter* that = object_manager->fighter[!id];
	if (pos.x == that->pos.x) return;
	internal_facing_right = pos.x < that->pos.x;
	if (internal_facing_right != facing_right) {
		std::swap(object_int[FIGHTER_INT_66_STEP], object_int[FIGHTER_INT_44_STEP]);
		std::swap(object_int[FIGHTER_INT_66_TIMER], object_int[FIGHTER_INT_44_TIMER]);
		move_list[FIGHTER_CONTEXT_GROUND].swap_buffers("dash_f", "dash_b");
		facing_right = internal_facing_right;
		if (facing_right) {
			facing_dir = 1.0;
		}
		else {
			facing_dir = -1.0;
		}
	}
}

void Fighter::apply_gravity(float gravity, float max_fall_speed) {
	object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] = clampf(max_fall_speed * -1.0, object_float[BATTLE_OBJECT_FLOAT_Y_SPEED] - gravity * object_manager->get_world_rate(this), object_float[BATTLE_OBJECT_FLOAT_Y_SPEED]);
}