#include <glm/gtx/string_cast.hpp>
#include "Fighter.h"
#include "utils.h"

void Fighter::change_opponent_status(unsigned int status_kind) {
	object_manager->fighter[!id]->change_status(status_kind);
}

void Fighter::damage_opponent(float damage) {
	Fighter* that = object_manager->fighter[!id];
	damage *= that->object_float[FIGHTER_FLOAT_DAMAGE_SCALE] / 100.0f;
	object_float[FIGHTER_FLOAT_COMBO_DAMAGE_UI_TRAINING] += damage;
	object_int[FIGHTER_INT_COMBO_COUNT]++;
	that->object_float[FIGHTER_FLOAT_HEALTH] = clampf(0.0, that->object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, that->object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
	that->object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] = clampf(0.0, that->object_float[FIGHTER_FLOAT_PARTIAL_HEALTH] - damage, that->object_float[FIGHTER_FLOAT_PARTIAL_HEALTH]);
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
	that->object_int[FIGHTER_INT_GRAB_BONE_ID] = attacker_index;
	that->object_int[FIGHTER_INT_GRABBED_BONE_ID] = defender_index;
	that->object_float[FIGHTER_FLOAT_GRAB_OFFSET_X] = offset.x * facing_dir;
	that->object_float[FIGHTER_FLOAT_GRAB_OFFSET_Y] = offset.y;
	that->object_float[FIGHTER_FLOAT_GRAB_OFFSET_Z] = offset.z;
	that->object_flag[FIGHTER_FLAG_IN_THROW_TECH_WINDOW] = status_kind == FIGHTER_STATUS_GRAB || status_kind == FIGHTER_STATUS_GRAB_AIR;
}