#include "ShadowMap.h"
#include <iostream>

ShadowMap::ShadowMap() {
	SHADOW_WIDTH = 2000;
	SHADOW_HEIGHT = 2000;

	m_light_position = glm::vec3(0.0, 1.0, 1.0);
	m_orthographic_perspective = glm::ortho(-shadow_map_fov, shadow_map_fov, -shadow_map_fov, shadow_map_fov, 0.1f, shadow_map_depth);
	m_lookat = glm::lookAt(m_light_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowMap::init() {
	glGenFramebuffers(1, &FBO);

	glGenTextures(1, &shadow_texture);

	glBindTexture(GL_TEXTURE_2D, shadow_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_texture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::update_light() {
	m_orthographic_perspective = glm::ortho(-shadow_map_fov, shadow_map_fov, -shadow_map_fov, shadow_map_fov, 0.1f, shadow_map_depth);
	m_lookat = glm::lookAt(m_light_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
