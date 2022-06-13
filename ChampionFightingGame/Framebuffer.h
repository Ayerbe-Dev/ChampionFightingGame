#pragma once
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"

//Essentially, an Overlay Layer is designed to be an FBO that can be blended
class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();

	void init(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir = "");
	void destroy();
	void use();
	void render();

	GLuint VAO;
	GLuint VBO;
	GLuint FBO;
	GLuint RBO;
	GLuint g_position;
	GLuint g_normal;
	GLuint g_diffuse;
	GLuint g_specular;
	Shader shader;
};