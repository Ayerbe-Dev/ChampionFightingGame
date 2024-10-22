#version 330 core

layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_texcoords;

uniform mat4 matrix;
layout(std140) uniform CameraMatrix {
    mat4 camera_matrix;
};
#ifdef SHADER_FEAT_BILLBOARD
layout(std140) uniform ViewMatrix {
    mat4 view_matrix;
};
uniform vec2 billboard_size;
#endif

out vec2 tex_coords;

void main() {
    gl_Position = camera_matrix * matrix * vec4(v_pos, 0.0f, 1.0f);
    tex_coords = vec2(v_texcoords);
} 