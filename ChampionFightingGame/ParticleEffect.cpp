#include "ParticleEffect.h"

Particle::Particle() {
	pos_base = glm::vec3(0.0);
	rot_base = glm::vec3(0.0);
	scale_base = glm::vec3(0.0);
	rgba_base = glm::vec4(0.0);
	pos_frame = glm::vec3(0.0);
	rot_frame = glm::vec3(0.0);
	scale_frame = glm::vec3(0.0);
	rgba_frame = glm::vec4(0.0);
	duration = 0.0;
	loop = false;
}

void Particle::render(ParticleEffectInstance* effect) {
	if (effect->frame >= duration && !loop) return;
	render_unique(effect, std::fmod(effect->frame, duration));
}

void Particle::render_unique(ParticleEffectInstance* effect, float frame) {}

ParticleModel::ParticleModel() {
	anim = nullptr;
}

void ParticleModel::render_unique(ParticleEffectInstance* effect, float frame) {

}

ParticleBillboard::ParticleBillboard() {

}

void ParticleBillboard::render_unique(ParticleEffectInstance* effect, float frame) {

}

ParticleTexture::ParticleTexture() {

}

void ParticleTexture::render_unique(ParticleEffectInstance* effect, float frame) {

}

ParticleTrail::ParticleTrail() {

}

void ParticleTrail::render_unique(ParticleEffectInstance* effect, float frame) {

}

ParticleEffectBase::ParticleEffectBase() {

}

ParticleEffectInstance::ParticleEffectInstance() {
	base = nullptr;
	flip = false;
	frame = 0.0;
}

void ParticleEffectInstance::render() {
	for (size_t i = 0, max = base->particles.size(); i < max; i++) {
		base->particles[i].render(this);
	}
}