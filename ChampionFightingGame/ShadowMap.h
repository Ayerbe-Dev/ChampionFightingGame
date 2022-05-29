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

	float shadow_map_depth = 7.0f;
	float shadow_map_fov = 5.0f;

	unsigned int FBO = 0;
	unsigned int shadow_texture = 0;

	glm::mat4 m_orthographic_perspective;
	glm::mat4 m_lookat;
	glm::vec3 m_light_position;
};