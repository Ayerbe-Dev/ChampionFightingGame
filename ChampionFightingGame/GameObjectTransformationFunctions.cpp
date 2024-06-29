#include "GameObject.h"
#include "ObjectManager.h"
#include "WindowConstants.h"

void GameObject::add_pos(glm::vec3 pos) {
	pos *= object_manager->get_world_rate(this);
	pos /= scale_vec;
	this->pos += pos;
}

void GameObject::set_pos(glm::vec3 pos) {
	pos /= scale_vec;
	this->pos = pos;
}

void GameObject::set_pos_x(float pos) {
	pos /= scale_vec.x;
	this->pos.x = pos;
}

void GameObject::set_pos_y(float pos) {
	pos /= scale_vec.y;
	this->pos.y = pos;
}

void GameObject::set_pos_z(float pos) {
	pos /= scale_vec.z;
	this->pos.z = pos;
}

glm::vec3 GameObject::get_pos() const {
	return pos;
}

glm::vec3 GameObject::get_pos_unscaled() const {
	return pos * scale_vec;
}

void GameObject::add_rot(glm::vec3 rot) {
	rot = glm::radians(rot);
	this->rot += rot;
	front = normalize(glm::vec3(
		sin(glm::radians(rot.x)) * cos(glm::radians(rot.y)),
		sin(glm::radians(rot.y)),
		-cos(glm::radians(rot.x)) * cos(glm::radians(rot.y))
	));
	right = normalize(cross(front, glm::vec3(0, 1, 0)));
	up = normalize(cross(right, front));
}

void GameObject::set_rot(glm::vec3 rot) {
	rot = glm::radians(rot);
	this->rot = rot;
	front = normalize(glm::vec3(
		sin(glm::radians(rot.x)) * cos(glm::radians(rot.y)),
		sin(glm::radians(rot.y)),
		-cos(glm::radians(rot.x)) * cos(glm::radians(rot.y))
	));
	right = normalize(cross(front, glm::vec3(0, 1, 0)));
	up = normalize(cross(right, front));
}

void GameObject::set_rot_x(float x) {
	x = glm::radians(x);
	this->rot.x = x;
	front = normalize(glm::vec3(
		sin(glm::radians(rot.x)) * cos(glm::radians(rot.y)),
		sin(glm::radians(rot.y)),
		-cos(glm::radians(rot.x)) * cos(glm::radians(rot.y))
	));
	right = normalize(cross(front, glm::vec3(0, 1, 0)));
	up = normalize(cross(right, front));
}

void GameObject::set_rot_y(float y) {
	y = glm::radians(y);
	this->rot.y = y;
	front = normalize(glm::vec3(
		sin(glm::radians(rot.x)) * cos(glm::radians(rot.y)),
		sin(glm::radians(rot.y)),
		-cos(glm::radians(rot.x)) * cos(glm::radians(rot.y))
	));
	right = normalize(cross(front, glm::vec3(0, 1, 0)));
	up = normalize(cross(right, front));
}

void GameObject::set_rot_z(float z) {
	z = glm::radians(z);
	this->rot.z = z;
	front = normalize(glm::vec3(
		sin(glm::radians(rot.x)) * cos(glm::radians(rot.y)),
		sin(glm::radians(rot.y)),
		-cos(glm::radians(rot.x)) * cos(glm::radians(rot.y))
	));
	right = normalize(cross(front, glm::vec3(0, 1, 0)));
	up = normalize(cross(right, front));
}


glm::vec3 GameObject::get_rot() const {
	return rot;
}

void GameObject::add_scale(glm::vec3 scale) {
	pos *= scale_vec;
	this->scale += scale;
	scale_vec = glm::vec3(
		WINDOW_WIDTH / (100 * scale.x),
		WINDOW_HEIGHT / (100 * scale.y),
		WINDOW_DEPTH / (100 * scale.z)
	);
	pos /= scale_vec;
}

void GameObject::set_scale(glm::vec3 scale) {
	pos *= scale_vec;
	this->scale = scale;
	scale_vec = glm::vec3(
		WINDOW_WIDTH / (100 * scale.x),
		WINDOW_HEIGHT / (100 * scale.y),
		WINDOW_DEPTH / (100 * scale.z)
	);
	pos /= scale_vec;
}

glm::vec3 GameObject::get_scale() const {
	return scale;
}

glm::vec3 GameObject::get_scale_vec() const {
	return scale_vec;
}

glm::vec3 GameObject::get_front() const {
	return front;
}

glm::vec3 GameObject::get_right() const {
	return right;
}

glm::vec3 GameObject::get_up() const {
	return up;
}