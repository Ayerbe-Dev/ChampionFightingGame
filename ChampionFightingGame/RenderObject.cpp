#include "RenderObject.h"
#include "RenderManager.h"
#include "utils.h"

RenderObject::RenderObject() {
	render_manager = RenderManager::get_instance();
}

RenderObject::RenderObject(Model model, Shader shader) {
	this->model = model;
	this->shader = shader;
}

void RenderObject::render(bool flip) {
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

void RenderObject::render_shadow(bool flip) {
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

void RenderObject::render_outline(bool flip) {
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
	model_mat = glm::scale(model_mat, scale * glm::vec3(1.02)); 
	outline_shader.set_mat4("model_matrix", model_mat);
	model.render_no_texture(&outline_shader, flip);
}