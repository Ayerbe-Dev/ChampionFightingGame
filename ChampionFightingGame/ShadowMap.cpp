#include "ShadowMap.h"

ShadowMap::ShadowMap()
{
	glGenFramebuffers(1, &fbo_location);

	glGenTextures(1, &depth_map_location);

	glBindTexture(GL_TEXTURE_2D, depth_map_location);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_location);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_location, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	m_light_position = glm::vec3(0.0,1.0,1.0);
	m_orthographic_perspective = glm::ortho(-shadow_map_fov, shadow_map_fov, -shadow_map_fov, shadow_map_fov, 0.1f, shadow_map_depth);
	m_lookat = glm::lookAt(m_light_position,
							glm::vec3(0.0f, 0.0f, 0.0f),
							glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowMap::update_light()
{
	m_orthographic_perspective = glm::ortho(-shadow_map_fov, shadow_map_fov, -shadow_map_fov, shadow_map_fov, 0.1f, shadow_map_depth);
	m_lookat = glm::lookAt(m_light_position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
}
