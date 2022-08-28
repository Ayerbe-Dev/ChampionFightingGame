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