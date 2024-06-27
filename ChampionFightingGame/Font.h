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

	unsigned int create_text(std::string text, glm::vec4 rgba, glm::vec4 border_rgbs, unsigned int* texture = nullptr);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int FBO;
	unsigned int RBO;
	Shader* shader;
	Shader* border_shader;

	float prev_width;
	float prev_height;
	float base_height;
	float base_y_offset;

	std::unordered_map<char, TexChar> char_map;

private:
	void write_to_fbo(std::string text, float x_offset, float y_offset, float width, float height);
};