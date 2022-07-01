#include "Framebuffer.h"
#include "RenderManager.h"
#include "SaveManager.h"
#include "utils.h"

TextureInfo::TextureInfo() {}

TextureInfo::TextureInfo(GLenum internal_format, GLenum format, GLenum type, void* source, bool no_resize) {
	this->internal_format = internal_format;
	this->format = format;
	this->type = type;
	this->source = source;
	this->no_resize = no_resize;
}

Framebuffer::Framebuffer() {
}

Framebuffer::~Framebuffer() {
	destroy();
}

void Framebuffer::init(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir) {
	SaveManager* save_manager = SaveManager::get_instance();
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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, save_manager->get_game_setting("res_x"), save_manager->get_game_setting("res_y"));
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

void Framebuffer::add_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, void* source, bool no_resize) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, source);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, clamp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, clamp);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textures.size(), GL_TEXTURE_2D, texture, 0);
	textures.push_back(texture);
	texture_info.push_back(TextureInfo(internal_format, format, type, source, no_resize));

	GLenum* attachments = new GLenum[textures.size()]; //I really hate cpp arrays
	for (int i = 0, max = textures.size(); i < max; i++) {
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(textures.size(), attachments); //Can't pass a vector here
	delete[] attachments;
}

void Framebuffer::add_texture(GLuint texture, TextureInfo info) {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textures.size(), GL_TEXTURE_2D, texture, 0);
	textures.push_back(texture);
	texture_info.push_back(info);
	GLenum* attachments = new GLenum[textures.size()];
	for (int i = 0, max = textures.size(); i < max; i++) {
		attachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	glDrawBuffers(textures.size(), attachments);
	delete[] attachments;
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
	for (int i = 0, max = textures.size(); i < max; i++) {
		if (!texture_info[i].no_resize) {
			glBindTexture(GL_TEXTURE_2D, textures[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, texture_info[i].internal_format, width, height, 0, texture_info[i].format, texture_info[i].type, texture_info[i].source);
		}
	}
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
}
