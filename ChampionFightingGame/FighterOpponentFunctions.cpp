#include "Fighter.h"
#include <glm/gtx/string_cast.hpp>

void Fighter::set_opponent_offset(glm::vec2 offset, int frames) {
	glm::vec2 new_offset;
	new_offset.x = this->pos.x + (offset.x * facing_dir);
	new_offset.y = this->pos.y + offset.y;

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_X] =
		(fighter_accessor->fighter[!id]->pos.x - new_offset.x) * fighter_accessor->fighter[!id]->facing_dir / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_Y] =
		(fighter_accessor->fighter[!id]->pos.y - new_offset.y) / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES] = frames + 1;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] = new_offset.x;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] = new_offset.y;
}

void Fighter::set_opponent_offset(glm::vec2 offset) {
	glm::vec2 new_offset;
	int frames = fighter_accessor->fighter[!id]->fighter_int[FIGHTER_INT_MANUAL_POS_CHANGE_FRAMES];
	new_offset.x = this->pos.x + (offset.x * facing_dir);
	new_offset.y = this->pos.y + offset.y;

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_X] =
		(fighter_accessor->fighter[!id]->pos.x - new_offset.x) * fighter_accessor->fighter[!id]->facing_dir / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_CHANGE_Y] =
		(fighter_accessor->fighter[!id]->pos.y - new_offset.y) / (frames + 1);

	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_X] = new_offset.x;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_MANUAL_POS_OFFSET_Y] = new_offset.y;
}

void Fighter::change_opponent_status(unsigned int status_kind) {
	if ((this->status_kind == FIGHTER_STATUS_THROW || this->status_kind == FIGHTER_STATUS_THROW_AIR) && status_kind == FIGHTER_STATUS_THROWN) {
		fighter_flag[FIGHTER_FLAG_THREW_OPPONENT] = true;
	}
	fighter_accessor->fighter[!id]->change_status(status_kind);
}

void Fighter::damage_opponent(float damage, float facing_dir, float x_speed, float y_speed) {
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_HEALTH] -= damage;
	fighter_accessor->fighter[!id]->facing_dir = facing_dir * this->facing_dir * -1;
	fighter_accessor->fighter[!id]->facing_right = (facing_dir * this->facing_dir * -1) > 0;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_CURRENT_X_SPEED] = x_speed * this->facing_dir;
	fighter_accessor->fighter[!id]->fighter_float[FIGHTER_FLOAT_CURRENT_Y_SPEED] = y_speed;
}

void Fighter::set_opponent_rot(glm::vec3 rot) {
	rot.x *= facing_dir;
	fighter_accessor->fighter[!id]->set_rot(rot);
}

void Fighter::add_opponent_rot(glm::vec3 rot) {
	rot.x *= facing_dir;
	fighter_accessor->fighter[!id]->add_rot(rot);
}

void Fighter::reset_opponent_rot() {
	fighter_accessor->fighter[!id]->reset_rot();
}

void Fighter::set_opponent_thrown_ticks() {
	if (fighter_accessor->fighter[!id]->anim_kind == nullptr) {
		return;
	}
	fighter_accessor->fighter[!id]->rate = ((anim_kind->length / this->rate) / fighter_accessor->fighter[!id]->anim_kind->length);
}

void Fighter::change_opponent_anim(std::string anim_kind, float frame_rate, float entry_frame) {
	fighter_accessor->fighter[!id]->change_anim(anim_kind, frame_rate, entry_frame);
}

void Fighter::attach_opponent(std::string bone_name) {
	Fighter* that = fighter_accessor->fighter[!id];
	int index = model.get_bone_id(bone_name);
	if (index == -1) {
		return;
	}
	glm::vec3 final_rot = rot;
	final_rot.z += glm::radians(90.0 * facing_dir);
	final_rot += extra_rot;
	that->extra_mat = inverse(glm::scale(model.bones[index].final_matrix, glm::vec3(facing_dir, 1.0, 1.0)) * orientate4(final_rot));
	glm::vec2 to_add = get_rotated_distance_to_bone(index);
	that->set_pos(pos + glm::vec3(to_add, 0.0));
}

void Fighter::detach_opponent() {
	fighter_accessor->fighter[!id]->extra_mat = glm::mat4(1.0);
}