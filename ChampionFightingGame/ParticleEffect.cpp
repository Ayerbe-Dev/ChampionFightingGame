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
	texture.set_pos(pos_base + (pos_frame * frame));
	texture.set_rot(rot_base + (rot_frame * frame));
	texture.set_scale(scale_base + (scale_frame * frame));
	texture.set_colormod(glm::vec3(rgba_base + (rgba_frame * frame)));
	texture.set_alpha(rgba_base.a + (rgba_frame.a * frame));
	texture.render();
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
	texture.set_pos(pos_base + (pos_frame * frame));
	texture.set_rot(rot_base + (rot_frame * frame));
	texture.set_scale(scale_base + (scale_frame * frame));
	texture.set_colormod(glm::vec3(rgba_base + (rgba_frame * frame)));
	texture.set_alpha(rgba_base.a + (rgba_frame.a * frame));
	texture.render();
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
	//Open the file at dir, use that to fill the particles array for this effect
}

void ParticleEffect::unload_particle_effect() {

}

ParticleEffectInstance& ParticleEffect::instantiate(GameObject* owner) {
	ParticleEffectInstance ret(this, owner);

	EffectManager* effect_manager = EffectManager::get_instance();
	for (size_t i = 0, max = particles.size(); i < max; i++) {
		effect_manager->register_particle_effect_instance(&ret, &particles[i]);
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
	EffectManager::get_instance()->unregister_particle_effect_instance(this);
}

float ParticleEffectInstance::get_frame() const {
	return frame;
}

bool ParticleEffectInstance::is_flipped() const {
	return flip;
}
