#include "GameObject.h"
#include "WindowConstants.h"
#include "WindowManager.h"

bool GameObject::is_in_camera_range() {
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::scale(model_mat, scale);
	model_mat = glm::translate(model_mat, pos / scale_vec);
	model_mat *= glm::orientate4(rot);
	model_mat *= extra_mat;
	Camera &camera = window_manager->camera;
	float camera_x = (camera.camera_matrix * model_mat * glm::vec4(0.0, 0.0, 0.0, 1.0)).x / camera.pos.z;
	return camera_x < 1.0 && camera_x > -1.0;
}

void GameObject::set_alpha(unsigned char alpha) {
	model.set_alpha(alpha);
}

void GameObject::set_alpha(unsigned char alpha, int frames) {
	model.set_alpha(alpha, frames);
}

void GameObject::add_alpha(short alpha) {
	model.add_alpha(alpha);
}

unsigned char GameObject::get_alpha() const {
	return model.get_alpha();
}

void GameObject::render() {
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::scale(model_mat, scale);
	model_mat = glm::translate(model_mat, pos / scale_vec);
	model_mat *= glm::orientate4(rot);
	model_mat *= extra_mat;
	shader->use();
	shader->set_mat4("model_matrix", model_mat);
	model.render(shader);
}

void GameObject::render_shadow() {
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::scale(model_mat, scale);
	model_mat = glm::translate(model_mat, pos / scale_vec);
	model_mat *= glm::orientate4(rot);
	model_mat *= extra_mat;
	shadow_shader->use();
	shadow_shader->set_mat4("model_matrix", model_mat);
	model.render_no_texture(shadow_shader);
}