#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 5) in vec4 v_weights;
layout (location = 6) in ivec4 v_boneids;

uniform mat4 model_matrix;
layout(std140) uniform ShadowMatrix {
    mat4 shadow_matrix;
};
#ifdef SHADER_FEAT_BONES
const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bone_matrix[MAX_BONES];
#endif

void main() {
#ifdef SHADER_FEAT_BONES
    mat4 bone_transform = mat4(0.0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        bone_transform += bone_matrix[v_boneids[i]] * v_weights[i];
    }
#else
    mat4 bone_transform = mat4(1.0);
#endif

     vec4 transform = model_matrix * bone_transform * vec4(v_pos, 1.0);

    gl_Position = shadow_matrix * transform;
}  