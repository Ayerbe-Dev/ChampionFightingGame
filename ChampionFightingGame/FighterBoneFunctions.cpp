#include "Fighter.h"
#include <gtx/rotate_vector.hpp>
#include <gtx/matrix_decompose.hpp>

vec3 Fighter::get_distance_to_bone(string bone_name) {
	int index = model.get_bone_id(bone_name);
	if (index != -1) {
		vec3 ret = (vec3)(model.bones[index].final_matrix)[3];
		ret.x *= facing_dir;
		return ret;
	}
	return vec3(0.0);
}

vec3 Fighter::get_distance_to_bone(int bone_id) {
	if (bone_id != -1) {
		vec3 ret = (vec3)(model.bones[bone_id].final_matrix)[3];
		ret.x *= facing_dir;
		return ret;
	}
	return vec3(0.0);
}

vec3 Fighter::get_bone_rotation(string bone_name) {
	int index = model.get_bone_id(bone_name);
	if (index != -1) {
		quat q = quat_cast(model.bones[index].anim_matrix);
		return eulerAngles(q);
	}
	return vec3(0.0);
}

vec3 Fighter::get_bone_rotation(int bone_id) {
	if (bone_id != -1) {
		quat q = quat_cast(model.bones[bone_id].anim_matrix);
		return eulerAngles(q);
	}
	return vec3(0.0);
}

vec3 Fighter::get_rotated_distance_to_bone(string bone_name) {
	int index = model.get_bone_id(bone_name);
	vec3 distance = get_distance_to_bone(index);
	vec3 rot = get_bone_rotation(index);
	distance = rotate(distance, rot.x, vec3(1.0, 0.0, 0.0));
	distance = rotate(distance, rot.y, vec3(0.0, 1.0, 0.0));
	distance = rotate(distance, rot.z, vec3(0.0, 0.0, 1.0));
	distance.x *= facing_dir;
	return distance;
}

vec3 Fighter::get_rotated_distance_to_bone(int bone_id) {
	vec3 distance = get_distance_to_bone(bone_id);
	vec3 rot = get_bone_rotation(bone_id);
	distance = rotate(distance, rot.x, vec3(1.0, 0.0, 0.0));
	distance = rotate(distance, rot.y, vec3(0.0, 1.0, 0.0));
	distance = rotate(distance, rot.z, vec3(0.0, 0.0, 1.0));
	distance.x *= facing_dir;
	return distance;
}