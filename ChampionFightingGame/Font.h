#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <iostream>

class Shader;

struct TexChar {
	unsigned int texture;
	glm::vec2 size;
	glm::vec2 bearing;
	unsigned int advance;
};

class Font {
public:
	Font();
	void unload_font();

	unsigned int create_text(std::string text, glm::vec4 rgba, float border_x = 0, float border_y = 0, unsigned int* texture = nullptr);
	void write_to_fbo(std::string text, float x_offset, float y_offset, float width, float height);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int FBO;
	unsigned int RBO;
	Shader *shader;

	float prev_width;
	float prev_height;
	float base_height;
	float base_y_offset;

	std::unordered_map<char, TexChar> char_map;
};