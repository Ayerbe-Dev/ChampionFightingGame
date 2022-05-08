#pragma once
#include "glm/glm.hpp"

enum {
    TEX_COORD_BOTTOM_LEFT,
    TEX_COORD_BOTTOM_RIGHT,
    TEX_COORD_TOP_RIGHT,
    TEX_COORD_TOP_LEFT,

    TEX_COORD_MAX,
};

struct TextureCoord {
    glm::vec3 pos;
    glm::vec2 tex_coord;
};