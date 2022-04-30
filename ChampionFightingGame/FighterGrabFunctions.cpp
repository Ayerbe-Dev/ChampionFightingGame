#include "Fighter.h"

void Fighter::grab_opponent(std::string bone_name, glm::vec2 offset, int frames) {
	Fighter* that = battle_object_manager->fighter[!id];
	int index = model.get_bone_id(bone_name);
	if (index == -1) {
		return;
	}
	that->fighter_int[FIGHTER_INT_GRAB_INIT_POS_CHANGE_FRAMES] = frames;
	that->fighter_int[FIGHTER_INT_GRAB_POS_CHANGE_FRAMES] = frames;
	that->fighter_int[FIGHTER_INT_GRAB_BONE_ID] = index;
	that->fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_X] = offset.x * facing_dir;
	that->fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_Y] = offset.y;
	that->fighter_flag[FIGHTER_FLAG_GRABBED] = true;
}

void Fighter::throw_opponent(float damage, float x_speed, float y_speed) {
	Fighter* that = battle_object_manager->fighter[!id];
	that->fighter_float[FIGHTER_FLOAT_HEALTH] -= damage;
	that->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = x_speed * this->facing_dir;
	that->fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = y_speed;
	that->fighter_flag[FIGHTER_FLAG_GRABBED] = false;
	that->change_status(FIGHTER_STATUS_THROWN);
}