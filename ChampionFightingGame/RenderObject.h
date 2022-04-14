#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"

class RenderManager;

class RenderObject {
public:
	RenderObject();
	RenderObject(Model model, Shader shader);

	void render();
	void render_shadow();

	glm::vec3 pos = glm::vec3(0.0);
	glm::vec3 rot = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
	glm::mat4 extra_mat = glm::mat4(1.0);

	Shader shader;
	Model model;

	RenderManager* render_manager;
};