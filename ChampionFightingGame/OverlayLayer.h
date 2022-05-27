#pragma once
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"

//Essentially, an Overlay Layer is designed to be an FBO that can be blended
class OverlayLayer {
public:
	OverlayLayer();
	~OverlayLayer();

	void render(float alpha);

	GLuint VAO;
	GLuint VBO;
	GLuint FBO;
	GLuint RBO;
	GLuint texture;
	Shader shader;
};