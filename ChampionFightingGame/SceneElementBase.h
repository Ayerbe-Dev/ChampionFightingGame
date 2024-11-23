#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "WindowConstants.h"

struct TextureAnchor {
	TextureAnchor() {
		screen_mat = glm::mat4(1.0);
		world_mat = glm::mat4(1.0);
		w = WINDOW_WIDTH;
		h = WINDOW_HEIGHT;
	}
	glm::mat4 screen_mat;
	glm::mat4 world_mat;
	float w;
	float h;
};

const int TEX_COORD_TOP_LEFT = 0;
const int TEX_COORD_TOP_RIGHT = 1;
const int TEX_COORD_BOTTOM_LEFT = 2;
const int TEX_COORD_BOTTOM_RIGHT = 3;
const int TEX_COORD_TOP_RIGHT2 = 4;
const int TEX_COORD_BOTTOM_LEFT2 = 5;

const int TEX_FEAT_4T5V = 1;

const int TEX_FLIP_N = 0;
const int TEX_FLIP_H = 1;
const int TEX_FLIP_V = 2;
const int TEX_FLIP_B = 3;

const int BILLBOARD_OFF = 0;
const int BILLBOARD_ON = 1;
const int BILLBOARD_ON_FIXED_SIZE = 2;

//"2 triangles, 4 vertices" -- This set of constants gives us the vertex
//indices for Screen/WorldTextures which are composed of 2 triangles and 4 unique
//vertex positions (I.E. Screen/WorldTextures which don't have corner cropping 
//enabled)

const int TC_2T4V_TOP_LEFT = 0;
const int TC_2T4V_TOP_RIGHT = 1;
const int TC_2T4V_BOTTOM_LEFT = 2;
const int TC_2T4V_BOTTOM_RIGHT = 3;
const int TC_2T4V_TOP_RIGHT2 = 4;
const int TC_2T4V_BOTTOM_LEFT2 = 5;

//"4 triangles, 5 vertices" -- To make corner cropping work for all 4 corners, 
//we use 4 triangles and a central vertex which is part of all of them. This
//needs to use a different ordering system, so different constants

const int TC_4T5V_TOP_LEFT = 0;
const int TC_4T5V_TOP_RIGHT = 1;
const int TC_4T5V_MIDDLE = 2;
const int TC_4T5V_BOTTOM_LEFT = 3;
const int TC_4T5V_BOTTOM_RIGHT = 4;
const int TC_4T5V_MIDDLE2 = 5;
const int TC_4T5V_TOP_LEFT2 = 6;
const int TC_4T5V_BOTTOM_LEFT2 = 7;
const int TC_4T5V_MIDDLE3 = 8;
const int TC_4T5V_TOP_RIGHT2 = 9;
const int TC_4T5V_BOTTOM_RIGHT2 = 10;
const int TC_4T5V_MIDDLE4 = 11;

struct VertexSpecifier {
	VertexSpecifier() {
		this->num_vertices = 0;
		this->num_vertices_internal = 0;
		this->top_left_idx = -1;
		this->top_right_idx = -1;
		this->bottom_left_idx = -1;
		this->bottom_right_idx = -1;
	}
	VertexSpecifier(int num_vertices, int num_vertices_internal,
		int top_left_idx, int top_right_idx,
		int bottom_left_idx, int bottom_right_idx,
		std::vector<std::pair<int, int>> vertex_bindings) {
		this->num_vertices = num_vertices;
		this->num_vertices_internal = num_vertices_internal;
		this->top_left_idx = top_left_idx;
		this->top_right_idx = top_right_idx;
		this->bottom_left_idx = bottom_left_idx;
		this->bottom_right_idx = bottom_right_idx;
		this->vertex_bindings = vertex_bindings;
	}
	int num_vertices;
	int num_vertices_internal;
	int top_left_idx;
	int top_right_idx;
	int bottom_left_idx;
	int bottom_right_idx;
	std::vector<std::pair<int, int>> vertex_bindings;
};

const VertexSpecifier v_spec_2t4v = VertexSpecifier(4, 6,
	TC_2T4V_TOP_LEFT, TC_2T4V_TOP_RIGHT, TC_2T4V_BOTTOM_LEFT, TC_2T4V_BOTTOM_RIGHT, {
	{TC_2T4V_TOP_RIGHT2, TC_2T4V_TOP_RIGHT},
	{TC_2T4V_BOTTOM_LEFT2, TC_2T4V_BOTTOM_LEFT},
});

const VertexSpecifier v_spec_4t5v = VertexSpecifier(5, 12,
	TC_4T5V_TOP_LEFT, TC_4T5V_TOP_RIGHT, TC_4T5V_BOTTOM_LEFT, TC_4T5V_BOTTOM_RIGHT, {
	{TC_4T5V_TOP_LEFT2, TC_4T5V_TOP_LEFT},
	{TC_4T5V_TOP_RIGHT2, TC_4T5V_TOP_RIGHT},
	{TC_4T5V_MIDDLE2, TC_4T5V_MIDDLE},
	{TC_4T5V_MIDDLE3, TC_4T5V_MIDDLE},
	{TC_4T5V_MIDDLE4, TC_4T5V_MIDDLE},
	{TC_4T5V_BOTTOM_LEFT2, TC_4T5V_BOTTOM_LEFT},
	{TC_4T5V_BOTTOM_RIGHT2, TC_4T5V_BOTTOM_RIGHT},
});

struct TextureCoord {
    glm::vec2 pos;
    glm::vec2 tex_coord;
};

struct GameTextureCoord {
    glm::vec3 pos;
    glm::vec2 tex_coord;
};

const int TEXTURE_MID = 0;
const int TEXTURE_RIGHT = 1;
const int TEXTURE_LEFT = 2;
const int TEXTURE_TOP = 4;
const int TEXTURE_BOTTOM = 8;
const int TEXTURE_TOP_LEFT = TEXTURE_TOP | TEXTURE_LEFT;
const int TEXTURE_TOP_RIGHT = TEXTURE_TOP | TEXTURE_RIGHT;
const int TEXTURE_BOTTOM_LEFT = TEXTURE_BOTTOM | TEXTURE_LEFT;
const int TEXTURE_BOTTOM_RIGHT = TEXTURE_BOTTOM | TEXTURE_RIGHT;
