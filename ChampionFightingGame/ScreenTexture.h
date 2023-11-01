#pragma once
#include <vector>
#include <iostream>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "TextureCoord.h"
#include "TargetVar.h"

class ScreenTexture {
public:
	ScreenTexture();
	ScreenTexture(std::string path);
	ScreenTexture(std::vector<std::vector<char>> texture, int width = -1, int height = -1);
	ScreenTexture(ScreenTexture& that);
	ScreenTexture(ScreenTexture&& that) noexcept;
	ScreenTexture& operator=(ScreenTexture& that);
	ScreenTexture& operator=(ScreenTexture&& that) noexcept;
	~ScreenTexture();


	void init(std::string path);
	void init(std::vector<std::vector<char>> texture, int width = -1, int height = -1);
	void destroy();
	ScreenTexture init_copy();

	std::string get_path() const;

	void set_orientation(int screen_orientation, int texture_orientation);
	int get_screen_orientation() const;
	int get_texture_orientation() const;

	void set_pos(glm::vec3 pos);
	void set_pos(glm::vec3 pos, int frames);
	void add_pos(glm::vec3 pos);
	glm::vec3 get_pos() const;

	void set_rot(glm::vec3 rot);
	void set_rot(glm::vec3 rot, int frames);
	void add_rot(glm::vec3 rot);
	glm::vec3 get_rot() const;

	void scale_right_edge(float percent);
	void scale_right_edge(float percent, int frames);
	void crop_right_edge(float percent);
	void crop_right_edge(float percent, int frames);
	void scale_left_edge(float percent);
	void scale_left_edge(float percent, int frames);
	void crop_left_edge(float percent);
	void crop_left_edge(float percent, int frames);
	void scale_top_edge(float percent);
	void scale_top_edge(float percent, int frames);
	void crop_top_edge(float percent);
	void crop_top_edge(float percent, int frames);
	void scale_bottom_edge(float percent);
	void scale_bottom_edge(float percent, int frames);
	void crop_bottom_edge(float percent);
	void crop_bottom_edge(float percent, int frames);

	void set_width(int new_width);
	void set_width(int new_width, int frames);
	void set_width_scale(float scale);
	void set_width_scale(float scale, int frames);
	void set_height(int new_height);
	void set_height(int new_height, int frames);
	void set_height_scale(float scale);
	void set_height_scale(float scale, int frames);

	int get_base_width() const;
	int get_base_height() const;
	float get_width_scale() const;
	float get_height_scale() const;
	float get_width();
	float get_height();

	void set_alpha(unsigned char alpha);
	void set_alpha(unsigned char alpha, int frames);
	void add_alpha(unsigned char alpha);
	unsigned char get_alpha() const;

	void set_colormod(glm::vec3 color);
	void set_colormod(glm::vec3 color, int frames);
	void add_colormod(glm::vec3 color);
	glm::vec3 get_colormod() const;

	void flip_h();
	void set_h_flipped(bool h_flipped);
	bool is_h_flipped() const;
	void flip_v();
	void set_v_flipped(bool v_flipped);
	bool is_v_flipped() const;

	void set_sprite(unsigned int sprite);
	void next_sprite();
	void prev_sprite();
	unsigned int get_sprite() const;

	void render();
private:
	std::vector<std::vector<char>> texture;
	std::string path;

	int screen_orientation;
	int texture_orientation;

	TargetVar<glm::vec3> pos;
	TargetVar<glm::vec3> rot;
	TargetVar<float> right_edge_scale;
	TargetVar<float> right_edge_crop;
	TargetVar<float> left_edge_scale;
	TargetVar<float> left_edge_crop;
	TargetVar<float> top_edge_scale;
	TargetVar<float> top_edge_crop;
	TargetVar<float> bottom_edge_scale;
	TargetVar<float> bottom_edge_crop;

	TargetVar<int> width;
	TargetVar<int> height;
	TargetVar<float> width_scale;
	TargetVar<float> height_scale;

	TargetVar<unsigned char> alpha;
	TargetVar<glm::vec3> colormod;

	bool h_flipped;
	bool v_flipped;

	unsigned int sprite;

	bool loaded;
};