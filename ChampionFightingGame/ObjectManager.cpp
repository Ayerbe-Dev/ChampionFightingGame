#include "ObjectManager.h"
#include "GameObject.h"

ObjectManager::ObjectManager() {
	world_rate = 1.0;
	world_frame = 0.0;
	real_time_object = nullptr;
	fighter[0] = nullptr;
	fighter[1] = nullptr;
	stage = nullptr;
}

void ObjectManager::process() {
	for (GameObject* object : game_objects) {
		object->process_render_pos();
	}
	if (is_frame_elapsed()) {
		world_frame = 0.0f;
	}
	world_frame += world_rate;
}

void ObjectManager::register_game_object(GameObject* object) {
	game_objects.push_back(object);
}

void ObjectManager::unregister_game_object(GameObject* object) {
	game_objects.remove(object);
}

bool ObjectManager::is_frame_elapsed() {
	if (world_rate == 0.0f) return false;
	float factor = 1.0 / world_rate;
	return world_frame >= world_rate * factor;
}

bool ObjectManager::is_allow_realtime_process(GameObject* object) {
	return is_frame_elapsed() || (real_time_object == object);
}

float ObjectManager::get_world_rate(GameObject* object) {
	if (real_time_object == object) {
		return 1.0f;
	}
	else {
		return world_rate;
	}
}

void ObjectManager::set_world_rate(GameObject* object, float world_rate) {
	if ((real_time_object == object) || (real_time_object == nullptr)) {
		real_time_object = object;
		this->world_rate = world_rate;
	}
}

void ObjectManager::reset_world_rate(GameObject* object) {
	if ((real_time_object == object) || (real_time_object == nullptr)) {
		world_frame = 1.0;
		world_rate = 1.0;
		real_time_object = nullptr;
	}
}

ObjectManager* ObjectManager::instance = nullptr;
ObjectManager* ObjectManager::get_instance() {
	if (instance == nullptr) {
		instance = new ObjectManager;
	}
	return instance;
}

void ObjectManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}