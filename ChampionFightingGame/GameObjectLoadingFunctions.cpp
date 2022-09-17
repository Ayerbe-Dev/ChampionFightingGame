#include "GameObject.h"
#include "RenderManager.h"

void GameObject::load_model(std::string resource_dir, std::string texture_dir) {
	this->resource_dir = resource_dir;
	model.load_model(resource_dir);
	if (texture_dir != "") {
		model.load_textures(texture_dir);
	}
}

void GameObject::init_shader() {
	shader.init("vertex_main.glsl", "fragment_main.glsl", "geometry_main.glsl");
	shadow_shader.init("vertex_shadow.glsl", "fragment_shadow.glsl");
	outline_shader.init("vertex_outline.glsl", "fragment_outline.glsl", "geometry_outline.glsl");
	shader.use();
	shader.set_int("shadow_map", 0);
	shader.set_int("material.diffuse", 1);
	shader.set_int("material.specular", 2);
	shader.set_float("brightness_mul", 1.0);
	shader.set_bool("has_skeleton", model.has_skeleton);
	shadow_shader.use();
	shadow_shader.set_bool("has_skeleton", model.has_skeleton);
	outline_shader.use();
	outline_shader.set_bool("has_skeleton", model.has_skeleton);
	render_manager->link_shader(&shader);
	render_manager->link_shader(&shadow_shader);
	render_manager->link_shader(&outline_shader);
}