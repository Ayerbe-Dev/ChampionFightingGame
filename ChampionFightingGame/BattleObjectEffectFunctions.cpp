#include "BattleObject.h"
#include "EffectManager.h"

void BattleObject::new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	pos.x *= facing_dir;
	pos_frame.x *= facing_dir;
	if (!facing_right) {
		rot.y += 180.0;
	}
	effect_manager->activate_effect(id, name, pos, rot, scale, rgba, this, bone_id, bone_offset, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void BattleObject::new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, std::string bone_name, glm::vec3 bone_offset, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	new_effect(name, pos, rot, scale, rgba, model.get_bone_id(bone_name, false), bone_offset, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void BattleObject::new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	pos.x *= facing_dir;
	pos_frame.x *= facing_dir;
	if (!facing_right) {
		scale.x *= -1.0;
	}
	effect_manager->activate_effect(id, name, pos, rot, scale, rgba, this, -1, glm::vec3(0.0), pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void BattleObject::new_effect_no_follow(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	pos.x *= facing_dir;
	pos_frame.x *= facing_dir;
	pos += this->pos;
	if (!facing_right) {
		scale.x *= -1.0;
	}
	effect_manager->activate_effect(id, name, pos, rot, scale, rgba, (GameObject*)nullptr, -1, glm::vec3(0.0), pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void BattleObject::clear_effect(std::string name, int instance) {
	effect_manager->clear_effect(id, name, instance);
}

void BattleObject::clear_effect_all() {
	effect_manager->clear_effect_all(id);
}

void BattleObject::load_effect(std::string effect) {
	effect_manager->load_effect(effect);
}

void BattleObject::unload_effect(std::string effect) {
	effect_manager->unload_effect(effect);
}

void BattleObject::unload_all_effects() {
	effect_manager->unload_all_effects();
}