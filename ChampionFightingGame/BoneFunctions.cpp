#include "BattleObject.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include "GLM Helpers.h"

glm::vec3 BattleObject::get_relative_bone_position(std::string bone_name, glm::vec3 offset) {
	if (!has_model) {
		return offset;
	}
	int bone_id = model.get_bone_id(bone_name);
	if (bone_id != -1) {
		Bone& target = model.bones[bone_id];
		glm::vec3 ret = target.pos;
		return ret + offset;
	}
	return offset;
}

glm::vec3 BattleObject::get_relative_bone_position(int bone_id, glm::vec3 offset) {
	if (!has_model) {
		return offset;
	}
	Bone& target = model.bones[bone_id];
	glm::vec3 ret = target.pos;
	return ret + offset;
}

glm::vec3 BattleObject::get_bone_position(std::string bone_name, glm::vec3 offset) {
	if (!has_model) {
		return offset;
	}
	int bone_id = model.get_bone_id(bone_name);
	if (bone_id != -1) {
		Bone& target = model.bones[bone_id];
		glm::vec3 ret = target.pos;
		return ret + pos + offset;
	}
	return offset;
}

glm::vec3 BattleObject::get_bone_position(int bone_id, glm::vec3 offset) {
	if (!has_model) {
		return offset;
	}
	Bone& target = model.bones[bone_id];
	glm::vec3 ret = target.pos;
	
	print_vec(std::cout, ret + offset);

	return ret + pos + offset;
}

glm::quat BattleObject::get_bone_rotation_quat(std::string bone_name) {
	if (!has_model) {
		return glm::quat();
	}
	int bone_id = model.get_bone_id(bone_name);
	if (bone_id != -1) {
		glm::quat ret = model.bones[bone_id].rot;
		return ret;
	}
	return glm::quat();
}

glm::quat BattleObject::get_bone_rotation_quat(int bone_id) {
	if (!has_model) {
		return glm::quat();
	}
	glm::quat ret = model.bones[bone_id].rot;
	return ret;
}

glm::vec3 BattleObject::get_bone_rotation(std::string bone_name) {
	return lazy_q2v(get_bone_rotation_quat(bone_name));
}

glm::vec3 BattleObject::get_bone_rotation(int bone_id) {
	return lazy_q2v(get_bone_rotation_quat(bone_id)) * glm::vec3(180/3.14);
}

//Disclaimer: The below 4 functions are all unused and don't work. Turns out just
//getting the position raw already accounts for rotation, but they might still come in
//handy idk
glm::vec3 BattleObject::get_rotated_bone_position(std::string bone_name, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	int bone_id = model.get_bone_id(bone_name);
	if (bone_id != -1) {
		glm::vec3 pos = get_relative_bone_position(bone_id, offset);
		glm::quat rot = get_bone_rotation(bone_id);
		pos = rotate(pos, rot);
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
	glm::quat rot = get_bone_rotation(bone_id);
	pos = rotate(pos, rot);
	pos.x *= facing_dir;
	pos += this->pos;
	return pos;
}

glm::vec3 BattleObject::get_rotated_relative_bone_position(std::string bone_name, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	int bone_id = model.get_bone_id(bone_name);
	if (bone_id != -1) {
		glm::vec3 pos = get_relative_bone_position(bone_id, offset);
		glm::quat rot = get_bone_rotation(bone_id);
		pos = rotate(pos, rot);
		return pos;
	}
	return glm::vec3(0.0);
}

glm::vec3 BattleObject::get_rotated_relative_bone_position(int bone_id, glm::vec3 offset) {
	if (!has_model) {
		return glm::vec3(0.0);
	}
	glm::vec3 pos = get_relative_bone_position(bone_id, offset);
	glm::quat rot = get_bone_rotation(bone_id);
	pos = rotate(pos, rot);
	return pos;
}