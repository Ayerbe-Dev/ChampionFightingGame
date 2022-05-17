#include "Camera.h"
#include "Stage.h"
#include "utils.h"

Camera::Camera() {
	pos = glm::vec3(0.0, 1.3, 4.0);
	fov = 45.0;
	following_players = true;
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
}

void Camera::follow_players(glm::vec2 p1, glm::vec2 p2, Stage* stage) {
	//Should move the camera into a position such that it balances the players and slightly tilts towards
	//whichever player is further from the center

	//I'll let you work your magic to figure out how to actually do that
}