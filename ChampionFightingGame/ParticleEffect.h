#pragma once
#include "WorldTexture.h"
#include "Model.h"

class ParticleEffectInstance;

class Particle {
public:
	Particle();
	void render(ParticleEffectInstance* effect);
	virtual void render_unique(ParticleEffectInstance* effect);
protected:
	glm::vec3 pos_base;
	glm::vec3 rot_base;
	glm::vec3 scale_base;
	glm::vec4 rgba_base;
	glm::vec3 pos_frame;
	glm::vec3 rot_frame;
	glm::vec3 scale_frame;
	glm::vec4 rgba_frame;
	float duration;
	bool loop;
};

class ParticleModel : public Particle {
public:
	ParticleModel();
	void render_unique(ParticleEffectInstance* effect);
private:
	ModelInstance model;
	Animation* anim;
};

class ParticleBillboard : public Particle {
public:
	ParticleBillboard();
	void render_unique(ParticleEffectInstance* effect);
private:
	WorldTexture texture;
};

class ParticleTexture : public Particle {
public:
	ParticleTexture();
	void render_unique(ParticleEffectInstance* effect);
private:
	WorldTexture texture;
};

class ParticleTrail : public Particle {
public:
	ParticleTrail();
	void render_unique(ParticleEffectInstance* effect);
private:
	WorldTexture texture[2];
};


class ParticleEffectBase {
public:
	ParticleEffectBase();
	friend class ParticleEffectInstance;
protected:
	std::vector<Particle> particles;
};

class ParticleEffectInstance {
public:
	ParticleEffectInstance();
	void render();
	friend class Particle;
protected:
	ParticleEffectBase* base;
	float frame;
	bool flip;
};