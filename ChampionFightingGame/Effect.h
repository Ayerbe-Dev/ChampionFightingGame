#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "Light.h"
#include "Shader.h"

class GameObject;
class BattleObject;
class OldParticle;
class EffectInstance;

//EffectInfo contains the data to be loaded at the start of every match. Essentially it's info that we should
//know about BEFORE opening the file. 
struct EffectInfo {
	EffectInfo();
	EffectInfo(std::string name, std::string dir);

	std::string name;
	std::string dir;
};

//Effect contains the data that actually gets loaded. It contains a list of all particles which make up the
//effect, and other values that are shared across all instances of the effect.
class Effect {
public:
	Effect();
	Effect(EffectInfo info);

	void init(EffectInfo info);
	void destroy();
	void attach_shader(Shader* shader);
	EffectInstance instantiate(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba,
		GameObject* game_object, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame,
		glm::vec3 rot_frame, glm::vec3 scale_frame, glm::vec4 rgba_frame, int* interp_var, float rate, float frame);

	EffectInfo info;
	std::vector<OldParticle> particles;
	int duration;
	Shader *shader;
};

//EffectInstances are what actually get rendered to the screen. Each instance can have a different position,
//color etc.

//Essentially, all of the particles from the base effect are rendered, and their data is offset by this 
//instance's data
class EffectInstance {
public:
	EffectInstance();
	EffectInstance(Effect* effect, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba,
		GameObject* game_object, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame,
		glm::vec3 rot_frame, glm::vec3 scale_frame, glm::vec4 rgba_frame, int* interp_var, float frame, float rate);

	bool process();
	void render();

	Effect* effect;
	float frame;
private:
	Shader* shader;
	GameObject* game_object;

	glm::vec3 pos; //Instance-specific values
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec4 rgba;

	int bone_id;
	glm::vec3 bone_offset;

	glm::vec3 pos_frame; //How much to deviate from the original values per frame
	glm::vec3 rot_frame;
	glm::vec3 scale_frame;
	glm::vec4 rgba_frame;

	glm::vec3 final_pos; //Only listed as members for storage purposes
	glm::vec3 final_rot;
	glm::vec3 final_scale;
	glm::vec4 final_rgba;
	glm::vec3 scale_vec;

	int* interp_var; //This is basically just the pointer to the owner's hitlag
	bool interpolating;

	bool flip;
	float rate;
};