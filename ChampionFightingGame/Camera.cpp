#include "Camera.h"
#include "Stage.h"
#include "utils.h"
#include <glm/gtx/vector_angle.hpp>
#include "RenderManager.h"

Camera::Camera() {
	base_pos = glm::vec3(0.0, 0.6, 2.7);
	pos = base_pos;
	prev_pos = glm::vec3(0.0);
	pitch = 3;
	fov = 45.0;
	following_players = true;
	auto_linear_scale = 3.0;
	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));
}

glm::mat4 Camera::get_view() {
	return lookAt(pos, pos + front, up);
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
	pitch = clampf(-89.0, pitch + (y * speed), 89.0);
	fov = clampf(1.0, fov + (z * speed), max_fov);
	update_view();
}

void Camera::update_view() {
	glm::vec3 new_front;
	new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	new_front.y = sin(glm::radians(pitch));
	new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = normalize(new_front);

	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));
	RenderManager::get_instance()->update_shader_cams();
}

void Camera::follow_players(glm::vec2 p1, glm::vec2 p2, Stage* stage) {
	pos.x = clampf(stage->camera_bounds.x, (p1.x + p2.x) / 2.0, stage->camera_bounds.y) / 400.0;
	pos.y = clampf(base_pos.y, ((std::max(p1.y, p2.y)) / 200.0) - (base_pos.y / 4.0), 800.0);
	if (prev_pos != pos) {
		update_view();
	}
	prev_pos = pos;
}