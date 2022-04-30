#include "Fighter.h"
#include <glm/gtx/string_cast.hpp>

void Fighter::change_opponent_status(unsigned int status_kind) {
	if ((this->status_kind == FIGHTER_STATUS_THROW || this->status_kind == FIGHTER_STATUS_THROW_AIR) && status_kind == FIGHTER_STATUS_THROWN) {
		fighter_flag[FIGHTER_FLAG_THREW_OPPONENT] = true;
	}
	battle_object_manager->fighter[!id]->change_status(status_kind);
}

void Fighter::damage_opponent(float damage) {
	battle_object_manager->fighter[!id]->fighter_float[FIGHTER_FLOAT_HEALTH] -= damage;
}

void Fighter::set_opponent_rot(glm::vec3 rot) {
	rot.x *= facing_dir;
	battle_object_manager->fighter[!id]->set_rot(rot);
}

void Fighter::add_opponent_rot(glm::vec3 rot) {
	rot.x *= facing_dir;
	battle_object_manager->fighter[!id]->add_rot(rot);
}

void Fighter::reset_opponent_rot() {
	battle_object_manager->fighter[!id]->reset_rot();
}

void Fighter::set_opponent_thrown_ticks() {
	if (battle_object_manager->fighter[!id]->anim_kind == nullptr) {
		return;
	}
	battle_object_manager->fighter[!id]->rate = ((anim_kind->length / this->rate) / battle_object_manager->fighter[!id]->anim_kind->length);
}

void Fighter::change_opponent_anim(std::string anim_kind, float frame_rate, float entry_frame) {
	battle_object_manager->fighter[!id]->change_anim(anim_kind, frame_rate, entry_frame);
}

void Fighter::attach_opponent(std::string bone_name) {
	Fighter* that = battle_object_manager->fighter[!id];
	int index = model.get_bone_id(bone_name);
	if (index == -1) {
		return;
	}
	that->fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION] = true;
	that->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = true;
	glm::vec3 final_rot = rot;
	final_rot.z += glm::radians(90.0 * facing_dir);
	final_rot += extra_rot;
	that->extra_mat = inverse(glm::scale(model.bones[index].final_matrix, glm::vec3(facing_dir, 1.0, 1.0)) * orientate4(final_rot));
//	glm::vec2 to_add = get_rotated_distance_to_bone(index);
//	that->set_pos(pos + glm::vec3(to_add, 0.0));
}

void Fighter::detach_opponent() {
	Fighter* that = battle_object_manager->fighter[!id];
	that->fighter_flag[FIGHTER_FLAG_LOCK_DIRECTION] = false;
	that->fighter_flag[FIGHTER_FLAG_ALLOW_GROUND_CROSSUP] = false;
	that->extra_mat = glm::mat4(1.0);
}