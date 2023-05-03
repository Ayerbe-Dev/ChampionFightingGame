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

void GameObject::load_used_model(std::string resource_dir, std::string texture_dir) {
	this->resource_dir = resource_dir;
	model.load_used_model(resource_dir);
	if (texture_dir != "") {
		model.load_used_textures(texture_dir);
	}
}

void GameObject::init_shader() {
	unsigned int flags = 0;
	if (model.has_skeleton) {
		flags |= SHADER_FEAT_HAS_BONES;
	}
	shader = shader_manager->get_shader("model", "model", "model", flags);
	shadow_shader = shader_manager->get_shader("shadow", "shadow", "", flags);
	outline_shader = shader_manager->get_shader("3d_outline", "3d_outline", "3d_outline", flags);
	shader->use();
	shader->set_int("shadow_map", 0);
	shader->set_int("material.diffuse", 1);
	shader->set_int("material.specular", 2);
}