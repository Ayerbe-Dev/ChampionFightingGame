#include "Fighter.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/matrix_decompose.hpp>

glm::vec3 Fighter::get_distance_to_bone(std::string bone_name) {
	int index = model.get_bone_id(bone_name);
	if (index != -1) {
		glm::vec3 ret = (glm::vec3)(model.bones[index].final_matrix)[3];
		ret.x *= facing_dir;
		return ret;
	}
	return glm::vec3(0.0);
}

glm::vec3 Fighter::get_distance_to_bone(int bone_id) {
	if (bone_id != -1) {
		glm::vec3 ret = (glm::vec3)(model.bones[bone_id].final_matrix)[3];
		ret.x *= facing_dir;
		return ret;
	}
	return glm::vec3(0.0);
}

glm::vec3 Fighter::get_bone_rotation(std::string bone_name) {
	int index = model.get_bone_id(bone_name);
	if (index != -1) {
		glm::quat q = quat_cast(model.bones[index].anim_matrix);
		return eulerAngles(q);
	}
	return glm::vec3(0.0);
}

glm::vec3 Fighter::get_bone_rotation(int bone_id) {
	if (bone_id != -1) {
		glm::quat q = quat_cast(model.bones[bone_id].anim_matrix);
		return eulerAngles(q);
	}
	return glm::vec3(0.0);
}

glm::vec3 Fighter::get_rotated_distance_to_bone(std::string bone_name) {
	int index = model.get_bone_id(bone_name);
	glm::vec3 distance = get_distance_to_bone(index);
	glm::vec3 rot = get_bone_rotation(index);
	distance = rotate(distance, rot.x, glm::vec3(1.0, 0.0, 0.0));
	distance = rotate(distance, rot.y, glm::vec3(0.0, 1.0, 0.0));
	distance = rotate(distance, rot.z, glm::vec3(0.0, 0.0, 1.0));
	distance.x *= facing_dir;
	return distance;
}

glm::vec3 Fighter::get_rotated_distance_to_bone(int bone_id) {
	glm::vec3 distance = get_distance_to_bone(bone_id);
	glm::vec3 rot = get_bone_rotation(bone_id);
	distance = rotate(distance, rot.x, glm::vec3(1.0, 0.0, 0.0));
	distance = rotate(distance, rot.y, glm::vec3(0.0, 1.0, 0.0));
	distance = rotate(distance, rot.z, glm::vec3(0.0, 0.0, 1.0));
	distance.x *= facing_dir;
	return distance;
}