#pragma once
#include <vector>
#include <iostream>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "TextureCommon.h"
#include "TargetStructs.h"

class Shader;

class WorldTexture {
public:
	WorldTexture();
	WorldTexture(std::string path, unsigned char features);
	WorldTexture(std::vector<unsigned int> texture, unsigned char features, int width = -1, int height = -1);
	WorldTexture(WorldTexture& that);
	WorldTexture(WorldTexture&& that) noexcept;
	WorldTexture& operator=(WorldTexture& that);
	WorldTexture& operator=(WorldTexture&& that) noexcept;
	~WorldTexture();


	WorldTexture& init(std::string path, unsigned char features);
	WorldTexture& init(std::vector<unsigned int> texture, unsigned char features, int width = -1, int height = -1);
	void destroy();
	WorldTexture init_copy();
	WorldTexture& set_shader(std::string frag_shader);
	WorldTexture& set_features(unsigned char features);

	std::string get_path() const;

	WorldTexture& set_orientation(int orientation);
	int get_texture_orientation() const;

	WorldTexture& set_pos(glm::vec3 pos);
	WorldTexture& set_pos(glm::vec3 pos, int frames);
	WorldTexture& add_pos(glm::vec3 pos);
	glm::vec3 get_pos() const;

	WorldTexture& set_rot(glm::vec3 rot);
	WorldTexture& set_rot(glm::vec3 rot, int frames);
	WorldTexture& add_rot(glm::vec3 rot);
	glm::vec3 get_rot() const;

	WorldTexture& set_width(int new_width);
	WorldTexture& set_width(int new_width, int frames);
	WorldTexture& add_width(int width);
	int get_width() const;

	WorldTexture& set_height(int new_height);
	WorldTexture& set_height(int new_height, int frames);
	WorldTexture& add_height(int height);
	int get_height() const;

	WorldTexture& set_scale(glm::vec3 scale);
	WorldTexture& set_scale(glm::vec3 scale, int frames);
	WorldTexture& add_scale(glm::vec3 scale);
	glm::vec3 get_scale() const;

	WorldTexture& scale_right_edge(float percent);
	WorldTexture& scale_right_edge(float percent, int frames);
	WorldTexture& crop_right_edge(float percent);
	WorldTexture& crop_right_edge(float percent, int frames);
	WorldTexture& scale_left_edge(float percent);
	WorldTexture& scale_left_edge(float percent, int frames);
	WorldTexture& crop_left_edge(float percent);
	WorldTexture& crop_left_edge(float percent, int frames);
	WorldTexture& scale_top_edge(float percent);
	WorldTexture& scale_top_edge(float percent, int frames);
	WorldTexture& crop_top_edge(float percent);
	WorldTexture& crop_top_edge(float percent, int frames);
	WorldTexture& scale_bottom_edge(float percent);
	WorldTexture& scale_bottom_edge(float percent, int frames);
	WorldTexture& crop_bottom_edge(float percent);
	WorldTexture& crop_bottom_edge(float percent, int frames);

	WorldTexture& scale_top_right_corner(float percent_x, float percent_y);
	WorldTexture& scale_top_right_corner(float percent_x, float percent_y, int frames);
	WorldTexture& crop_top_right_corner(float percent_x, float percent_y);
	WorldTexture& crop_top_right_corner(float percent_x, float percent_y, int frames);
	WorldTexture& scale_top_left_corner(float percent_x, float percent_y);
	WorldTexture& scale_top_left_corner(float percent_x, float percent_y, int frames);
	WorldTexture& crop_top_left_corner(float percent_x, float percent_y);
	WorldTexture& crop_top_left_corner(float percent_x, float percent_y, int frames);
	WorldTexture& scale_bottom_right_corner(float percent_x, float percent_y);
	WorldTexture& scale_bottom_right_corner(float percent_x, float percent_y, int frames);
	WorldTexture& crop_bottom_right_corner(float percent_x, float percent_y);
	WorldTexture& crop_bottom_right_corner(float percent_x, float percent_y, int frames);
	WorldTexture& scale_bottom_left_corner(float percent_x, float percent_y);
	WorldTexture& scale_bottom_left_corner(float percent_x, float percent_y, int frames);
	WorldTexture& crop_bottom_left_corner(float percent_x, float percent_y);
	WorldTexture& crop_bottom_left_corner(float percent_x, float percent_y, int frames);

	WorldTexture& set_alpha(unsigned char alpha);
	WorldTexture& set_alpha(unsigned char alpha, int frames);
	WorldTexture& add_alpha(unsigned char alpha);
	unsigned char get_alpha() const;

	WorldTexture& set_colormod(glm::vec3 color);
	WorldTexture& set_colormod(glm::vec3 color, int frames);
	WorldTexture& add_colormod(glm::vec3 color);
	glm::vec3 get_colormod() const;

	WorldTexture& flip_h();
	WorldTexture& set_h_flipped(bool h_flipped);
	bool is_h_flipped() const;
	WorldTexture& flip_v();
	WorldTexture& set_v_flipped(bool v_flipped);
	bool is_v_flipped() const;
	int get_flipped() const;

	WorldTexture& set_billboard_setting(int billboard_setting);
	int get_billboard_setting() const;

	WorldTexture& set_sprite(unsigned int sprite);
	WorldTexture& next_sprite();
	WorldTexture& prev_sprite();
	unsigned int get_sprite() const;

	void render();
private:
	void set_default_vertex_data();
	void update_buffer_data();
#ifdef TEX_IMPL_MODE_VULKAN

#else
	unsigned int VAO;
	unsigned int VBO;
#endif
	VertexSpecifier v_spec;
	std::vector<TargetVec2> v_pos;
	std::vector<TargetVec2> v_texcoord;
	std::vector<TargetVec2*> v_pos_accessor;
	std::vector<TargetVec2*> v_texcoord_accessor;

	std::vector<TextureCoord> v_data_for_gpu;

	std::vector<unsigned int> texture;
	Shader* shader;
	unsigned char features;

	std::string path;

	int texture_orientation;

	TargetVar<glm::vec3> pos;
	TargetVar<glm::vec3> rot;
	TargetVar<glm::vec3> scale;

	TargetVar<int> width;
	TargetVar<int> height;

	TargetVar<unsigned char> alpha;
	TargetVar<glm::vec3> colormod;

	bool h_flipped;
	bool v_flipped;

	int billboard_setting;
	unsigned int sprite;

	bool loaded;
};