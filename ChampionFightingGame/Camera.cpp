#include "Camera.h"
#include "Fighter.h"
#include "Stage.h"
#include "WindowConstants.h"
#include <glm/gtx/vector_angle.hpp>
#include "RenderManager.h"
#include "OpenAL/al.h"
#include "GLM Helpers.h"
#include "utils.h"

Camera::Camera() {
	base_pos = glm::vec3(0.0, 8.0, 55.0);
	pos_target.set_persistence(true);
	pos = base_pos;
	prev_pos = glm::vec3(0.0);
	flip_matrix = glm::mat4(
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	fov = 45.0;
	yaw = 0.0;
	pitch = 3.0;
	roll = 0.0;
	following_players = true;
	anim_end = false;
	camera_locked = false;
	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));
	projection_matrix = glm::perspective(glm::radians(fov), (float)WINDOW_FACTOR, 0.1f, 1000.0f);
	view_matrix = glm::mat4(1.0);
	camera_matrix = glm::mat4(1.0);
	anim_kind = nullptr;
	for (int i = 0; i < 2; i++) {
		fighter[i] = nullptr;
	}
	stage = nullptr;
	follow_id = -1;
	frame = 0.0;
	rate = 1.0;
}

void Camera::camera_main() {
	if (camera_locked) return;
	if (anim_kind != nullptr) {
		follow_anim();
	}
	else if (following_players) {
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

void Camera::play_camera_anim(int follow_id, std::string anim_kind, float rate, float frame) {
	if (!camera_anim_map.contains(anim_kind)) {
		return;
	}

	this->follow_id = follow_id;
	following_players = false;
	anim_end = false;
	this->frame = frame;
	this->rate = rate;
	this->anim_kind = &camera_anims[camera_anim_map[anim_kind]];
}

void Camera::play_camera_anim(int follow_id, CameraAnim* anim_kind, float rate, float frame) {
	this->follow_id = follow_id;
	following_players = false;
	anim_end = false;
	this->frame = frame;
	this->rate = rate;
	this->anim_kind = anim_kind;
}

void Camera::unload_camera_anims() {
	camera_anims.clear();
	camera_anim_map.clear();
}

void Camera::add_pos(float x, float y, float z, float speed) {
	if (speed == 0.0) {
		speed = cam_speed;
	}
	pos += front * z * speed;
	pos += glm::normalize(glm::cross(front, up)) * x * speed;

	pos.y += y * speed;
	update_view();
}

void Camera::adjust_view(float x, float y, float z, float speed) {
	if (speed == 0.0) {
		speed = cam_sens;
	}

	yaw += x * speed;
	pitch += y * speed;
	roll += z * speed;
	update_view();
}

void Camera::set_fov(float fov) {
	RenderManager* render_manager = RenderManager::get_instance();
	this->fov = fov;
	projection_matrix = glm::perspective(glm::radians(fov), (float)WINDOW_FACTOR, 0.1f, 1000.0f);
	update_view();
}

void Camera::reset_camera() {
	pos_target = base_pos.x;
	pos = base_pos;
	prev_pos = base_pos;
	set_fov(45.0);
	yaw = 0.0;
	pitch = 3.0;
	roll = 0.0;
	following_players = true;
	anim_kind = nullptr;
	anim_end = false;
	camera_locked = false;
	follow_id = -1;
	frame = 0.0;
	rate = 1.0;
	view_matrix = glm::mat4(1.0);
	camera_matrix = glm::mat4(1.0);
	update_view();
}

void Camera::update_view() {
	glm::vec3 new_front;
	new_front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	new_front.y = sin(glm::radians(pitch));
	new_front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = normalize(new_front);

	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));

	glm::mat4 roll_mat = glm::rotate(glm::mat4(1.0f), glm::radians(roll), front);
	up = glm::mat3(roll_mat) * up;

	view_matrix = lookAt(pos, pos + front, up);
	camera_matrix = projection_matrix * view_matrix;
	RenderManager::get_instance()->update_shader_cams();
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

void Camera::follow_players() {
	//NOTE: Fighter scale is effectively a constant value of 1.0 rn. If changing the scale of a character breaks the
	//camera only when they move, x_scaler may actually just be a constant value of WINDOW_WIDTH / 100.0.

	//For now this will be accurate though, and shouldn't be too hard to adjust if it turns out to be wrong.
	float x_scaler = WINDOW_WIDTH / (100 * std::max(fighter[0]->scale.x, fighter[1]->scale.x));
	float y_scaler = WINDOW_HEIGHT / (100 * std::max(fighter[0]->scale.x, fighter[1]->scale.x));

	pos_target.set_target_val(clampf(stage->camera_bounds.x, (fighter[0]->pos.x + fighter[1]->pos.x) / 2.0f, stage->camera_bounds.y) / x_scaler, 10);
	pos.x = pos_target;
	pos.y = clampf(base_pos.y, std::max(fighter[0]->pos.y, fighter[1]->pos.y) / y_scaler, 100.0f);
	pos.z = base_pos.z;

	yaw = 0.0;
	pitch = 3.0;
	roll = 0.0;
	fov = 45.0;

	if (prev_pos != pos) {
		update_view();
	}
	prev_pos = pos;
}

void Camera::follow_anim() {
	CameraKeyframe keyframe = anim_kind->keyframes[clamp(0, floorf(frame), anim_kind->keyframes.size() - 1)];
	CameraKeyframe next_keyframe = anim_kind->keyframes[clamp(0, floorf(frame + 1.0f), anim_kind->keyframes.size() - 1)];

	keyframe.pos_key += (frame - (int)frame) * (next_keyframe.pos_key - keyframe.pos_key);
	keyframe.rot_key += (frame - (int)frame) * (next_keyframe.rot_key - keyframe.rot_key);

	pos = keyframe.pos_key;
	yaw = keyframe.rot_key.x;
	pitch = keyframe.rot_key.y;
	roll = keyframe.rot_key.z;

	if (follow_id != -1) {
		pos.x *= fighter[follow_id]->facing_dir;
		yaw *= fighter[follow_id]->facing_dir;

		RenderManager* render_manager = RenderManager::get_instance();

		pos += fighter[follow_id]->pos / glm::vec3(
			WINDOW_WIDTH / (100.0f * fighter[follow_id]->scale.x),
			WINDOW_HEIGHT / (100.0f * fighter[follow_id]->scale.y),
			WINDOW_DEPTH / (100.0f * fighter[follow_id]->scale.z)
		);
	}

	prev_pos = pos;

	update_view();
	frame += rate;
	if (frame >= anim_kind->length) {
		if (follow_id != -1) {
			fighter[follow_id]->stop_camera_anim();
		}
		else {
			frame = 0.0;
		}
		anim_end = true;
	}
	else if (frame < 0.0) {
		if (follow_id != -1) {
			fighter[follow_id]->stop_camera_anim();
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