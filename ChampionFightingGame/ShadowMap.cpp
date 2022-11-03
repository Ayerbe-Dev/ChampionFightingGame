#include <iostream>
#include "ShadowMap.h"
#include "RenderManager.h"

ShadowMap::ShadowMap() {
	depth = 25.0;
	fov = 100.0;

	light_pos = glm::vec3(0.0, 1.0, 1.0);
	projection_matrix = glm::ortho(-fov, fov, -fov, fov, 0.1f, depth);
	view_matrix = glm::lookAt(light_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowMap::update_light_pos() {
	projection_matrix = glm::ortho(-fov, fov, -fov, fov, 0.1f, depth);
	view_matrix = glm::lookAt(light_pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	RenderManager::get_instance()->update_shader_shadows();
}
