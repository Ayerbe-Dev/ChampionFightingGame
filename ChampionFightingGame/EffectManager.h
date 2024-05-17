#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <utility>
#include <glm/glm.hpp>
#include "ParticleEffect.h"

//OLD

struct EffectInfo;
class Effect;
class EffectInstance;
class BattleObject;

struct ParticleEffectResource {
	ParticleEffectResource();
	ParticleEffect particle_effect;
	int user_count;
};

class EffectManager {
public:
	EffectManager(EffectManager& other) = delete;
	void operator=(const EffectManager& other) = delete;


	//NEW

	void process_effects();

	void register_game_object(GameObject* object);
	void unregister_game_object(GameObject* object);

	void clear_game_objects();

	void register_particle_effect_instance(ParticleEffectInstance* instance, Particle* particle);
	void unregister_particle_effect_instance(ParticleEffectInstance* instance);

	ParticleEffect& get_particle_effect(std::string dir);
	void load_particle_effect(std::string dir);
	void unuse_particle_effect(std::string dir);
	void unload_particle_effect(std::string dir, bool strict = true);
	void unload_all_particle_effects();
	void unload_unused();

	//OLD

	void process();
	void render();

	void activate_effect(int object_id, std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
		glm::vec4 rgba, GameObject* game_object, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame,
		glm::vec3 rot_frame, glm::vec3 scale_frame, glm::vec4 rgba_frame, int* interp_var = nullptr, float rate = 1.0, float frame = 0.0);

	void clear_effect(int object_id, std::string name, int instance_id = 0);
	void clear_effect_all(int object_id = -1);
	Effect get_effect(std::string name);
	EffectInstance& get_effect_instance(int object_id, std::string name, int instance_id = 0);

	void load_effect(std::string name);
	void unload_effect(std::string name);
	void unload_all_effects();

	int add_effect_caster();
	void add_effect_caster(int object_id);
	void remove_effect_caster(int id);
	void remove_effect_casters();

	static EffectManager* get_instance();
	void destroy_instance();
private:
	EffectManager();

	//NEW

	std::list<std::pair<ParticleEffectInstance*, Particle*>> particle_effect_instances;
	std::list<GameObject*> game_objects;
	std::map<std::string, ParticleEffectResource> particle_effect_map;

	//OLD

	void populate_effects();
	void add_effect_info(std::string name, std::string dir);

	std::vector<EffectInfo> effect_info;
	std::unordered_map<std::string, int> effect_info_map;

	std::vector<Effect> loaded_effects;
	std::unordered_map<std::string, int> effect_name_map;

	std::vector<std::list<EffectInstance>> active_effects;
	std::unordered_map<int, int> id2index;

	static EffectManager* instance;
};