#include "Framebuffer.h"
#include "RenderManager.h"
#include "SaveManager.h"
#include "utils.h"

TextureInfo::TextureInfo() {}

TextureInfo::TextureInfo(GLuint texture, GLenum internal_format, GLenum format, GLenum type) {
	this->texture = texture;
	this->internal_format = internal_format;
	this->format = format;
	this->type = type;
}

Framebuffer::Framebuffer() {

}

void Framebuffer::init(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir) {
	float coords[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader.init(vertex_dir, fragment_dir, geometry_dir);
	shader.use();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(coords), &coords, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Framebuffer::add_write_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, GLenum attachment_point) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_point, GL_TEXTURE_2D, texture, 0);
	textures.push_back(texture);
	attachment_points.push_back(attachment_point);
	resize_textures.push_back(TextureInfo(texture, internal_format, format, type));

	GLenum* attachments = new GLenum[attachment_points.size()];
	for (int i = 0, max = attachment_points.size(); i < max; i++) {
		attachments[i] = attachment_points[i];
	}
	glDrawBuffers(attachment_points.size(), attachments);
	delete[] attachments;
}

void Framebuffer::add_write_texture(GLuint texture, GLenum attachment_point) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_point, GL_TEXTURE_2D, texture, 0);
	textures.push_back(texture);
	GLenum* attachments = new GLenum[attachment_points.size()];
	for (int i = 0, max = attachment_points.size(); i < max; i++) {
		attachments[i] = attachment_points[i];
	}
	glDrawBuffers(attachment_points.size(), attachments);
	delete[] attachments;
}

void Framebuffer::add_read_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, void* source) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp);
	textures.push_back(texture);
}

void Framebuffer::add_read_texture(GLuint texture) {
	textures.push_back(texture);
}

void Framebuffer::destroy() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);
}

void Framebuffer::use() {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void Framebuffer::render() {
	glDepthMask(GL_FALSE);
	shader.use();
	for (int i = 0, max = textures.size(); i < max; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDepthMask(GL_TRUE);
}

void Framebuffer::update_dimensions() {
	RenderManager* render_manager = RenderManager::get_instance();
	float width = render_manager->s_window_width;
	float height = render_manager->s_window_height;
	for (int i = 0, max = resize_textures.size(); i < max; i++) {
		glBindTexture(GL_TEXTURE_2D, resize_textures[i].texture);
		glTexImage2D(GL_TEXTURE_2D, 0, resize_textures[i].internal_format, width, height, 0, resize_textures[i].format, resize_textures[i].type, nullptr);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}
