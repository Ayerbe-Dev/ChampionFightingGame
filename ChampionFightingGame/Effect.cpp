#include "Effect.h"
#include "Particle.h"
#include <fstream>

EffectInfo::EffectInfo() {

}

EffectInfo::EffectInfo(std::string name, std::string dir) {
	this->name = name;
	this->dir = dir;
}

Effect::Effect() {
	duration = -1;
}

void Effect::init(EffectInfo info) {
	std::ifstream stream;
	stream.open(info.dir);
	if (stream.fail()) {
		stream.close();
		return;
	}
	this->info = info;

	//Read in the particle info
}

void Effect::destroy() {
	for (int i = 0, max = particles.size(); i < max; i++) {
		particles[i].destroy();
	}
}

EffectInstance Effect::instantiate(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, float rate, float frame) {
	EffectInstance ret(this, pos, rot, scale, rgba, rate, frame);
	return ret;
}

EffectInstance::EffectInstance() {
	effect = nullptr;
	pos = glm::vec3(0.0);
	rot = glm::vec3(0.0);
	scale = glm::vec3(1.0);
	rgba = glm::vec4(1.0);
	frame = 0.0;
	rate = 1.0;
}

EffectInstance::EffectInstance(Effect* effect, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, float rate, float frame) {
	this->effect = effect;
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->rgba = rgba;
	this->rate = rate;
	this->frame = frame;
	shader.init("vertex_effect", "fragment_effect");
}

bool EffectInstance::process() {
	if (frame >= effect->duration) {
		return false;
	}
	frame += rate;

	return true;
}

void EffectInstance::render() {
	shader.use();
	for (int i = 0, max = effect->particles.size(); i < max; i++) {
		if (effect->particles[i].spritesheet) {
			effect->particles[i].render(&shader, pos, rot, scale, rgba, (int)frame);
		}
		else {
			effect->particles[i].render(&shader, pos, rot, scale, rgba);
		}
	}
}