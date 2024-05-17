#pragma once
#include "WorldTexture.h"
#include "Model.h"

class GameObject;
class ParticleEffectInstance;

class Particle {
public:
	Particle();
	virtual void render(ParticleEffectInstance* effect);
protected:
	glm::vec3 pos_base;
	glm::vec3 rot_base;
	glm::vec3 scale_base;
	glm::vec4 rgba_base;
	glm::vec3 pos_frame;
	glm::vec3 rot_frame;
	glm::vec3 scale_frame;
	glm::vec4 rgba_frame;
	unsigned short duration;
	bool loop;
};

class ParticleModel : public Particle {
public:
	ParticleModel();
	void render(ParticleEffectInstance* effect);
private:
	ModelInstance model;
	Animation* anim;
};

class ParticleBillboard : public Particle {
public:
	ParticleBillboard();
	void render(ParticleEffectInstance* effect);
private:
	WorldTexture texture;
};

class ParticleTexture : public Particle {
public:
	ParticleTexture();
	void render(ParticleEffectInstance* effect);
private:
	WorldTexture texture;
};

class ParticleTrail : public Particle {
public:
	ParticleTrail();
	void render(ParticleEffectInstance* effect);
private:
	WorldTexture texture[2];
};

class ParticleEffect {
public:
	ParticleEffect();

	void load_particle_effect(std::string dir);
	void unload_particle_effect();
	ParticleEffectInstance& instantiate(GameObject* owner);
	unsigned short get_duration() const;
	bool is_loop() const;
private:
	std::vector<Particle> particles;
	unsigned short duration;
	bool loop;
};

class ParticleEffectInstance {
public:
	ParticleEffectInstance();
	ParticleEffectInstance(ParticleEffect* base, GameObject* owner);

	void process();

	void deactivate();

	float get_frame() const;
	bool is_flipped() const;
private:
	ParticleEffect* base;
	GameObject* owner;
	float frame;
	bool flip;	
};