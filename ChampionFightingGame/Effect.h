#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "Light.h"
#include "Shader.h"

class Particle;
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
	EffectInstance instantiate(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, float rate = 1.0, float frame = 0.0);

	EffectInfo info;
	std::vector<Particle> particles;
	int duration;
};

//EffectInstances are what actually get rendered to the screen. Each instance can have a different position,
//color etc.

//Essentially, all of the particles from the base effect are rendered, and their data is offset by this 
//instance's data
class EffectInstance {
public:
	EffectInstance();
	EffectInstance(Effect* effect, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, float frame, float rate);

	bool process();
	void render();
	Effect* effect;
private:
	Shader shader;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec4 rgba;
	float frame;
	float rate;
};