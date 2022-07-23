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
	
	void add_write_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, GLenum attachment_point, int active_index, bool resize = true);
	void add_write_texture(GLuint texture, GLenum attachment_point, int active_index);
	void add_read_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, int active_index, void* source = nullptr);
	void add_read_texture(GLuint texture, int active_index);

	void destroy();
	void use();
	void bind_textures();
	void render();
	void render_passthrough();
	void update_dimensions();

	GLuint VAO;
	GLuint VBO;
	GLuint FBO;
	GLuint RBO;

	std::vector<GLuint> textures;
	std::vector<TextureInfo> resize_textures;
	std::vector<GLenum> attachment_points;
	std::vector<int> active_indices;

	Shader shader;
};