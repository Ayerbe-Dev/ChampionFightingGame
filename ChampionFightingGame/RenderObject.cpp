#include "RenderObject.h"
#include "RenderManager.h"
extern RenderManager g_rendermanager;

RenderObject::RenderObject() {
}

RenderObject::RenderObject(Model model, Shader shader) {
	this->model = model;
	this->shader = shader;
}

void RenderObject::render() {
	g_rendermanager.render_model(&model, &shader, extra_mat, &pos, &rot, &scale);
}