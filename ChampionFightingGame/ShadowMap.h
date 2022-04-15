#pragma once

#include <stdio.h>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class ShadowMap
{
public:
	ShadowMap();
	void update_light();

	const int SHADOW_WIDTH = 1024;
	const int SHADOW_HEIGHT = 1024;

	float shadow_map_depth = 7.0f, shadow_map_fov = 5.0f;

	GLuint fbo_location = 0;
	GLuint depth_map_location = 0;

	glm::mat4 m_orthographic_perspective;
	glm::mat4 m_lookat;
	glm::vec3 m_light_position;
};