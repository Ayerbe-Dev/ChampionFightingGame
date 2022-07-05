#pragma once
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"

struct TextureInfo {
	TextureInfo();
	TextureInfo(GLuint texture, GLenum internal_format, GLenum format, GLenum type);
	GLuint texture;
	GLenum internal_format;
	GLenum format;
	GLenum type; 
};

class Framebuffer {
public:
	Framebuffer();

	void init(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir = "");
	
	void add_write_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, GLenum attachment_point);
	void add_write_texture(GLuint texture, GLenum attachment_point);
	void add_read_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, void* source = nullptr);
	void add_read_texture(GLuint texture);

	void destroy();
	void use();
	void render();
	void update_dimensions();

	GLuint VAO;
	GLuint VBO;
	GLuint FBO;
	GLuint RBO;

	std::vector<GLuint> textures;
	std::vector<TextureInfo> resize_textures;
	std::vector<GLenum> attachment_points;

	Shader shader;
};