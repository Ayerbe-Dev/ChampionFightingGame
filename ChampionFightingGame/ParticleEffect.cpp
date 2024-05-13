#include "ParticleEffect.h"
#include "GameObject.h"
#include "EffectManager.h"

Particle::Particle() {
	pos_base = glm::vec3(0.0);
	rot_base = glm::vec3(0.0);
	scale_base = glm::vec3(0.0);
	rgba_base = glm::vec4(0.0);
	pos_frame = glm::vec3(0.0);
	rot_frame = glm::vec3(0.0);
	scale_frame = glm::vec3(0.0);
	rgba_frame = glm::vec4(0.0);
	duration = 0;
	loop = false;
}
void Particle::render(ParticleEffectInstance* effect) {

}

ParticleModel::ParticleModel() {
	anim = nullptr;
}

void ParticleModel::render(ParticleEffectInstance* effect) {
	float frame = effect->get_frame();
	if (frame >= duration && !loop) {
		return;
	}
	else {
		frame = std::fmod(effect->get_frame(), duration);
	}
}

ParticleBillboard::ParticleBillboard() {

}

void ParticleBillboard::render(ParticleEffectInstance* effect) {
	float frame = effect->get_frame();
	if (frame >= duration && !loop) {
		return;
	}
	else {
		frame = std::fmod(effect->get_frame(), duration);
	}
}

ParticleTexture::ParticleTexture() {

}

void ParticleTexture::render(ParticleEffectInstance* effect) {
	float frame = effect->get_frame();
	if (frame >= duration && !loop) {
		return;
	}
	else {
		frame = std::fmod(effect->get_frame(), duration);
	}
}

ParticleTrail::ParticleTrail() {

}

void ParticleTrail::render(ParticleEffectInstance* effect) {
	float frame = effect->get_frame();
	if (frame >= duration && !loop) {
		return;
	}
	else {
		frame = std::fmod(effect->get_frame(), duration);
	}
}

ParticleEffect::ParticleEffect() {
	duration = 0;
	loop = false;
}

void ParticleEffect::load_particle_effect(std::string dir) {

}

ParticleEffectInstance& ParticleEffect::instantiate(GameObject* owner) {
	ParticleEffectInstance ret(this, owner);

	EffectManager* effect_manager = EffectManager::get_instance();
	for (size_t i = 0, max = particles.size(); i < max; i++) {
		//TODO: Revamp the effect manager and add register_particle()
		//effect_manager->register_particle(&ret, &particles[i]);
	}

	return ret;
}

unsigned short ParticleEffect::get_duration() const {
	return duration;
}

bool ParticleEffect::is_loop() const {
	return loop;
}

ParticleEffectInstance::ParticleEffectInstance() {
	base = nullptr;
	owner = nullptr;
	flip = false;
	frame = 0.0;
}

ParticleEffectInstance::ParticleEffectInstance(ParticleEffect* base, GameObject* owner) {
	this->base = base;
	this->owner = owner;
	flip = false;
	frame = 0.0;
}

void ParticleEffectInstance::process() {
	frame += owner->frame_delta;
	if (frame >= base->get_duration() && !base->is_loop()) {
		deactivate();
	}
}

void ParticleEffectInstance::deactivate() {
	//EffectManager::get_instance()->unregister_particle_effect_instance(this);
}

float ParticleEffectInstance::get_frame() const {
	return frame;
}

bool ParticleEffectInstance::is_flipped() const {
	return flip;
}
