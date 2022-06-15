#include "Camera.h"
#include "Fighter.h"
#include "Stage.h"
#include "utils.h"
#include <glm/gtx/vector_angle.hpp>
#include "RenderManager.h"

Camera::Camera() {
	base_pos = glm::vec3(0.0, 0.6, 2.7);
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
	auto_linear_scale = 3.0;
	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));
	projection_matrix = glm::mat4(1.0);
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
	if (anim_kind != nullptr) {
		follow_anim();
	}
	else if (following_players) {
		follow_players();
	}
}

void Camera::load_camera_anim(std::string anim_kind, std::string anim_dir) {
	if (camera_anim_map.find(anim_kind) != camera_anim_map.end()) {
		return;
	}
	camera_anim_map[anim_kind] = camera_anims.size();
	camera_anims.push_back(CameraAnim(anim_kind, anim_dir));
}

void Camera::play_camera_anim(int follow_id, std::string anim_kind, float rate, float frame) {
	if (camera_anim_map.find(anim_kind) == camera_anim_map.end()) {
		return;
	}

	this->follow_id = follow_id;
	following_players = false;
	this->frame = frame;
	this->rate = rate;
	this->anim_kind = &camera_anims[camera_anim_map[anim_kind]];
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
	this->fov = fov;
	projection_matrix = glm::perspective(glm::radians(fov), (float)WINDOW_W_FACTOR, 0.1f, 100.0f);
	update_view();
}

void Camera::update_view() {
	glm::vec3 new_front;
	new_front.x = cos(glm::radians(yaw - 90)) * cos(glm::radians(pitch));
	new_front.y = sin(glm::radians(pitch));
	new_front.z = sin(glm::radians(yaw - 90)) * cos(glm::radians(pitch));
	front = normalize(new_front);

	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));

	glm::mat4 roll_mat = glm::rotate(glm::mat4(1.0f), glm::radians(roll), front);
	up = glm::mat3(roll_mat) * up;

	camera_matrix = projection_matrix * lookAt(pos, pos + front, up);
	RenderManager::get_instance()->update_shader_cams();
}

void Camera::follow_players() {
	pos.x = clampf(stage->camera_bounds.x, (fighter[0]->pos.x + fighter[1]->pos.x) / 2.0, stage->camera_bounds.y) / 400.0;
	pos.y = clampf(base_pos.y, ((std::max(fighter[0]->pos.y, fighter[1]->pos.y)) / 200.0) - (base_pos.y / 4.0), 800.0);
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
	pos = anim_kind->keyframes[frame].pos_key;

	yaw = anim_kind->keyframes[frame].rot_key.x;
	pitch = anim_kind->keyframes[frame].rot_key.y * 180;
	roll = anim_kind->keyframes[frame].rot_key.z;

	pos.x *= fighter[follow_id]->facing_dir;
	yaw *= fighter[follow_id]->facing_dir;

	pos = glm::normalize(pos);
	pos += fighter[follow_id]->pos / glm::vec3 (
		WINDOW_WIDTH / (100 * fighter[follow_id]->scale.x),
		WINDOW_HEIGHT / (100 * fighter[follow_id]->scale.y),
		WINDOW_DEPTH / (100 * fighter[follow_id]->scale.z)
	);

	prev_pos = pos;

	update_view();
	frame += rate;
	if (frame >= anim_kind->length) {
		fighter[follow_id]->stop_camera_anim();
	}
}