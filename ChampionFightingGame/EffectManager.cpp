#include <stdexcept>
#include "EffectManager.h"
#include "ThreadManager.h"
#include "Effect.h"
#include "Particle.h"
#include "ParticleEffect.h"
#include "BattleObject.h"
#include "utils.h"

ParticleEffectResource::ParticleEffectResource() {
	user_count = 0;
}

EffectManager::EffectManager() {
	populate_effects();
}

void EffectManager::process_effects() {

}

void EffectManager::register_game_object(GameObject* object) {
	game_objects.push_back(object);
}

void EffectManager::unregister_game_object(GameObject* object) {
	game_objects.remove(object);
}

void EffectManager::clear_game_objects() {
	game_objects.clear();
}

void EffectManager::register_particle_effect_instance(ParticleEffectInstance* instance, Particle* particle) {
	particle_effect_instances.push_back(std::pair(instance, particle));
}

void EffectManager::unregister_particle_effect_instance(ParticleEffectInstance* instance) {
	for (auto &pair : particle_effect_instances) {
		if (pair.first == instance) {
			particle_effect_instances.remove(pair);
		}
	}
}

ParticleEffect& EffectManager::get_particle_effect(std::string dir) {
	if (!particle_effect_map.contains(dir)) {
		load_particle_effect(dir);
	}
	particle_effect_map[dir].user_count++;
	return particle_effect_map[dir].particle_effect;
}

void EffectManager::load_particle_effect(std::string dir) {
	particle_effect_map[dir].particle_effect.load_particle_effect(dir);
	particle_effect_map[dir].user_count = 0;
}

void EffectManager::unuse_particle_effect(std::string dir) {
	if (particle_effect_map.contains(dir)) {
		particle_effect_map[dir].user_count--;
	}
}

void EffectManager::unload_particle_effect(std::string dir, bool strict) {
	if (particle_effect_map.contains(dir)) {
		if (particle_effect_map[dir].user_count == 0 || !strict) {
			particle_effect_map[dir].particle_effect.unload_particle_effect();
			particle_effect_map.erase(dir);
		}
	}
}

void EffectManager::unload_all_particle_effects() {
	for (auto& particle_effect : particle_effect_map) {
		particle_effect.second.particle_effect.unload_particle_effect();
	}
	particle_effect_map.clear();
}

void EffectManager::unload_unused() {
	for (auto& particle_effect : particle_effect_map) {
		if (particle_effect.second.user_count == 0) {
			particle_effect.second.particle_effect.unload_particle_effect();
			particle_effect_map.erase(particle_effect.first);
		}
	}
}

void EffectManager::populate_effects() {
	add_effect_info("flame", "resource/chara/eric/effects/flame/");
}

void EffectManager::process() {
	for (int i = 0, max = active_effects.size(); i < max; i++) {
		for (std::list<EffectInstance>::iterator it = active_effects[i].begin(); it != active_effects[i].end(); it++) {
			if (!it->process()) {
				if (active_effects[i].size() != 1) {
					it = active_effects[i].erase(it);
				}
				else {
					active_effects[i].erase(it);
					break;
				}
			}
		}
	}
}

void EffectManager::render() {
	for (int i = 0, max = active_effects.size(); i < max; i++) {
		for (std::list<EffectInstance>::iterator it = active_effects[i].begin(), max = active_effects[i].end(); it != max; it++) {
			it->render();
		}
	}
}

void EffectManager::activate_effect(int object_id, std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	glm::vec4 rgba, GameObject* game_object, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame,
	glm::vec3 rot_frame, glm::vec3 scale_frame, glm::vec4 rgba_frame, int* interp_var, float rate, float frame) {
	if (effect_name_map.find(name) == effect_name_map.end()) {
		std::cerr << "Effect " << name << " isn't loaded!\n";
		return;
	}
	if (id2index.find(object_id) == id2index.end()) {
		std::cerr << "ID " << object_id << " is not a valid effect caster!\n";
		return;
	}

	EffectInstance to_add = loaded_effects[effect_name_map[name]].instantiate(pos, rot, scale, rgba, game_object,
		bone_id, bone_offset, pos_frame, rot_frame, scale_frame, rgba_frame, interp_var, rate, frame);

	active_effects[id2index[object_id]].push_back(to_add);
}

void EffectManager::clear_effect(int object_id, std::string name, int instance_id) {
	if (id2index.find(object_id) == id2index.end()) {
		std::cerr << "ID " << object_id << " is not a valid effect caster!\n";
		return;
	}
	for (std::list<EffectInstance>::iterator it = active_effects[id2index[object_id]].begin(), max = active_effects[id2index[object_id]].end(); it != max; it++) {
		if (it->effect->info.name == name) {
			if (instance_id == 0) {
				active_effects[id2index[object_id]].erase(it);
				return;
			}
			else {
				instance_id--;
			}
		}
	}
	std::cerr << "Clear Effect: Instance ID for effect " << name << " not found!\n";
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

Effect EffectManager::get_effect(std::string name) {
	if (effect_name_map.find(name) == effect_name_map.end()) {
		std::cerr << "Effect " << name << " isn't loaded!\n";
		return Effect();
	}
	return loaded_effects[effect_name_map[name]];
}

EffectInstance& EffectManager::get_effect_instance(int object_id, std::string name, int instance_id) {
	if (!id2index.contains(object_id)) {
		throw std::range_error("ID of object caster not in EffectManager!");
	}
	for (std::list<EffectInstance>::iterator it = active_effects[id2index[object_id]].begin(), max = active_effects[id2index[object_id]].end(); it != max; it++) {
		if (it->effect->info.name == name) {
			if (instance_id == 0) {
				return *it;
			}
			else {
				instance_id--;
			}
		}
	}
	throw std::range_error("Get Effect Instance: Instance ID for effect " + name + " not found!");
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

void EffectManager::unload_effect(std::string name) {
	if (effect_name_map.find(name) == effect_name_map.end()) {
		std::cout << "Effect " << name << " is not loaded!\n";
		return;
	}
	Effect& to_unload = loaded_effects[effect_name_map[name]];
	to_unload.destroy();
	effect_name_map.erase(name);
}

void EffectManager::unload_all_effects() {
	for (int i = 0, max = loaded_effects.size(); i < max; i++) {
		loaded_effects[i].destroy();
	}
	loaded_effects.clear();
	effect_name_map.clear();
}

int EffectManager::add_effect_caster() {
	int i;
	for (i = 50; id2index.contains(i); i++); //Slots 0-29 are reserved for fighters and projectiles, slots
	//30-49 are reserved for stage assets. 50+ is where we get to things that aren't necessarily assigned
	//their own IDs
	id2index[i] = active_effects.size();
	active_effects.resize(active_effects.size() + 1);
	return i;
}

//Adds another index to the active effects vector, then maps our object ID to that index
void EffectManager::add_effect_caster(int object_id) {
	id2index[object_id] = active_effects.size();
	active_effects.resize(active_effects.size() + 1);
}

void EffectManager::remove_effect_caster(int id) {
	clear_effect_all(id);
	active_effects.erase(active_effects.begin() + id2index[id]);
	id2index.erase(id);
}

void EffectManager::remove_effect_casters() {
	clear_effect_all();
	active_effects.clear();
	id2index.clear();
}

void EffectManager::add_effect_info(std::string name, std::string dir) {
	effect_info_map[name] = effect_info.size();
	EffectInfo to_add(name, dir);
	effect_info.push_back(to_add);
}

EffectManager* EffectManager::instance = nullptr;
EffectManager* EffectManager::get_instance() {
	if (instance == nullptr) {
		instance = new EffectManager;
	}
	return instance;
}

void EffectManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}