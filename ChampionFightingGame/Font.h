#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <iostream>

class Shader;

struct TexChar {
	unsigned int texture;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};

class Font {
public:
	Font();
	void unload_font();

	unsigned int create_text(std::string text, glm::vec4 rgba, int* width_ret, int* height_ret, unsigned int* texture = nullptr);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int FBO;
	unsigned int RBO;
	Shader *shader;

	std::unordered_map<char, TexChar> char_map;
};