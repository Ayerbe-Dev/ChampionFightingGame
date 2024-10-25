#version 330 core

layout (location = 0) in vec2 v_pos;
layout (location = 1) in vec2 v_texcoords;

uniform mat4 matrix;
layout(std140) uniform CameraMatrix {
    mat4 camera_matrix;
};
#ifdef SHADER_FEAT_BILLBOARD_ON
layout(std140) uniform ViewMatrix {
    mat4 view_matrix;
};
uniform vec3 world_pos;
#endif
out vec2 tex_coords;

void main() {
#ifdef SHADER_FEAT_BILLBOARD_ON
    vec3 cam_right = { view_matrix[0][0], view_matrix[1][0], view_matrix[2][0] };
    vec3 cam_up = { view_matrix[0][1], view_matrix[1][1], view_matrix[2][1] };
    vec3 pos = world_pos + cam_right * v_pos.x + cam_up * v_pos.y;
    gl_Position = camera_matrix * matrix * vec4(pos, 1.0f);
#else
    gl_Position = camera_matrix * matrix * vec4(v_pos, 0.0f, 1.0f);
#endif
    tex_coords = vec2(v_texcoords);
} 