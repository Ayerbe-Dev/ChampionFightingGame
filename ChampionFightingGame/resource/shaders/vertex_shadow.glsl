#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 5) in ivec4 v_boneids;
layout (location = 6) in vec4 v_weights;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 model_matrix;
uniform mat4 shadow_matrix;
uniform mat4 bone_matrix[MAX_BONES];
uniform bool has_skeleton;

void main() {
    mat4 bone_transform = mat4(!has_skeleton);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        bone_transform += bone_matrix[v_boneids[i]] * v_weights[i];
    }

    vec4 total_pos = bone_transform * vec4(v_pos, 1.0);

    gl_Position = shadow_matrix * (model_matrix * total_pos);
}  