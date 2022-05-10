#include "BattleObject.h"
#include "EffectManager.h"

void BattleObject::new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, int bone_id, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	pos.x *= facing_dir;
	pos_frame.x *= facing_dir;
	if (!facing_right) {
		rot.x += 180.0;
	}
	effect_manager->activate_effect(id, name, pos, rot, scale, rgba, this, bone_id, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void BattleObject::new_effect_detached(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	pos.x *= facing_dir;
	pos_frame.x *= facing_dir;
	pos += this->pos;
	if (!facing_right) {
		rot.x += 180.0;
	}
	effect_manager->activate_effect(id, name, pos, rot, scale, rgba, nullptr, -1, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void BattleObject::clear_effect(std::string name, int instance) {
	effect_manager->clear_effect(id, name, instance);
}

void BattleObject::clear_effect_all() {
	effect_manager->clear_effect_all(id);
}