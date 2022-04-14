#include "RenderObject.h"
#include "RenderManager.h"

RenderObject::RenderObject() {
	render_manager = RenderManager::get_instance();
}

RenderObject::RenderObject(Model model, Shader shader) {
	this->model = model;
	this->shader = shader;
}

void RenderObject::render() {
	render_manager->render_model(&model, &shader, extra_mat, &pos, &rot, &scale);
}

void RenderObject::render_shadow() {
	render_manager->render_model_shadow(&model, extra_mat, &pos, &rot, &scale);
}