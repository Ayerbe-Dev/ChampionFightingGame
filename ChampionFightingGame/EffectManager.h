#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <glm/glm.hpp>

struct EffectInfo;
class Effect;
class EffectInstance;
class BattleObject;

class EffectManager {
public:
	EffectManager(EffectManager& other) = delete;
	void operator=(const EffectManager& other) = delete;

	static EffectManager* get_instance();

	void render();

	void activate_effect(int object_id, std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, 
		glm::vec4 rgba, BattleObject* battle_object, int bone_id, glm::vec3 pos_frame, glm::vec3 rot_frame, 
		glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate = 1.0, float frame = 0.0);
	
	void clear_effect(int object_id, std::string name, int instance_id = 0);
	void clear_effect_all(int object_id = -1);
	Effect get_effect(std::string name);
	EffectInstance& get_effect_instance(int object_id, std::string name, int instance_id = 0);

	void load_effect(std::string name);
	void unload_effects();

	void add_effect_caster(int object_id);
	void remove_effect_casters();
private:
	EffectManager();
	void init();
	void add_effect_info(std::string name, std::string dir);

	std::vector<EffectInfo> effect_info;
	std::unordered_map<std::string, int> effect_info_map;

	std::vector<Effect> loaded_effects;
	std::unordered_map<std::string, int> effect_name_map;

	std::vector<std::list<EffectInstance>> active_effects;
	std::unordered_map<int, int> id2index;

	static EffectManager* instance;
};