#include "Camera.h"

Camera::Camera() {}

mat4 Camera::get_view() {
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
	fov = clampf(1.0, fov + (z * speed), 45.0);
	update_view();
}

void Camera::update_view() {
	glm::vec3 new_front;
	new_front.x = cos(radians(yaw)) * cos(radians(pitch));
	new_front.y = sin(radians(pitch));
	new_front.z = sin(radians(yaw)) * cos(radians(pitch));
	front = normalize(new_front);

	right = normalize(cross(front, world_up));
	up = normalize(cross(right, front));
}