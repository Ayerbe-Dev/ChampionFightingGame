#pragma once
#include <iostream>
#include <glew/glew.h>
#include "Font.h"
#include "TextureCommon.h"
#include "TargetStructs.h"

class WorldText {
public:
	WorldText();
	WorldText(Font* font, std::string text, TextSpecifier spec);
	WorldText(WorldText& that);
	WorldText(WorldText&& that) noexcept;
	WorldText& operator=(WorldText& that);
	WorldText& operator=(WorldText&& that) noexcept;
	~WorldText();


	WorldText& init(Font* font, std::string text, TextSpecifier spec);
	void destroy();
	WorldText& set_shader(std::string frag_shader);
	WorldText& set_pause(bool pause);

	WorldText& update_text(std::string new_text);
	WorldText& start_scroll(int frames);

	WorldText& set_orientation(int orientation);
	int get_texture_orientation() const;

	WorldText& set_pos(glm::vec3 pos);
	WorldText& set_pos(glm::vec3 pos, int frames);
	WorldText& add_pos(glm::vec3 pos);
	glm::vec3 get_pos() const;

	WorldText& set_rot(glm::vec3 rot);
	WorldText& set_rot(glm::vec3 rot, int frames);
	WorldText& add_rot(glm::vec3 rot);
	glm::vec3 get_rot() const;

	WorldText& set_width(int new_width);
	WorldText& set_width(int new_width, int frames);
	WorldText& add_width(int width);
	int get_width() const;

	WorldText& set_height(int new_height);
	WorldText& set_height(int new_height, int frames);
	WorldText& add_height(int height);
	int get_height() const;

	WorldText& set_scale(glm::vec3 scale);
	WorldText& set_scale(glm::vec3 scale, int frames);
	WorldText& add_scale(glm::vec3 scale);
	glm::vec3 get_scale() const;

	WorldText& set_billboard_setting(int billboard_setting);
	int get_billboard_setting() const;

	void render();
private:
	void set_default_vertex_data();
	void update_buffer_data();
#ifdef TEX_IMPL_MODE_VULKAN

#else
	unsigned int VAO;
	unsigned int VBO;
#endif
	std::vector<TargetVec2> v_pos;
	std::vector<TargetVec2> v_texcoord;
	std::vector<TextureCoord> v_data_for_gpu;

	unsigned int texture;
	unsigned int num_lines;
	Shader* shader;
	std::string text;
	Font* font;
	TargetVar<float> scroll;

	int texture_orientation;

	TargetVar<glm::vec3> pos;
	TargetVar<glm::vec3> rot;
	TargetVar<glm::vec3> scale;

	TargetVar<int> width;
	TargetVar<int> height;

	int billboard_setting;

	TextSpecifier spec;
	bool loaded;
};