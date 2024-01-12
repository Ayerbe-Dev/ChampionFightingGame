#include <glm/gtx/string_cast.hpp>
#include "Fighter.h"
#include "utils.h"

void Fighter::change_opponent_status(unsigned int status_kind) {
	if ((this->status_kind == FIGHTER_STATUS_THROW || this->status_kind == FIGHTER_STATUS_THROW_AIR) && status_kind == FIGHTER_STATUS_THROWN) {
		fighter_flag[FIGHTER_FLAG_THREW_OPPONENT] = true;
	}
	object_manager->fighter[!id]->change_status(status_kind);
}

void Fighter::damage_opponent(float damage) {
	damage *= (clampf(1, 10 - fighter_int[FIGHTER_INT_DAMAGE_SCALE], 15)) / 10.0;
	Fighter* that = object_manager->fighter[!id];
	fighter_float[FIGHTER_FLOAT_COMBO_DAMAGE] += damage;
	fighter_int[FIGHTER_INT_COMBO_COUNT]++;
	that->fighter_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, that->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, that->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	that->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, that->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, that->fighter_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
}

void Fighter::set_opponent_rot(glm::vec3 rot) {
	rot.x *= facing_dir;
	object_manager->fighter[!id]->set_rot(rot);
}

void Fighter::add_opponent_rot(glm::vec3 rot) {
	rot.x *= facing_dir;
	object_manager->fighter[!id]->add_rot(rot);
}

void Fighter::reset_opponent_rot() {
	object_manager->fighter[!id]->reset_rot();
}

void Fighter::set_opponent_thrown_ticks() {
	if (anim_kind == nullptr || object_manager->fighter[!id]->anim_kind == nullptr) {
		return;
	}
	object_manager->fighter[!id]->rate = ((anim_kind->length / this->rate) / object_manager->fighter[!id]->anim_kind->length);
}

void Fighter::change_opponent_anim(std::string anim_kind, float frame_rate, float entry_frame) {
	Fighter* that = object_manager->fighter[!id];
	bool scale = false;
	if (frame_rate == -1.0) {
		scale = true;
		frame_rate = 1.0;
	}
	that->change_anim(anim_kind, frame_rate, entry_frame);
	if (scale && this->anim_kind != nullptr) {
		that->rate = (float)that->anim_kind->length / (((float)this->anim_kind->length - this->frame) / this->rate);
	}
}

void Fighter::grab_opponent(std::string attacker_bone_name, std::string defender_bone_name, glm::vec3 offset) {
	Fighter* that = object_manager->fighter[!id];
	int attacker_index = model.get_bone_id(attacker_bone_name);
	int defender_index = model.get_bone_id(defender_bone_name);
	if (attacker_index == -1 || defender_index == -1) {
		return;
	}
	that->fighter_int[FIGHTER_INT_GRAB_BONE_ID] = attacker_index;
	that->fighter_int[FIGHTER_INT_GRABBED_BONE_ID] = defender_index;
	that->fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_X] = offset.x * facing_dir;
	that->fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_Y] = offset.y;
	that->fighter_float[FIGHTER_FLOAT_GRAB_OFFSET_Z] = offset.z;
	that->fighter_flag[FIGHTER_FLAG_IN_THROW_TECH_WINDOW] = status_kind == FIGHTER_STATUS_GRAB || status_kind == FIGHTER_STATUS_GRAB_AIR;
}