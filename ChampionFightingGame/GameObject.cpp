#include "GameObject.h"
#include "RenderManager.h"
#include "EffectManager.h"
#include "SoundManager.h"
#include "utils.h"

GameObject::GameObject() {
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
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
	sound_manager = SoundManager::get_instance();
	this->id_effect = other.id_effect;
	this->id_sound = other.id_sound;
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
	if (!glIsShader(shader.id)) {
		std::cout << "Shader " << shader.id << " deleted!\n";
	}
	for (int i = 0; i < render_manager->linked_shaders.size(); i++) {
		if (render_manager->linked_shaders[i] == &other.shader) {
			render_manager->linked_shaders[i] = &shader;
		}
		if (render_manager->linked_shaders[i] == &other.shadow_shader) {
			render_manager->linked_shaders[i] = &shadow_shader;
		}
		if (render_manager->linked_shaders[i] == &other.outline_shader) {
			render_manager->linked_shaders[i] = &outline_shader;
		}
	}
	this->model = other.model;
}

GameObject::GameObject(const GameObject& other) {
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	sound_manager = SoundManager::get_instance();
	this->id_effect = other.id_effect;
	this->id_sound = other.id_sound;
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
	for (int i = 0; i < render_manager->linked_shaders.size(); i++) {
		if (render_manager->linked_shaders[i] == &other.shader) {
			render_manager->linked_shaders[i] = &shader;
		}
		if (render_manager->linked_shaders[i] == &other.shadow_shader) {
			render_manager->linked_shaders[i] = &shadow_shader;
		}
		if (render_manager->linked_shaders[i] == &other.outline_shader) {
			render_manager->linked_shaders[i] = &outline_shader;
		}
	}
	this->model = other.model;
}

GameObject& GameObject::operator=(GameObject& other) {
	if (this == &other) {
		return *this;
	}
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	sound_manager = SoundManager::get_instance();

	this->id_effect = other.id_effect;
	this->id_sound = other.id_sound;
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
	for (int i = 0; i < render_manager->linked_shaders.size(); i++) {
		if (render_manager->linked_shaders[i] == &other.shader) {
			render_manager->linked_shaders[i] = &shader;
		}
		if (render_manager->linked_shaders[i] == &other.shadow_shader) {
			render_manager->linked_shaders[i] = &shadow_shader;
		}
		if (render_manager->linked_shaders[i] == &other.outline_shader) {
			render_manager->linked_shaders[i] = &outline_shader;
		}
	}
	this->model = other.model;

	return *this;
}

GameObject& GameObject::operator=(const GameObject& other) {
	if (this == &other) {
		return *this;
	}
	render_manager = RenderManager::get_instance();
	effect_manager = EffectManager::get_instance();
	sound_manager = SoundManager::get_instance();

	this->id_effect = other.id_effect;
	this->id_sound = other.id_sound;
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
	for (int i = 0; i < render_manager->linked_shaders.size(); i++) {
		if (render_manager->linked_shaders[i] == &other.shader) {
			render_manager->linked_shaders[i] = &shader;
		}
		if (render_manager->linked_shaders[i] == &other.shadow_shader) {
			render_manager->linked_shaders[i] = &shadow_shader;
		}
		if (render_manager->linked_shaders[i] == &other.outline_shader) {
			render_manager->linked_shaders[i] = &outline_shader;
		}
	}
	this->model = other.model;

	return *this;
}

void GameObject::render(bool flip) {
	shader.use();
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat,
		pos / glm::vec3(
			WINDOW_WIDTH / (100 * scale.x),
			WINDOW_HEIGHT / (100 * scale.y),
			WINDOW_DEPTH / (100 * scale.z)
		)
	);
	model_mat *= glm::orientate4(rot);
	model_mat = glm::scale(model_mat, scale);
	model_mat *= extra_mat;
	shader.set_mat4("model_matrix", model_mat);
	model.render(&shader, flip);
}

void GameObject::render_shadow(bool flip) {
	shadow_shader.use();
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat,
		pos / glm::vec3(
			WINDOW_WIDTH / (100 * scale.x),
			WINDOW_HEIGHT / (100 * scale.y),
			WINDOW_DEPTH / (100 * scale.z)
		)
	);
	model_mat *= glm::orientate4(rot);
	model_mat = glm::scale(model_mat, scale);
	model_mat *= extra_mat;
	shadow_shader.set_mat4("model_matrix", model_mat);
	model.render_no_texture(&shadow_shader, flip);
}

void GameObject::render_outline(bool flip) {
	outline_shader.use();
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat,
		pos / glm::vec3(
			WINDOW_WIDTH / (100 * scale.x),
			WINDOW_HEIGHT / (100 * scale.y),
			WINDOW_DEPTH / (100 * scale.z)
		)
	);
	model_mat *= glm::orientate4(rot);
	model_mat *= extra_mat;
	model_mat = glm::scale(model_mat, scale); 
	outline_shader.set_mat4("model_matrix", model_mat);
	model.render_no_texture(&outline_shader, flip);
}