#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"

class RenderManager;

class GameObject {
public:
	GameObject();
	GameObject(Model model, Shader shader);

	void render(bool flip = false);
	void render_shadow(bool flip = false);
	void render_outline(bool flip = false);

	glm::vec3 pos = glm::vec3(0.0);
	glm::vec3 rot = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
	glm::mat4 extra_mat = glm::mat4(1.0);

	Shader shader;
	Shader shadow_shader;
	Shader outline_shader;
	Model model;

	RenderManager* render_manager;
};