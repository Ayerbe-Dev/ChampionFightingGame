#pragma warning(disable : 4996)
#include "Fighter.h"

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
		char buffer_1[73];
		sprintf(buffer_1, "Player %d (Me) Status: %d. Pos X: %f, Pos Y: %f, Pos Z: %f. You probably", (id + 1), status_kind, prev_pos.x, prev_pos.y, prev_pos.z);
		char buffer_2[89];
		sprintf(buffer_2, "accidentally divided by 0 somewhere in that status. Player %d (Not Me) Status: %d. (Add)", ((!id) + 1), that->status_kind);
		battle_object_manager->fighter[0]->player_info->crash_reason = buffer_1;
		battle_object_manager->fighter[1]->player_info->crash_reason = buffer_2;

		crash_to_debug = true;
		return false;
	}

	//Ok now to actually set some positions

	bool ret = true;

	//Check if the front quarter or so of our sprite is inside of the front quarter of the opponent's BEFORE we changed the position. We'll check this
	//again after and compare the values. Note: The reason we divide by 4 for the offset and not 2 is because we want the sprites to be able to partly 
	//overlap and let the jostle boxes create a more natural looking pushback. 

	float this_x_front = this->pos.x + ((this->jostle_box.corners[2].x - this->pos.x) * facing_dir / 4);
	float that_x_front = that->pos.x + ((that->jostle_box.corners[2].x - that->pos.x) * that->facing_dir / 4);
	bool opponent_right = this_x_front > that_x_front;

	//Add positions, then do a whole bunch of checks to see if we'll need to change to a different position.

	this->pos += pos;

	//Note: The prev arg determines what should happen should a position check fail. If it's true, the changes to position on that axis are canceled
	//completely. If not, the position is moved to the closest valid position.

	if (this->pos.x > WINDOW_WIDTH / 2) { //If you went out of the horizontal bounds
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			this->pos.x = WINDOW_WIDTH / 2;
		}
		if (get_param_bool("has_wallbounce") && facing_right && status_kind == FIGHTER_STATUS_JUMP) { //Dunno if I'll keep this but it's sick af
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] *= -1;
		}
		ret = false;
	}
	if (this->pos.x < WINDOW_WIDTH / -2) {
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			this->pos.x = WINDOW_WIDTH / -2;
		}
		if (get_param_bool("has_wallbounce") && !facing_right && status_kind == FIGHTER_STATUS_JUMP) {
			fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] *= -1;
		}
		ret = false;
	}
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

	//Check if a player is about to walk out of the camera range even if they would stay in bounds.

	float this_x_back = this->pos.x + ((this->jostle_box.corners[2].x - this->pos.x) * this->facing_dir / -2);
	float that_x_back = that->pos.x + ((that->jostle_box.corners[2].x - that->pos.x) * that->facing_dir / -2);
	float x_distance = std::max(this_x_back, that_x_back) - std::min(this_x_back, that_x_back);
	if (x_distance > CAMERA_MAX_ZOOM_OUT) {
		this->pos.x = prev_pos.x; //I don't know what the calculation for "make it so you're as close as possible to the max distance without going over" would
		//look like, and frankly I don't care enough to do it
		ret = false;
	}

	//Check to see if you crossed up the opponent by changing positions

	float new_this_x_front = this->pos.x + ((this->jostle_box.corners[2].x - this->pos.x) * facing_dir / 4);
	float new_that_x_front = that->pos.x + ((that->jostle_box.corners[2].x - that->pos.x) * that->facing_dir / 4);
	bool new_opponent_right = new_this_x_front > new_that_x_front;

	if (opponent_right != new_opponent_right && !fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] && situation_kind == FIGHTER_SITUATION_GROUND
		&& that->situation_kind == FIGHTER_SITUATION_GROUND && this->pos.x * facing_dir > 0) {
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
		char buffer_1[73];
		sprintf(buffer_1, "Player %d (Me) Status: %d. Pos X: %f, Pos Y: %f, Pos Z: %f. You probably", (id + 1), status_kind, prev_pos.x, prev_pos.y, prev_pos.z);
		char buffer_2[89];
		sprintf(buffer_2, "accidentally divided by 0 somewhere in that status. Player %d (Not Me) Status: %d. (Add)", ((!id) + 1), that->status_kind);
		battle_object_manager->fighter[0]->player_info->crash_reason = buffer_1;
		battle_object_manager->fighter[1]->player_info->crash_reason = buffer_2;

		crash_to_debug = true;
		return false;
	}

	//Ok now to actually set some positions

	bool ret = true;

	float this_x_front = this->pos.x + ((this->jostle_box.corners[2].x - this->pos.x) * facing_dir / 2);
	float that_x_front = that->pos.x + ((that->jostle_box.corners[2].x - that->pos.x) * that->facing_dir / 2);
	bool opponent_right = this_x_front > that_x_front;
	this->pos = pos;
	if (this->pos.x > WINDOW_WIDTH / 2) {
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			this->pos.x = WINDOW_WIDTH / 2;
		}
		ret = false;
	}
	if (this->pos.x < WINDOW_WIDTH / -2) {
		if (prev) {
			this->pos.x = prev_pos.x;
		}
		else {
			this->pos.x = WINDOW_WIDTH / -2;
		}
		ret = false;
	}
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
	float this_x_back = this->pos.x + ((this->jostle_box.corners[2].x - this->pos.x) * this->facing_dir / -2);
	float that_x_back = that->pos.x + ((that->jostle_box.corners[2].x - that->pos.x) * that->facing_dir / -2);
	float x_distance = std::max(this_x_back, that_x_back) - std::min(this_x_back, that_x_back);
	if (x_distance > CAMERA_MAX_ZOOM_OUT) {
		this->pos.x = prev_pos.x;
		ret = false;
	}
	float new_this_x_front = this->pos.x + ((this->jostle_box.corners[2].x - this->pos.x) * facing_dir / 2);
	float new_that_x_front = that->pos.x + ((that->jostle_box.corners[2].x - that->pos.x) * that->facing_dir / 2);
	bool new_opponent_right = new_this_x_front > new_that_x_front;

	if (opponent_right != new_opponent_right && !fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] && situation_kind == FIGHTER_SITUATION_GROUND && that->situation_kind == FIGHTER_SITUATION_GROUND) {
		this->pos.x = prev_pos.x;
		ret = false;
	}

	return ret;
}

bool Fighter::set_pos(float x, float y, float z, bool prev) {
	return set_pos(glm::vec3(x, y, z), prev);
}