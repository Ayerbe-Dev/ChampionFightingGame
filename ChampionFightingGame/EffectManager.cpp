#include "EffectManager.h"
#include "Effect.h"
#include "Particle.h";
#include <stdexcept>

EffectManager* EffectManager::instance = nullptr;

EffectManager::EffectManager() {
	init();
}

EffectManager* EffectManager::get_instance() {
	if (instance == nullptr) {
		instance = new EffectManager;
	}
	return instance;
}

void EffectManager::init() {
	add_effect_info("flame", "resource/chara/roy/effects/flame/");
}

//We look at every active effect across all objects. If any of them have been active for their entire duration,
//remove them. If not, render them.
void EffectManager::render() {
	for (int i = 0, max = active_effects.size(); i < max; i++) {
		std::list<EffectInstance>::iterator it = active_effects[i].begin();
		for (int i2 = 0; i2 < active_effects[i].size(); i2++) {
			if (it->process()) {
				it->render();
				std::next(it, 1);
			}
			else {
				active_effects[i].erase(it);
				it = active_effects[i].begin(); //How the fuck do iterators
				std::next(it, i2);
			}
		}
	}
}

void EffectManager::activate_effect(int object_id, std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, float rate, float frame) {
	if (effect_name_map.find(name) == effect_name_map.end()) {
		std::cerr << "Effect " << name << " isn't loaded!\n";
		return;
	}
	if (id2index.find(object_id) == id2index.end()) {
		std::cerr << "ID " << object_id << " is not a valid effect caster!\n";
		return;
	}

	EffectInstance to_add = loaded_effects[effect_name_map[name]].instantiate(pos, rot, scale, rgba, rate, frame);
	active_effects[id2index[object_id]].push_back(to_add);
}

void EffectManager::clear_effect(int object_id, std::string name, int instance_id) {
	if (id2index.find(object_id) == id2index.end()) {
		std::cerr << "ID " << object_id << " is not a valid effect caster!\n";
		return;
	}
	std::list<EffectInstance>::iterator it = active_effects[id2index[object_id]].begin();
	for (int i = 0, max = active_effects[id2index[object_id]].size(); i < max; i++) {
		if (it->effect->info.name == name) {
			if (instance_id == 0) {
				active_effects[id2index[object_id]].erase(it);
				return;
			}
			else {
				instance_id--;
			}
		}
		std::next(it, 1);
	}
	std::cerr << "Instance ID not found!\n";
}

//If no argument is passed, every effect across all objects will be cleared.
void EffectManager::clear_effect_all(int object_id) {
	if (object_id == -1) {
		for (int i = 0, max = active_effects.size(); i < max; i++) {
			active_effects[i].clear();
		}
	}
	else {
		active_effects[id2index[object_id]].clear();
	}
}

EffectInstance& EffectManager::get_effect_instance(int object_id, std::string name, int instance_id) {
	if (id2index.find(object_id) == id2index.end()) {
		throw std::range_error("ID of object caster not in EffectManager!");
	}
	std::list<EffectInstance>::iterator it = active_effects[id2index[object_id]].begin();
	for (int i = 0, max = active_effects[id2index[object_id]].size(); i < max; i++) {
		if (it->effect->info.name == name) {
			if (instance_id == 0) {
				return *it;
			}
			else {
				instance_id--;
			}
		}
		std::next(it, 1);
	}
	throw std::range_error("Instance ID not found!");
}

void EffectManager::load_effect(std::string name) {
	if (effect_info_map.find(name) == effect_info_map.end()) {
		std::cerr << "Effect " << name << " doesn't exist!\n";
		return;
	}
	if (effect_name_map.find(name) != effect_name_map.end()) {
		return; //The effect was already loaded, probably by another player
	}
	Effect to_load(effect_info[effect_info_map[name]]);
	effect_name_map[name] = loaded_effects.size();
	loaded_effects.push_back(to_load);
}

void EffectManager::unload_effects() {
	loaded_effects.clear();
	effect_name_map.clear();
}

//Adds another index to the active effects vector, then maps our object ID to that index
void EffectManager::add_effect_caster(int object_id) {
	id2index[object_id] = active_effects.size();
	active_effects.resize(active_effects.size() + 1);
}

//Note: DO NOT call this function if there are any active effects, or you will leak memory
void EffectManager::remove_effect_casters() {
	active_effects.clear();
	id2index.clear();
}

void EffectManager::add_effect_info(std::string name, std::string dir) {
	effect_info_map[name] = effect_info.size();
	EffectInfo to_add(name, dir);
	effect_info.push_back(to_add);
}