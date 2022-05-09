#include "BattleObject.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>

glm::vec3 BattleObject::get_relative_bone_position(std::string bone_name, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	int index = model.get_bone_id(bone_name);
	if (index != -1) {
		Bone& target = model.bones[index];
		glm::vec3 ret = glm::vec3(
			target.anim_matrix[3].z * facing_dir,
			target.anim_matrix[3].y,
			target.anim_matrix[3].x * facing_dir * -1
		);
		return (ret / scale) + offset;
	}
	return glm::vec3(0.0);
}

glm::vec3 BattleObject::get_relative_bone_position(int bone_id, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	Bone& target = model.bones[bone_id];
	glm::vec3 ret = glm::vec3(
		target.anim_matrix[3].z * facing_dir,
		target.anim_matrix[3].y,
		target.anim_matrix[3].x * facing_dir * -1
	);
	return (ret / scale) + offset;
}

glm::vec3 BattleObject::get_bone_position(std::string bone_name, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	int index = model.get_bone_id(bone_name);
	if (index != -1) {
		glm::vec3 base_offset = get_relative_bone_position(index, offset);
		return pos + base_offset;
	}
	return glm::vec3(0.0);
}

glm::vec3 BattleObject::get_bone_position(int bone_id, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	glm::vec3 base_offset = get_relative_bone_position(bone_id, offset);
	return pos + base_offset;
}

glm::vec3 BattleObject::get_bone_rotation(std::string bone_name) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	int index = model.get_bone_id(bone_name);
	if (index != -1) {
		glm::quat q = quat_cast(model.bones[index].anim_matrix);
		return degrees(eulerAngles(q));
	}
	return glm::vec3(0.0);
}

glm::vec3 BattleObject::get_bone_rotation(int bone_id) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	glm::quat q = quat_cast(model.bones[bone_id].anim_matrix);
	return degrees(eulerAngles(q));
}

//Disclaimer: The below 4 functions are all unused and don't work. Turns out just
//getting the position raw already accounts for rotation, but they might still come in
//handy idk
glm::vec3 BattleObject::get_rotated_bone_position(std::string bone_name, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	int index = model.get_bone_id(bone_name);
	if (index != -1) {
		glm::vec3 pos = get_relative_bone_position(index, offset);
		glm::vec3 rot = get_bone_rotation(index);
		pos = rotate(pos, rot.x, glm::vec3(1.0, 0.0, 0.0));
		pos = rotate(pos, rot.y, glm::vec3(0.0, 1.0, 0.0));
		pos = rotate(pos, rot.z, glm::vec3(0.0, 0.0, 1.0));
		pos.x *= facing_dir;
		pos += this->pos;
		return pos;
	}
	return glm::vec3(0.0);
}

glm::vec3 BattleObject::get_rotated_bone_position(int bone_id, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	glm::vec3 pos = get_relative_bone_position(bone_id, offset);
	glm::vec3 rot = get_bone_rotation(bone_id);
	pos = rotate(pos, rot.x, glm::vec3(1.0, 0.0, 0.0));
	pos = rotate(pos, rot.y, glm::vec3(0.0, 1.0, 0.0));
	pos = rotate(pos, rot.z, glm::vec3(0.0, 0.0, 1.0));
	pos.x *= facing_dir;
	pos += this->pos;
	return pos;
}

glm::vec3 BattleObject::get_rotated_relative_bone_position(std::string bone_name, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	int index = model.get_bone_id(bone_name);
	if (index != -1) {
		glm::vec3 pos = get_relative_bone_position(index, offset);
		glm::vec3 rot = get_bone_rotation(index);
		pos = rotate(pos, rot.x, glm::vec3(1.0, 0.0, 0.0));
		pos = rotate(pos, rot.y, glm::vec3(0.0, 1.0, 0.0));
		pos = rotate(pos, rot.z, glm::vec3(0.0, 0.0, 1.0));
		return pos;
	}
	return glm::vec3(0.0);
}

glm::vec3 BattleObject::get_rotated_relative_bone_position(int bone_id, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	glm::vec3 pos = get_relative_bone_position(bone_id, offset);
	glm::vec3 rot = get_bone_rotation(bone_id);
	pos = rotate(pos, rot.x, glm::vec3(1.0, 0.0, 0.0));
	pos = rotate(pos, rot.y, glm::vec3(0.0, 1.0, 0.0));
	pos = rotate(pos, rot.z, glm::vec3(0.0, 0.0, 1.0));
	return pos;
}