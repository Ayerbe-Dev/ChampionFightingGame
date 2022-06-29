#pragma once

#include <stdio.h>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap {
public:
	ShadowMap();

	void init();

	void update_light_pos();

	int SHADOW_WIDTH;
	int SHADOW_HEIGHT;

	float depth;
	float fov;

	unsigned int FBO = 0;
	unsigned int shadow_texture = 0;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::vec3 light_pos;
};