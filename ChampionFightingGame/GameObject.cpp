#include "GameObject.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"
#include "utils.h"

GameObject::GameObject() {
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	shader_manager = ShaderManager::get_instance();
	sound_manager = SoundManager::get_instance();
	anim_kind = nullptr;
	prev_anim_kind = nullptr;
	frame = 0.0;
	prev_anim_frame = 0.0;
	rate = 1.0;
	prev_anim_rate = 0.0;
	prev_anim_offset = glm::vec3(0.0);
	pos = glm::vec3(0.0);
}

GameObject::GameObject(GameObject& other) {
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	shader_manager = ShaderManager::get_instance();
	sound_manager = SoundManager::get_instance();
	this->id_effect = other.id_effect;
	this->resource_dir = other.resource_dir;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->extra_mat = other.extra_mat;
	this->anim_table = other.anim_table;
	if (other.anim_kind != nullptr) {
		this->anim_kind = anim_table.get_anim(other.anim_kind->name, false);
	}
	else {
		this->anim_kind = nullptr;
	}
	if (other.prev_anim_kind != nullptr) {
		this->prev_anim_kind = anim_table.get_anim(other.prev_anim_kind->name, false);
	}
	else {
		this->prev_anim_kind = nullptr;
	}
	this->prev_anim_offset = other.prev_anim_offset;
	this->frame = other.frame;
	this->rate = other.rate;
	this->prev_anim_frame = other.prev_anim_frame;
	this->prev_anim_rate = other.prev_anim_rate;
	this->is_anim_end = other.is_anim_end;
	this->shader = other.shader;
	this->shadow_shader = other.shadow_shader;
	this->outline_shader = other.outline_shader;
	this->model = other.model;
}

GameObject::GameObject(const GameObject& other) {
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	shader_manager = ShaderManager::get_instance();
	sound_manager = SoundManager::get_instance();
	this->id_effect = other.id_effect;
	this->resource_dir = other.resource_dir;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->extra_mat = other.extra_mat;
	this->anim_table = other.anim_table;
	if (other.anim_kind != nullptr) {
		this->anim_kind = anim_table.get_anim(other.anim_kind->name, false);
	}
	else {
		this->anim_kind = nullptr;
	}
	if (other.prev_anim_kind != nullptr) {
		this->prev_anim_kind = anim_table.get_anim(other.prev_anim_kind->name, false);
	}
	else {
		this->prev_anim_kind = nullptr;
	}
	this->prev_anim_offset = other.prev_anim_offset;
	this->frame = other.frame;
	this->rate = other.rate;
	this->prev_anim_frame = other.prev_anim_frame;
	this->prev_anim_rate = other.prev_anim_rate;
	this->is_anim_end = other.is_anim_end;
	this->shader = other.shader;
	this->shadow_shader = other.shadow_shader;
	this->outline_shader = other.outline_shader;
	this->model = other.model;
}

GameObject& GameObject::operator=(GameObject& other) {
	if (this == &other) {
		return *this;
	}
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	shader_manager = ShaderManager::get_instance();
	sound_manager = SoundManager::get_instance();

	this->id_effect = other.id_effect;
	this->resource_dir = other.resource_dir;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->extra_mat = other.extra_mat;
	this->anim_table = other.anim_table;
	if (other.anim_kind != nullptr) {
		this->anim_kind = anim_table.get_anim(other.anim_kind->name, false);
	}
	else {
		this->anim_kind = nullptr;
	}
	if (other.prev_anim_kind != nullptr) {
		this->prev_anim_kind = anim_table.get_anim(other.prev_anim_kind->name, false);
	}
	else {
		this->prev_anim_kind = nullptr;
	}
	this->prev_anim_offset = other.prev_anim_offset;
	this->frame = other.frame;
	this->rate = other.rate;
	this->prev_anim_frame = other.prev_anim_frame;
	this->prev_anim_rate = other.prev_anim_rate;
	this->is_anim_end = other.is_anim_end;
	this->shader = other.shader;
	this->shadow_shader = other.shadow_shader;
	this->outline_shader = other.outline_shader;
	this->model = other.model;

	return *this;
}

GameObject& GameObject::operator=(const GameObject& other) {
	if (this == &other) {
		return *this;
	}
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	shader_manager = ShaderManager::get_instance();
	sound_manager = SoundManager::get_instance();

	this->id_effect = other.id_effect;
	this->resource_dir = other.resource_dir;
	this->pos = other.pos;
	this->rot = other.rot;
	this->scale = other.scale;
	this->extra_mat = other.extra_mat;
	this->anim_table = other.anim_table;
	if (other.anim_kind != nullptr) {
		this->anim_kind = anim_table.get_anim(other.anim_kind->name, false);
	}
	else {
		this->anim_kind = nullptr;
	}
	if (other.prev_anim_kind != nullptr) {
		this->prev_anim_kind = anim_table.get_anim(other.prev_anim_kind->name, false);
	}
	else {
		this->prev_anim_kind = nullptr;
	}
	this->prev_anim_offset = other.prev_anim_offset;
	this->frame = other.frame;
	this->rate = other.rate;
	this->prev_anim_frame = other.prev_anim_frame;
	this->prev_anim_rate = other.prev_anim_rate;
	this->is_anim_end = other.is_anim_end;
	this->shader = other.shader;
	this->shadow_shader = other.shadow_shader;
	this->outline_shader = other.outline_shader;
	this->model = other.model;

	return *this;
}

void GameObject::process_sound() {
	ALint state;
	for (std::list<unsigned int>::iterator source = sound_effects.begin(), max = sound_effects.end(); source != max; source++) {
		alGetSourcei(*source, AL_SOURCE_STATE, &state);
		if (state == AL_STOPPED) {
			alDeleteSources(1, &(*source));
			if (sound_effects.size() != 1) {
				source = sound_effects.erase(source);
			}
			else {
				sound_effects.erase(source);
			}
		}
	}
	for (std::list<unsigned int>::iterator source = voice_clips.begin(), max = voice_clips.end(); source != max; source++) {
		alGetSourcei(*source, AL_SOURCE_STATE, &state);
		if (state == AL_STOPPED) {
			alDeleteSources(1, &(*source));
			if (voice_clips.size() != 1) {
				source = voice_clips.erase(source);
			}
			else {
				voice_clips.erase(source);
			}
		}
	}
}