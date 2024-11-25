#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <iostream>

struct TextSpecifier {
	TextSpecifier() {
		rgba = glm::vec4(255.0, 255.0, 255.0, 255.0);
		border_rgbs = glm::vec4(0.0, 0.0, 0.0, 0.0);
		enable_multiline_scroll = false;
		enable_center = false;
		max_line_length = UINT32_MAX;
	}

	TextSpecifier& color(glm::vec3 col) {
		rgba.r = col.r;
		rgba.g = col.g;
		rgba.b = col.b;
		return *this;
	}

	TextSpecifier& alpha(float alpha) {
		rgba.a = alpha;
		return *this;
	}

	TextSpecifier& border(glm::vec4 rgbs) {
		border_rgbs.r = rgbs.r;
		border_rgbs.g = rgbs.g;
		border_rgbs.b = rgbs.b;
		border_rgbs.a = rgbs.a;
		return *this;
	}

	TextSpecifier& border(glm::vec3 col) {
		border_rgbs.r = col.r;
		border_rgbs.g = col.g;
		border_rgbs.b = col.b;
		return *this;
	}

	TextSpecifier& border(int size) {
		border_rgbs.a = size;
		return *this;
	}

	TextSpecifier& multiline_scroll(bool enable_multiline_scroll) {
		this->enable_multiline_scroll = enable_multiline_scroll;
		return *this;
	}

	TextSpecifier& line_length(unsigned int length) {
		max_line_length = length;
		return *this;
	}

	TextSpecifier& centered(bool center) {
		enable_center = center;
		return *this;
	}

	glm::vec4 rgba;
	glm::vec4 border_rgbs;
	bool enable_multiline_scroll;
	bool enable_center;
	unsigned int max_line_length;
};
typedef TextSpecifier TxtSpec;

class Shader;

struct TexChar {
	TexChar() {
		texture = 0;
		size = glm::vec2(0.0);
		bearing = glm::vec2(0.0);
		advance = 0;
	}
	unsigned int texture;
	glm::vec2 size;
	glm::vec2 bearing;
	unsigned int advance;
};

class Font {
public:
	Font();
	void unload_font();

	unsigned int create_text(std::string text, glm::vec4 rgba, glm::vec4 border_rgbs, unsigned int* existing_texture = nullptr);
	unsigned int create_text(std::string text, glm::vec3 rgb, glm::vec4 border_rgbs, bool enable_center, 
		unsigned int max_line_length, unsigned int* num_lines, unsigned int* existing_texture);

	unsigned int VAO;
	unsigned int VBO;
	unsigned int VAO_outline;
	unsigned int VBO_outline;
	unsigned int FBO;
	unsigned int RBO;
	Shader* shader;
	Shader* border_shader;

	float prev_width;
	float prev_height;
	float base_height;
	float base_y_offset;

	std::unordered_map<char, TexChar> char_map;
};