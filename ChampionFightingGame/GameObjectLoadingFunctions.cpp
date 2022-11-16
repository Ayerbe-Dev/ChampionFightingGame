#include "GameObject.h"
#include "RenderManager.h"
#include "ShaderManager.h"

void GameObject::load_model(std::string resource_dir, std::string texture_dir) {
	this->resource_dir = resource_dir;
	model.load_model(resource_dir);
	if (texture_dir != "") {
		model.load_textures(texture_dir);
	}
}

void GameObject::init_shader() {
	shader = shader_manager->get_shader("main", "main", "main", 0);
	shadow_shader = shader_manager->get_shader("shadow", "shadow", "", 0);
	outline_shader = shader_manager->get_shader("outline", "outline", "outline", 0);
	shader->use();
	shader->set_int("shadow_map", 0);
	shader->set_int("material.diffuse", 1);
	shader->set_int("material.specular", 2);
	shader->set_float("brightness_mul", 1.0);
	shader->set_bool("has_skeleton", model.has_skeleton);
	shadow_shader->use();
	shadow_shader->set_bool("has_skeleton", model.has_skeleton);
	outline_shader->use();
	outline_shader->set_bool("has_skeleton", model.has_skeleton);
}