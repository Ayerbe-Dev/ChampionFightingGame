#pragma once
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"

struct TextureInfo {
	TextureInfo();
	TextureInfo(GLenum internal_format, GLenum format, GLenum type, void* source, bool no_resize);
	GLenum internal_format;
	GLenum format;
	GLenum type; 
	const void* source;
	bool no_resize;
};

class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();

	void init(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir = "");
	void add_texture(GLenum internal_format, GLenum format, GLenum type, GLenum clamp, float width, float height, void* source = nullptr, bool no_resize = false);
	void add_texture(GLuint texture, TextureInfo info);
	void destroy();
	void use();
	void render();
	void update_dimensions();

	GLuint VAO;
	GLuint VBO;
	GLuint FBO;
	GLuint RBO;
	std::vector<GLuint> textures;
	std::vector<TextureInfo> texture_info;
	GLuint g_position;
	GLuint g_normal;
	GLuint g_diffuse;
	GLuint g_specular;
	Shader shader;
};