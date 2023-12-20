#include "GameObject.h"
#include "WindowConstants.h"

void GameObject::render() {
	shader->use();
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, render_pos);
	model_mat *= glm::orientate4(rot);
	model_mat = glm::scale(model_mat, scale);
	model_mat *= extra_mat;
	shader->set_mat4("model_matrix", model_mat);
	model.render(shader);
}

void GameObject::render_shadow() {
	shadow_shader->use();
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, render_pos);
	model_mat *= glm::orientate4(rot);
	model_mat = glm::scale(model_mat, scale);
	model_mat *= extra_mat;
	shadow_shader->set_mat4("model_matrix", model_mat);
	model.render_no_texture(shadow_shader);
}

void GameObject::render_outline() {
	outline_shader->use();
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, render_pos);
	model_mat *= glm::orientate4(rot);
	model_mat = glm::scale(model_mat, scale);
	model_mat *= extra_mat;
	outline_shader->set_mat4("model_matrix", model_mat);
	model.render_no_texture(outline_shader);
}