#include "GameObject.h"
#include "EffectManager.h"

void GameObject::new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {

	effect_manager->activate_effect(id_effect, name, pos, rot, scale, rgba, this, bone_id, bone_offset, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void GameObject::new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, std::string bone_name, glm::vec3 bone_offset, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	new_effect(name, pos, rot, scale, rgba, model.get_bone_id(bone_name, false), bone_offset, pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void GameObject::new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	effect_manager->activate_effect(id_effect, name, pos, rot, scale, rgba, this, -1, glm::vec3(0.0), pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void GameObject::new_effect_no_follow(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, glm::vec3 pos_frame, glm::vec3 rot_frame,
	glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate, float frame) {
	pos += this->pos;
	effect_manager->activate_effect(id_effect, name, pos, rot, scale, rgba, (GameObject*)nullptr, -1, glm::vec3(0.0), pos_frame, rot_frame, scale_frame, rgba_frame, rate, frame);
}

void GameObject::clear_effect(std::string name, int instance) {
	effect_manager->clear_effect(id_effect, name, instance);
}

void GameObject::clear_effect_all() {
	effect_manager->clear_effect_all(id_effect);
}

void GameObject::load_effect(std::string effect) {
	effect_manager->load_effect(effect);
}

void GameObject::unload_effect(std::string effect) {
	effect_manager->unload_effect(effect);
}

void GameObject::unload_all_effects() {
	effect_manager->unload_all_effects();
}