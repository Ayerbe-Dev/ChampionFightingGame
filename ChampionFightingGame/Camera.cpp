#include "Camera.h"
#include "Fighter.h"
#include "Stage.h"
#include "WindowConstants.h"
#include <glm/gtx/vector_angle.hpp>
#include "WindowManager.h"
#include "OpenAL/al.h"
#include "GLM Helpers.h"
#include "utils.h"

Camera::Camera() {
	pos = base_pos;
	prev_pos = glm::vec3(0.0);
	pos_x_interpolator = base_pos.x;

	yaw = 0.0;
	pitch = 3.0;
	roll = 0.0;
	fov = 45.0;

	front = normalize(glm::vec3(
		sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		-cos(glm::radians(yaw)) * cos(glm::radians(pitch))
	));

	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));
	aim = pos + front;

	projection_matrix = glm::perspective(glm::radians(fov), WINDOW_FACTOR, 0.1f, 1000.0f);
	view_matrix = glm::mat4(1.0);
	camera_matrix = glm::mat4(1.0);

	for (int i = 0; i < 2; i++) {
		fighter[i] = nullptr;
	}
	stage = nullptr;

	anim_kind = nullptr;
	frame = 0.0f;
	rate = 1.0f;

	target = nullptr;
	target_base_pos = glm::vec3(0.0f);
	target_facing_dir = 1.0f;
	target_end_cinematic_on_anim_end = false;

	camera_locked = false;
	anim_end = false;
}

void Camera::camera_main() {
	if (camera_locked) return;
	if (anim_kind != nullptr) {
		follow_anim();
	}
	else {
		follow_players();
	}
}

void Camera::load_camera_anim(std::string anim_kind, std::string anim_dir) {
	if (camera_anim_map.contains(anim_kind)) {
		return;
	}
	camera_anim_map[anim_kind] = camera_anims.size();
	camera_anims.push_back(CameraAnim(anim_kind, anim_dir));
}

void Camera::play_camera_anim(std::string anim_kind, float rate, float frame) {
	if (!camera_anim_map.contains(anim_kind)) {
		return;
	}

	this->anim_end = false;
	this->frame = frame;
	this->rate = rate;
	this->anim_kind = &camera_anims[camera_anim_map[anim_kind]];

	this->target = nullptr;
	this->target_base_pos = glm::vec3(0.0f);
	this->target_facing_dir = 1.0f;
	this->target_end_cinematic_on_anim_end = false;
}

void Camera::play_camera_anim(CameraAnim* anim_kind, float rate, float frame) {
	this->anim_end = false;
	this->frame = frame;
	this->rate = rate;
	this->anim_kind = anim_kind;

	this->target = nullptr;
	this->target_base_pos = glm::vec3(0.0f);
	this->target_facing_dir = 1.0f;
	this->target_end_cinematic_on_anim_end = false;
}

void Camera::play_camera_anim(GameObject* target, std::string anim_kind, float rate, float frame, bool end_cinematic_on_anim_end) {
	if (!camera_anim_map.contains(anim_kind)) {
		return;
	}

	this->anim_end = false;
	this->frame = frame;
	this->rate = rate;
	this->anim_kind = &camera_anims[camera_anim_map[anim_kind]];

	this->target = target;
	this->target_base_pos = target->get_scaled_pos();
	this->target_facing_dir = 1.0f;
	this->target_end_cinematic_on_anim_end = end_cinematic_on_anim_end;
}

void Camera::play_camera_anim(GameObject* target, CameraAnim* anim_kind, float rate, float frame, bool end_cinematic_on_anim_end) {
	this->anim_end = false;
	this->frame = frame;
	this->rate = rate;
	this->anim_kind = anim_kind;

	this->target = target;
	this->target_base_pos = target->get_scaled_pos();
	this->target_facing_dir = 1.0f;
	this->target_end_cinematic_on_anim_end = end_cinematic_on_anim_end;
}

void Camera::play_camera_anim(BattleObject* target, std::string anim_kind, float rate, float frame, bool end_cinematic_on_anim_end) {
	if (!camera_anim_map.contains(anim_kind)) {
		return;
	}

	this->anim_end = false;
	this->frame = frame;
	this->rate = rate;
	this->anim_kind = &camera_anims[camera_anim_map[anim_kind]];

	this->target = target;
	this->target_base_pos = target->get_scaled_pos();
	this->target_facing_dir = target->facing_dir;
	this->target_end_cinematic_on_anim_end = end_cinematic_on_anim_end;
}

void Camera::play_camera_anim(BattleObject* target, CameraAnim* anim_kind, float rate, float frame, bool end_cinematic_on_anim_end) {
	this->anim_end = false;
	this->frame = frame;
	this->rate = rate;
	this->anim_kind = anim_kind;

	this->target = target;
	this->target_base_pos = target->get_scaled_pos();
	this->target_facing_dir = target->facing_dir;
	this->target_end_cinematic_on_anim_end = end_cinematic_on_anim_end;
}

void Camera::stop_camera_anim() {
	target = nullptr;
	target_base_pos = glm::vec3(0.0f);
	target_facing_dir = 1.0f;
	target_end_cinematic_on_anim_end = false;
	anim_kind = nullptr;
	rate = 1.0f;
	frame = 0.0f;
}

void Camera::unload_camera_anims() {
	camera_anims.clear();
	camera_anim_map.clear();
}

void Camera::set_pos(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;
	calc_aim_from_ypr();
	update_view();
}

void Camera::set_rot(float x, float y, float z) {
	yaw = x;
	pitch = y;
	roll = z;
	calc_aim_from_ypr();
	update_view();
}

void Camera::add_pos(float x, float y, float z) {
	pos += front * z;
	pos += glm::normalize(glm::cross(front, up)) * x;

	pos.y += y;
	calc_aim_from_ypr();
	update_view();
}

void Camera::add_rot(float x, float y, float z) {
	yaw += x;
	pitch += y;
	roll += z;
	calc_aim_from_ypr();
	update_view();
}

void Camera::set_fov(float fov) {
	this->fov = fov;
	projection_matrix = glm::perspective(glm::radians(fov), WINDOW_FACTOR, 0.1f, 1000.0f);
	update_view();
}

void Camera::reset_camera() {
	pos = base_pos;
	prev_pos = base_pos;
	pos_x_interpolator = base_pos.x;
	pos_x_interpolator.set_pause(false);

	yaw = 0.0;
	pitch = 3.0;
	roll = 0.0;
	fov = 45.0;

	projection_matrix = glm::perspective(glm::radians(fov), WINDOW_FACTOR, 0.1f, 1000.0f);
	view_matrix = glm::mat4(1.0);
	camera_matrix = glm::mat4(1.0);

	anim_kind = nullptr;
	frame = 0.0;
	rate = 1.0;

	camera_locked = false;
	anim_end = false;
	calc_aim_from_ypr();
	update_view();
}

void Camera::calc_aim_from_ypr() {
	front = normalize(glm::vec3(
		sin(glm::radians(yaw)) * cos(glm::radians(pitch)),
		sin(glm::radians(pitch)),
		-cos(glm::radians(yaw)) * cos(glm::radians(pitch))
	));

	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));

	if (pitch < -90.0f || pitch > 90.0f) {
		up = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(roll + 180.0f), front)) * up;
	}
	else {
		up = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(roll), front)) * up;
	}
	aim = pos + front;
}

void Camera::calc_ypr_from_aim() {
	front = normalize(aim - pos);
	pitch = glm::degrees(glm::asin(front.y));
	yaw = glm::degrees(glm::asin(front.x / glm::cos(glm::asin(front.y))));
}

void Camera::update_view() {
	view_matrix = lookAt(pos, aim, up);
	camera_matrix = projection_matrix * view_matrix;
	WindowManager::get_instance()->update_shader_cams();
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

void Camera::follow_players() {
	pos_x_interpolator.set_target_val(clampf(-stage->camera_bound, (fighter[0]->get_scaled_pos().x + fighter[1]->get_scaled_pos().x) / 2.0f, stage->camera_bound), 10);
	pos.x = pos_x_interpolator;
	pos.y = clampf(base_pos.y, std::max(fighter[0]->get_scaled_pos().y, fighter[1]->get_scaled_pos().y), 100.0f);
	pos.z = base_pos.z;

	yaw = 0.0;
	pitch = 3.0;
	roll = 0.0;
	fov = 45.0;

	if (prev_pos != pos) {
		calc_aim_from_ypr();
		update_view();
	}
	prev_pos = pos;
}

void Camera::follow_anim() {
	CameraKeyframe keyframe = anim_kind->keyframes[clamp(0, floorf(frame), anim_kind->keyframes.size() - 1)];
	CameraKeyframe next_keyframe = anim_kind->keyframes[clamp(0, floorf(frame + 1.0f), anim_kind->keyframes.size() - 1)];

	keyframe.pos_key += (frame - (int)frame) * (next_keyframe.pos_key - keyframe.pos_key);
	keyframe.rot_key += (frame - (int)frame) * (next_keyframe.rot_key - keyframe.rot_key);

	switch (anim_kind->anim_mode) {
	case (CAMERA_ANIM_MODE_STAGE): {
		pos = keyframe.pos_key;
		yaw = keyframe.rot_key.x;
		pitch = keyframe.rot_key.y;
		roll = keyframe.rot_key.z;

		pos.x *= target_facing_dir;
		yaw *= target_facing_dir;
		pos += target_base_pos;

		calc_aim_from_ypr();
	} break;
	case (CAMERA_ANIM_MODE_FIGHTER): {
		pos = keyframe.pos_key;
		aim = keyframe.rot_key;

		pos.x *= target_facing_dir;
		aim.x *= target_facing_dir;
		pos += target_base_pos;
		aim += target_base_pos;

		calc_ypr_from_aim();
	} break;
	default: {

	} break;
	}
	update_view();

	prev_pos = pos;
	frame += rate;
	if (frame >= anim_kind->length) {
		if (target != nullptr) {
			if (target_end_cinematic_on_anim_end) {
				target->stop_cinematic_sequence();
			}
			stop_camera_anim();
		}
		else {
			frame = 0.0;
		}
		anim_end = true;
	}
	else if (frame < 0.0) {
		if (target != nullptr) {
			if (target_end_cinematic_on_anim_end) {
				target->stop_cinematic_sequence();
			}
			stop_camera_anim();
		}
		else {
			frame = anim_kind->length - 1;
		}
		anim_end = true;
	}
	else {
		anim_end = false;
	}
}

std::string Camera::get_anim_name() {
	if (anim_kind == nullptr) {
		return "None";
	}
	else {
		return anim_kind->name;
	}
}