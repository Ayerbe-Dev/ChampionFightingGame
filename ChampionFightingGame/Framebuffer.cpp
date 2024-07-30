#include "Framebuffer.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "SaveManager.h"
#include "utils.h"

TextureInfo::TextureInfo() {
	texture = 0;
	internal_format = 0;
	format = 0;
	type = 0;
}

TextureInfo::TextureInfo(GLuint texture, GLenum internal_format, GLenum format, GLenum type) {
	this->texture = texture;
	this->internal_format = internal_format;
	this->format = format;
	this->type = type;
}

Framebuffer::Framebuffer() {
	VAO = 0;
	VBO = 0;
	FBO = 0;
	RBO = 0;
	shader = nullptr;
}

void Framebuffer::init(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir, unsigned int features, float window_width, float window_height) {
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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window_width, window_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shader = ShaderManager::get_instance()->get_shader(vertex_dir, fragment_dir, geometry_dir, features);
	shader->use();

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

void Framebuffer::add_write_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, GLenum attachment_point, int active_index, bool resize) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_point, GL_TEXTURE_2D, texture, 0);
	textures.push_back(texture);
	attachment_points.push_back(attachment_point);
	if (resize) {
		resize_textures.push_back(TextureInfo(texture, internal_format, format, type));
	}
	active_indices.push_back(active_index);
	glDrawBuffers(attachment_points.size(), attachment_points.data());
}

void Framebuffer::add_write_texture(GLuint texture, GLenum attachment_point, int active_index) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_point, GL_TEXTURE_2D, texture, 0);
	textures.push_back(texture);
	active_indices.push_back(active_index);
	glDrawBuffers(attachment_points.size(), attachment_points.data());
}

void Framebuffer::add_read_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, int active_index, void* source) {
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
	active_indices.push_back(active_index);
}

void Framebuffer::add_read_texture(GLuint texture, int active_index) {
	textures.push_back(texture);
	active_indices.push_back(active_index);
}

void Framebuffer::add_uniform(std::string name, GLuint texture) {
	uniforms[name] = texture;
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

void Framebuffer::set_feats(unsigned int remove_feats, unsigned int add_feats) {
	shader = ShaderManager::get_instance()->get_shader_switch_features(shader, remove_feats, add_feats);
	RenderManager::get_instance()->update_shader_lights();
	bind_uniforms();
}

void Framebuffer::bind_textures() {
	for (int i = 0, max = textures.size(); i < max; i++) {
		glActiveTexture(GL_TEXTURE0 + active_indices[i]);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
	}
}

void Framebuffer::bind_uniforms() {
	shader->use();
	for (const auto& u : uniforms) {
		shader->set_int(u.first, u.second);
	}
}

void Framebuffer::bind_ex_uniforms(std::vector<std::pair<std::string, GLuint>> ex_textures) {
	shader->use();
	for (size_t i = 0, max = ex_textures.size(); i < max; i++) {
		shader->set_int(ex_textures[i].first, i + textures.size());
		glActiveTexture(GL_TEXTURE0 + textures.size());
		glBindTexture(GL_TEXTURE_2D, ex_textures[i].second);
	}
}

void Framebuffer::render() {
	glDepthMask(GL_FALSE);
	shader->use();
	bind_textures();

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDepthMask(GL_TRUE);
}

void Framebuffer::render_passthrough() {
	glDepthMask(GL_FALSE);
	ShaderManager::get_instance()->get_shader("passthrough", "passthrough", "", 0)->use();
	bind_textures();

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDepthMask(GL_TRUE);
}

void Framebuffer::update_dimensions(float x_scale, float y_scale) {
	RenderManager* render_manager = RenderManager::get_instance();
	float width = render_manager->res_width * x_scale;
	float height = render_manager->res_height * y_scale;
	for (int i = 0, max = resize_textures.size(); i < max; i++) {
		glBindTexture(GL_TEXTURE_2D, resize_textures[i].texture);
		glTexImage2D(GL_TEXTURE_2D, 0, resize_textures[i].internal_format, width, height, 0, resize_textures[i].format, resize_textures[i].type, nullptr);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, render_manager->window_width, render_manager->window_height);
}
