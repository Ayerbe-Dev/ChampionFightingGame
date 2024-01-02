#include "BattleObject.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "GLM Helpers.h"

glm::vec3 BattleObject::get_relative_bone_position(std::string bone_name, glm::vec3 offset) {
	if (has_model) {
		int bone_id = model.get_bone_id(bone_name);
		if (bone_id != -1) {
			offset += model.bone_data[bone_id].pos * scale_vec;
		}
	}
	offset.x *= facing_dir;
	return offset;
}

glm::vec3 BattleObject::get_relative_bone_position(int bone_id, glm::vec3 offset) {
	if (has_model && bone_id != -1) {
		offset += model.bone_data[bone_id].pos * scale_vec;
	}
	offset.x *= facing_dir;
	return offset;
}

glm::vec3 BattleObject::get_bone_position(std::string bone_name, glm::vec3 offset) {
	if (has_model) {
		int bone_id = model.get_bone_id(bone_name);
		if (bone_id != -1) {
			offset += model.bone_data[bone_id].pos * scale_vec;
			if (anim_kind != nullptr && anim_kind->flag_move) {
				offset -= GameObject::get_relative_bone_position("Trans"); //GameObject's version of
				//this function doesn't include an object's facing direction so we use that one here
			}
		}
	}
	offset.x *= facing_dir;
	return offset + pos;
}

glm::vec3 BattleObject::get_bone_position(int bone_id, glm::vec3 offset) {
	if (has_model && bone_id != -1) {
		offset += model.bone_data[bone_id].pos * scale_vec;
		if (anim_kind != nullptr && anim_kind->flag_move) {
			offset -= GameObject::get_relative_bone_position("Trans");
		}
	}
	offset.x *= facing_dir;
	return offset + pos;
}

glm::quat BattleObject::get_bone_rotation_quat(std::string bone_name) {
	if (!has_model) {
		return glm::quat();
	}
	int bone_id = model.get_bone_id(bone_name);
	if (bone_id != -1) {
		glm::quat ret = model.bone_data[bone_id].rot;
		return ret;
	}
	return glm::quat();
}

glm::quat BattleObject::get_bone_rotation_quat(int bone_id) {
	if (!has_model) {
		return glm::quat();
	}
	glm::quat ret = model.bone_data[bone_id].rot;
	return ret;
}

glm::vec3 BattleObject::get_bone_rotation(std::string bone_name) {
	return lazy_q2v(get_bone_rotation_quat(bone_name)) * glm::vec3(180 / 3.14);
}

glm::vec3 BattleObject::get_bone_rotation(int bone_id) {
	return lazy_q2v(get_bone_rotation_quat(bone_id)) * glm::vec3(180 / 3.14);
}

glm::vec3 BattleObject::get_bone_angle(int base_id, int angle_id) {
	glm::vec3 base_bone = normalize(get_relative_bone_position(base_id));
	glm::vec3 angle_bone = normalize(get_relative_bone_position(angle_id));
	
	return calc_rotation(base_bone, angle_bone) * glm::vec3(180 / 3.14);
}