#include "Effect.h"
#include "Particle.h"
#include <fstream>
#include "RenderManager.h"

EffectInfo::EffectInfo() {

}

EffectInfo::EffectInfo(std::string name, std::string dir) {
	this->name = name;
	this->dir = dir;
}

Effect::Effect() {
	duration = -1;
}

Effect::Effect(EffectInfo info) {
	init(info);
}

void Effect::init(EffectInfo info) {
	std::ifstream stream;
	stream.open(info.dir + "effect_info.yml");
	if (stream.fail()) {
		stream.close();
		return;
	}
	this->info = info;
	stream >> duration;

	std::string particle_name;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec4 rgba;
	bool has_spritesheet;
	while (stream >> particle_name) {
		stream >> pos.x >> pos.y >> pos.z;
		stream >> rot.x >> rot.y >> rot.z;
		stream >> scale.x >> scale.y >> scale.z;
		stream >> rgba.x >> rgba.y >> rgba.z >> rgba.w;
		stream >> has_spritesheet;
		Particle particle(info.dir + particle_name + ".png", pos, rot, scale, rgba);
		if (has_spritesheet) {
			particle.load_spritesheet(info.dir + particle_name + ".yml");
		}
		particles.push_back(particle);
	}
	RenderManager* render_manager = RenderManager::get_instance();
	attach_shader(&render_manager->default_effect_shader);
	shader->use();
}

void Effect::destroy() {
	for (int i = 0, max = particles.size(); i < max; i++) {
		particles[i].destroy();
	}
}

void Effect::attach_shader(Shader* shader) {
	this->shader = shader;
}

EffectInstance Effect::instantiate(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, float rate, float frame) {
	EffectInstance ret(this, pos, rot, scale, rgba, rate, frame);
	return ret;
}

EffectInstance::EffectInstance() {
	effect = nullptr;
	shader = nullptr;
	pos = glm::vec3(0.0);
	rot = glm::vec3(0.0);
	scale = glm::vec3(1.0);
	rgba = glm::vec4(1.0);
	frame = 0.0;
	rate = 1.0;
}

EffectInstance::EffectInstance(Effect* effect, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale, glm::vec4 rgba, float rate, float frame) {
	this->effect = effect;
	shader = effect->shader;
	this->pos = pos;
	this->rot = rot;
	this->scale = scale;
	this->rgba = rgba;
	this->rate = rate;
	this->frame = frame;
}

bool EffectInstance::process() {
	frame += rate;
	if (frame >= effect->duration) {
		return false;
	}

	return true;
}

void EffectInstance::render() {
	glDepthMask(GL_TRUE);
	shader->use();
	shader->set_int("f_texture", 0);
	glActiveTexture(GL_TEXTURE0);
	RenderManager::get_instance()->update_shader_cam(shader);
	for (int i = 0, max = effect->particles.size(); i < max; i++) {
		effect->particles[i].render(shader, pos, rot, scale, rgba, (int)frame);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}