#pragma warning(disable : 4996)
#include "Fighter.h"
#include "ParamAccessor.h"
#include "GameManager.h"

bool Fighter::add_pos(glm::vec3 pos, bool prev) {
	Fighter* that = battle_object_manager->fighter[!id]; //Get the opponent's Fighter, since we'll need to use them a lot
	glm::vec3 prev_pos = this->pos;
	//Check if the X or Y coord is -0.0. If it is, we shouldn't necessarily cause a crash since sometimes that'd cause stuff to break, but it's
	//still helpful to know for debugging
	if (pos.x == -0.0) {
		pos.x = 0.0;
	}
	if (pos.y == -0.0) {
		pos.y = 0.0;
	}
	if (pos.z == -0.0) {
		pos.z = 0.0;
	}
	if (isnan(pos.x) || isnan(pos.y) || isnan(pos.z)) { //If we're trying to add something that isn't a number, crash to debug and print both the statuses and our 
		//previous X/Y coords. This will make debugging easier.
		GameManager* game_manager = GameManager::get_instance();
		game_manager->add_crash_log("Player: " + std::to_string(id + 1) + " Status: " + 
			std::to_string(status_kind) + ". Pos X: " + std::to_string(prev_pos.x) + ", Pos Y: " + 
			std::to_string(prev_pos.y) + ", Pos Z: " + std::to_string(prev_pos.z) + 
			". Probably tried to divide by 0 somewhere in the status. Opponent Status: " + 
			std::to_string(that->status_kind)
		);

		return false;
	}
	pos *= battle_object_manager->get_time_multiplier(id);

	//Ok now to actually set some positions

	float this_x_front = this->jostle_box.corners[2].x - (((this->jostle_box.corners[2].x - this->pos.x) * this->facing_dir) / 2 * this->facing_dir);
	float that_x_front = that->jostle_box.corners[2].x - (((that->jostle_box.corners[2].x - that->pos.x) * that->facing_dir) / 2 * that->facing_dir);
	bool ret = true;
	bool opponent_right = this_x_front > that_x_front;

	//Add positions, then do a whole bunch of checks to see if we'll need to change to a different position.

	this->pos += pos;

	update_jostle_rect();

	//Note: The prev arg determines what should happen should a position check fail. If it's true, the changes to position on that axis are canceled
	//completely. If not, the position is moved to the closest valid position.

	if (this->pos.x > stage->stage_bounds.y) { //If you went out of the horizontal bounds
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			this->pos.x = stage->stage_bounds.y;
		}
		if (get_local_param_bool("has_wallbounce") && facing_right && status_kind == FIGHTER_STATUS_JUMP) { //Dunno if I'll keep this but it's sick af
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] *= -1;
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
		if (get_local_param_bool("has_wallbounce") && !facing_right && status_kind == FIGHTER_STATUS_JUMP) {
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] *= -1;
		}
		ret = false;
	}
	if (!fighter_flag[FIGHTER_FLAG_GRABBED]) {
		if (this->pos.y < 0) { //If you're about to land in the floor
			if (prev) {
				this->pos.y = prev_pos.y;
			}
			else {
				this->pos.y = 0; //Pretty sure this will get autocorrected to FLOOR_GAMECOORD on the next frame anyway
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

	//Check if a player is about to walk out of the camera range even if they would stay in bounds.

	float this_x_back = this->jostle_box.corners[0].x;
	float that_x_back = that->jostle_box.corners[0].x;
	float x_distance = std::max(this_x_back, that_x_back) - std::min(this_x_back, that_x_back);
	if (x_distance > get_param_float("max_distance", PARAM_FIGHTER)) {
		this->pos.x = prev_pos.x; //I don't know what the calculation for "make it so you're as close as possible to the max distance without going over" would
		//look like, and frankly I don't care enough to do it
		ret = false;
	}

	//Check to see if you crossed up the opponent by changing positions

	float new_this_x_front = this->jostle_box.corners[2].x - (((this->jostle_box.corners[2].x - this->pos.x) * this->facing_dir) / 2 * this->facing_dir);
	bool new_opponent_right = new_this_x_front > that_x_front;

	if (opponent_right != new_opponent_right  //We crossed the front of the opponent's jostle box
		&& (this_x_front - new_this_x_front) * facing_dir < 0 //We're moving towards them
		&& !fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] //We aren't allowed to cross up
		&& situation_kind == FIGHTER_SITUATION_GROUND //We're on the ground
		&& (that->situation_kind == FIGHTER_SITUATION_GROUND || that->situation_kind == FIGHTER_SITUATION_DOWN)) { //And so are they
		this->pos.x = prev_pos.x;
		ret = false;
	}

	return ret;
}

bool Fighter::add_pos(float x, float y, float z, bool prev) {
	return add_pos(glm::vec3(x, y, z), prev);
}

bool Fighter::set_pos(glm::vec3 pos, bool prev) {
	Fighter* that = battle_object_manager->fighter[!id]; //Get the opponent's Fighter, since we'll need to use them a lot
	glm::vec3 prev_pos = this->pos;
	//Check if the X or Y coord is -0.0. If it is, we shouldn't necessarily cause a crash since sometimes that'd cause stuff to break, but it's
	//still helpful to know for debugging
	if (pos.x == -0.0) {
		pos.x = 0.0;
	}
	if (pos.y == -0.0) {
		pos.y = 0.0;
	}
	if (pos.z == -0.0) {
		pos.z = 0.0;
	}
	if (isnan(pos.x) || isnan(pos.y) || isnan(pos.z)) { //If we're trying to add something that isn't a number, crash to debug and print both the statuses and our 
		//previous X/Y coords. This will make debugging easier.
		GameManager* game_manager = GameManager::get_instance();
		game_manager->add_crash_log("Player: " + std::to_string(id + 1) + " Status: " +
			std::to_string(status_kind) + ". Pos X: " + std::to_string(prev_pos.x) + ", Pos Y: " +
			std::to_string(prev_pos.y) + ", Pos Z: " + std::to_string(prev_pos.z) +
			". Probably tried to divide by 0 somewhere in the status. Opponent Status: " + 
			std::to_string(that->status_kind)
		);

		return false;
	}

	//Ok now to actually set some positions

	bool ret = true;

	bool opponent_right = this->pos.x > that->pos.x;

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
	if (!fighter_flag[FIGHTER_FLAG_GRABBED]) {
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
	float this_x_back = this->pos.x + ((this->jostle_box.corners[2].x - this->pos.x) * this->facing_dir / -2);
	float that_x_back = that->pos.x + ((that->jostle_box.corners[2].x - that->pos.x) * that->facing_dir / -2);
	float x_distance = std::max(this_x_back, that_x_back) - std::min(this_x_back, that_x_back);
	if (x_distance > get_param_float("max_distance", PARAM_FIGHTER)) {
		this->pos.x = prev_pos.x;
		ret = false;
	}
	bool new_opponent_right = this->pos.x > that->pos.x;

	if (opponent_right != new_opponent_right && !fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] && situation_kind == FIGHTER_SITUATION_GROUND && that->situation_kind == FIGHTER_SITUATION_GROUND) {
		this->pos.x = prev_pos.x;
		ret = false;
	}

	return ret;
}

bool Fighter::set_pos(float x, float y, float z, bool prev) {
	return set_pos(glm::vec3(x, y, z), prev);
}

bool Fighter::set_pos_anim() {
	if (anim_kind == nullptr) {
		return false;
	}
	if (anim_kind->move) {
		Bone& trans_bone = model.bones[model.get_bone_id("Trans")];
		glm::vec3 trans_offset = glm::vec3(
			trans_bone.anim_matrix[3].z * facing_dir,
			trans_bone.anim_matrix[3].y,
			0.0
		);
		trans_offset -= prev_anim_offset;
		trans_offset /= scale;

		bool ret = add_pos(trans_offset);
		prev_anim_offset = glm::vec3(trans_bone.anim_matrix[3].z * facing_dir, trans_bone.anim_matrix[3].y, 0.0);
		return ret;
	}
	else {
		return false;
	}
}

void Fighter::landing_crossup() {
	Fighter* that = battle_object_manager->fighter[!id];
	if (is_collide(jostle_box, that->jostle_box) && pos.x == that->pos.x) {
		bool prev_allowed_crossup = fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP];
		bool that_prev_allowed_crossup = that->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP];
		fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
		that->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
		if (that->internal_facing_right) {
			add_pos(-20.0, 0.0);
			that->add_pos(20.0, 0.0);
		}
		else {
			add_pos(20.0, 0.0);
			that->add_pos(-20.0, 0.0);
		}
		fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = prev_allowed_crossup;
		that->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = that_prev_allowed_crossup;
	}
}

void Fighter::apply_gravity(float gravity, float max_fall_speed) {
	fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = clampf(max_fall_speed * -1.0, fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] - gravity * battle_object_manager->get_time_multiplier(id), fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED]);
}