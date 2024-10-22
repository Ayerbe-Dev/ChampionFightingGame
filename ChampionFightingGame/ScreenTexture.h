#pragma once
#include <vector>
#include <iostream>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "TextureCommon.h"
#include "TargetStructs.h"

class Shader;

/// <summary>
/// A rendering unit which operates in screen space. Instances of this class should usually be rendered 
/// directly to the screen rather than through a framebuffer.
/// </summary>
class ScreenTexture {
public:
	ScreenTexture();
	ScreenTexture(std::string path, unsigned char features);
	ScreenTexture(std::vector<unsigned int> texture, unsigned char features, int width = -1, int height = -1);

	ScreenTexture(ScreenTexture& that);
	ScreenTexture(ScreenTexture&& that) noexcept;
	ScreenTexture& operator=(ScreenTexture& that);
	ScreenTexture& operator=(ScreenTexture&& that) noexcept;
	~ScreenTexture();

	ScreenTexture& init(std::string path, unsigned char features);
	ScreenTexture& init(std::vector<unsigned int> texture, unsigned char features, int width = -1, int height = -1);
	void destroy();
	ScreenTexture init_copy() const;
	ScreenTexture& set_shader(std::string frag_shader);
	ScreenTexture& set_features(unsigned char features);

	std::string get_path() const;

	ScreenTexture& set_screen_orientation(int orientation);
	ScreenTexture& set_texture_orientation(int orientation);
	ScreenTexture& set_orientation(int screen_orientation, int texture_orientation);
	int get_screen_orientation() const;
	int get_texture_orientation() const;

	ScreenTexture& set_pos(glm::vec3 pos);
	ScreenTexture& set_pos(glm::vec3 pos, int frames);
	ScreenTexture& add_pos(glm::vec3 pos);
	glm::vec3 get_pos() const;

	ScreenTexture& set_rot(glm::vec3 rot);
	ScreenTexture& set_rot(glm::vec3 rot, int frames);
	ScreenTexture& add_rot(glm::vec3 rot);
	glm::vec3 get_rot() const;

	//TODO: Consider scrapping the "set_base_dimension" funcs and making them just "set_dimension" 
	//funcs which reset their respective scales to 1.0f. This would be an improvement for the set_ 
	//ones, but it's unclear what the behavior would be for the "add_" ones, I.E. would they add
	//(input) * dimension_scale, would they scale the base value itself before adding, etc.

	ScreenTexture& set_base_width(int new_width);
	ScreenTexture& set_base_width(int new_width, int frames);
	ScreenTexture& add_base_width(int width);
	int get_base_width() const;

	ScreenTexture& set_base_height(int new_height);
	ScreenTexture& set_base_height(int new_height, int frames);
	ScreenTexture& add_base_height(int height);
	int get_base_height() const;

	float get_width() const;
	float get_height() const;

	ScreenTexture& set_width_scale(float scale);
	ScreenTexture& set_width_scale(float scale, int frames);
	ScreenTexture& add_width_scale(float scale);
	float get_width_scale() const;

	ScreenTexture& set_height_scale(float scale);
	ScreenTexture& set_height_scale(float scale, int frames);
	ScreenTexture& add_height_scale(float scale);
	float get_height_scale() const;

	ScreenTexture& set_scale(float scale);
	ScreenTexture& set_scale(float scale, int frames);

	//TODO: Holy shit that's a lot of scale/crop functions. Consider making a single function using bit
	//flags instead?

	ScreenTexture& scale_right_edge(float percent);
	ScreenTexture& scale_right_edge(float percent, int frames);
	ScreenTexture& crop_right_edge(float percent);
	ScreenTexture& crop_right_edge(float percent, int frames);
	ScreenTexture& scale_left_edge(float percent);
	ScreenTexture& scale_left_edge(float percent, int frames);
	ScreenTexture& crop_left_edge(float percent);
	ScreenTexture& crop_left_edge(float percent, int frames);
	ScreenTexture& scale_top_edge(float percent);
	ScreenTexture& scale_top_edge(float percent, int frames);
	ScreenTexture& crop_top_edge(float percent);
	ScreenTexture& crop_top_edge(float percent, int frames);
	ScreenTexture& scale_bottom_edge(float percent);
	ScreenTexture& scale_bottom_edge(float percent, int frames);
	ScreenTexture& crop_bottom_edge(float percent);
	ScreenTexture& crop_bottom_edge(float percent, int frames);

	ScreenTexture& scale_top_right_corner(float percent_x, float percent_y);
	ScreenTexture& scale_top_right_corner(float percent_x, float percent_y, int frames);
	ScreenTexture& crop_top_right_corner(float percent_x, float percent_y);
	ScreenTexture& crop_top_right_corner(float percent_x, float percent_y, int frames);
	ScreenTexture& scale_top_left_corner(float percent_x, float percent_y);
	ScreenTexture& scale_top_left_corner(float percent_x, float percent_y, int frames);
	ScreenTexture& crop_top_left_corner(float percent_x, float percent_y);
	ScreenTexture& crop_top_left_corner(float percent_x, float percent_y, int frames);
	ScreenTexture& scale_bottom_right_corner(float percent_x, float percent_y);
	ScreenTexture& scale_bottom_right_corner(float percent_x, float percent_y, int frames);
	ScreenTexture& crop_bottom_right_corner(float percent_x, float percent_y);
	ScreenTexture& crop_bottom_right_corner(float percent_x, float percent_y, int frames);
	ScreenTexture& scale_bottom_left_corner(float percent_x, float percent_y);
	ScreenTexture& scale_bottom_left_corner(float percent_x, float percent_y, int frames);
	ScreenTexture& crop_bottom_left_corner(float percent_x, float percent_y);
	ScreenTexture& crop_bottom_left_corner(float percent_x, float percent_y, int frames);

	ScreenTexture& set_alpha(unsigned char alpha);
	ScreenTexture& set_alpha(unsigned char alpha, int frames);
	ScreenTexture& add_alpha(unsigned char alpha);
	unsigned char get_alpha() const;

	ScreenTexture& set_colormod(glm::vec3 color);
	ScreenTexture& set_colormod(glm::vec3 color, int frames);
	ScreenTexture& add_colormod(glm::vec3 color);
	glm::vec3 get_colormod() const;

	ScreenTexture& flip_h();
	ScreenTexture& set_h_flipped(bool h_flipped);
	bool is_h_flipped() const;
	ScreenTexture& flip_v();
	ScreenTexture& set_v_flipped(bool v_flipped);
	bool is_v_flipped() const;
	int get_flipped() const;

	ScreenTexture& set_sprite(unsigned int sprite);
	ScreenTexture& next_sprite();
	ScreenTexture& prev_sprite();
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

	int screen_orientation;
	int texture_orientation;

	TargetVar<glm::vec3> pos;
	TargetVar<glm::vec3> rot;

	TargetVar<int> base_width;
	TargetVar<int> base_height;
	TargetVar<float> width_scale;
	TargetVar<float> height_scale;

	TargetVar<unsigned char> alpha;
	TargetVar<glm::vec3> colormod;

	bool h_flipped;
	bool v_flipped;

	unsigned int sprite;

	bool loaded;
};