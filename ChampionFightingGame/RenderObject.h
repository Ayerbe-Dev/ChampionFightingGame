#pragma once
#include <vector>
#include "glm.hpp"
#include "Shader.h"
#include "Model.h"

using namespace glm;

class RenderObject {
public:
	RenderObject();
	RenderObject(Model model, Shader shader);

	void render();

	vec3 pos = vec3(0.0);
	vec3 rot = vec3(0.0);
	vec3 scale = vec3(1.0);
	mat4 extra_mat = mat4(1.0);

	Shader shader;
	Model model;
};