#pragma once

#include <stdio.h>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap {
public:
	ShadowMap();

	void init();

	void update_light();

	int SHADOW_WIDTH;
	int SHADOW_HEIGHT;

	float depth = 7.0f;
	float fov = 5.0f;

	unsigned int FBO = 0;
	unsigned int shadow_texture = 0;

	glm::mat4 perspective;
	glm::mat4 lookat;
	glm::vec3 light_pos;
};