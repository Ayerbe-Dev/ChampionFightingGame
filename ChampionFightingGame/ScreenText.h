#pragma once
#include "SceneElementBase.h"
#include <iostream>
#include <glew/glew.h>
#include "Font.h"
#include "TargetStructs.h"

class ScreenText {
public:
	ScreenText();
	ScreenText(Font* font, std::string text, TextSpecifier spec);

	ScreenText(ScreenText& that);
	ScreenText(const ScreenText& that);
	ScreenText(ScreenText&& that) noexcept;
	ScreenText& operator=(ScreenText& that);
	ScreenText& operator=(const ScreenText& that);
	ScreenText& operator=(ScreenText&& that) noexcept;
	~ScreenText();

	ScreenText&& init(Font* font, std::string text, TextSpecifier spec);
	void destroy();
	ScreenText&& set_shader(std::string frag_shader);
	ScreenText&& set_pause(bool persistence);

	ScreenText&& update_text(std::string new_text);
	std::string get_text() const;
	ScreenText&& start_scroll(int frames);

	ScreenText&& set_screen_orientation(int orientation);
	ScreenText&& set_texture_orientation(int orientation);
	ScreenText&& set_orientation(int orientation);
	int get_screen_orientation() const;
	int get_texture_orientation() const;

	ScreenText&& set_anchor(TextureAnchor* anchor);

	ScreenText&& set_pos(glm::vec3 pos);
	ScreenText&& set_pos(glm::vec3 pos, int frames);
	ScreenText&& add_pos(glm::vec3 pos);
	glm::vec3 get_pos() const;
	TargetVar<glm::vec3> get_pos_target() const;

	ScreenText&& set_rot(glm::vec3 rot);
	ScreenText&& set_rot(glm::vec3 rot, int frames);
	ScreenText&& add_rot(glm::vec3 rot);
	glm::vec3 get_rot() const;

	ScreenText&& set_magnitude(glm::vec3 magnitude);
	ScreenText&& set_magnitude(glm::vec3 magnitude, int frames);
	ScreenText&& add_magnitude(glm::vec3 magnitude);
	glm::vec3 get_magnitude() const;

	ScreenText&& set_base_width(int new_width);
	ScreenText&& set_base_width(int new_width, int frames);
	ScreenText&& add_base_width(int width);
	int get_base_width() const;

	ScreenText&& set_base_height(int new_height);
	ScreenText&& set_base_height(int new_height, int frames);
	ScreenText&& add_base_height(int height);
	int get_base_height() const;

	float get_width() const;
	float get_height() const;

	ScreenText&& set_width_scale(float scale);
	ScreenText&& set_width_scale(float scale, int frames);
	ScreenText&& add_width_scale(float scale);
	float get_width_scale() const;

	ScreenText&& set_height_scale(float scale);
	ScreenText&& set_height_scale(float scale, int frames);
	ScreenText&& add_height_scale(float scale);
	float get_height_scale() const;

	ScreenText&& set_scale(float scale);
	ScreenText&& set_scale(float scale, int frames);

	ScreenText&& set_alpha(unsigned char alpha);
	ScreenText&& set_alpha(unsigned char alpha, int frames);
	ScreenText&& add_alpha(unsigned char alpha);
	unsigned char get_alpha() const;

	ScreenText&& set_color(glm::vec3 color);
	ScreenText&& add_color(glm::vec3 color);
	glm::vec3 get_color() const;

	void render();

private:
	void set_default_vertex_data();
	void update_buffer_data();

#ifdef TEX_IMPL_MODE_VULKAN
	//TODO: However Fez handles Vulkan vert info, plop it here
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
	int screen_orientation;
	int texture_orientation;

	TextureAnchor* anchor;

	TargetVar<glm::vec3> pos;
	TargetVar<glm::vec3> rot;
	TargetVar<glm::vec3> magnitude;
	TargetVar<unsigned char> alpha;

	TargetVar<int> base_width;
	TargetVar<int> base_height;
	TargetVar<float> width_scale;
	TargetVar<float> height_scale;
	TextSpecifier spec;
	int buffer_updates;
	bool loaded;
};