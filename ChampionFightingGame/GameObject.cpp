#include "GameObject.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"
#include "ObjectManager.h"
#include "utils.h"

GameObject::GameObject() {
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	shader_manager = ShaderManager::get_instance();
	sound_manager = SoundManager::get_instance();
	object_manager = ObjectManager::get_instance();
	object_manager->register_game_object(this);
	pos = glm::vec3(0.0);
	render_pos = glm::vec3(0.0);
	rot = glm::vec3(0.0);
	scale = glm::vec3(1.0);
	extra_mat = glm::mat4(1.0);
	resource_dir = "";
	anim_kind = nullptr;
	prev_anim_kind = nullptr;
	frame = 0.0;
	prev_anim_frame = 0.0;
	rate = 1.0;
	prev_anim_rate = 0.0;
	prev_anim_offset = glm::vec3(0.0);
	anim_end = false;
	sound_player.init(&render_pos);
	id_effect = -1;
	shader = nullptr;
	shadow_shader = nullptr;
	outline_shader = nullptr;
}

GameObject::GameObject(GameObject& other) {
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	shader_manager = ShaderManager::get_instance();
	sound_manager = SoundManager::get_instance();
	object_manager = ObjectManager::get_instance();
	object_manager->register_game_object(this);
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
	this->anim_end = other.anim_end;
	this->sound_player.init(&render_pos);
	this->shader = other.shader;
	this->shadow_shader = other.shadow_shader;
	this->outline_shader = other.outline_shader;
	this->model = other.model;
	this->anim_end = other.anim_end;
}

GameObject::GameObject(const GameObject& other) {
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	shader_manager = ShaderManager::get_instance();
	sound_manager = SoundManager::get_instance();
	object_manager = ObjectManager::get_instance();
	object_manager->register_game_object(this);
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
	this->anim_end = other.anim_end;
	this->sound_player.init(&render_pos);
	this->shader = other.shader;
	this->shadow_shader = other.shadow_shader;
	this->outline_shader = other.outline_shader;
	this->model = other.model;
	this->anim_end = other.anim_end;
}

GameObject& GameObject::operator=(GameObject& other) {
	if (this == &other) {
		return *this;
	}
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	shader_manager = ShaderManager::get_instance();
	sound_manager = SoundManager::get_instance();
	object_manager = ObjectManager::get_instance();
	object_manager->register_game_object(this);

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
	this->anim_end = other.anim_end;
	this->sound_player.init(&render_pos);
	this->shader = other.shader;
	this->shadow_shader = other.shadow_shader;
	this->outline_shader = other.outline_shader;
	this->model = other.model;
	this->anim_end = other.anim_end;
	
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
	object_manager = ObjectManager::get_instance();
	object_manager->register_game_object(this);

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
	this->anim_end = other.anim_end;
	this->sound_player.init(&render_pos);
	this->shader = other.shader;
	this->shadow_shader = other.shadow_shader;
	this->outline_shader = other.outline_shader;
	this->model = other.model;
	this->anim_end = other.anim_end;

	return *this;
}

GameObject::~GameObject() {
	object_manager->unregister_game_object(this);
}