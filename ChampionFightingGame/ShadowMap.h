#pragma once
#include "Framebuffer.h"
#include <stdio.h>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShadowMap : public Framebuffer {
public:
	ShadowMap();

	void update_light_pos();

	float depth;
	float fov;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
	glm::vec3 light_pos;
};