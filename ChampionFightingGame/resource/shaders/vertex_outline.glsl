#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_nor;
layout (location = 2) in vec2 v_texcoords;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;
layout (location = 5) in ivec4 v_boneids;
layout (location = 6) in vec4 v_weights;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;

out VS_OUT {
    vec3 Normal;
} vs_out;

uniform mat4 model_matrix;
layout(std140) uniform CameraMatrix {
    mat4 camera_matrix;
};
layout(std140) uniform ViewMatrix {
    mat4 view_matrix;
};
uniform mat4 bone_matrix[MAX_BONES];

void main() {
#ifdef SHADER_FEAT_HAS_BONES
    mat4 bone_transform = mat4(0.0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        bone_transform += bone_matrix[v_boneids[i]] * v_weights[i];
    }
#else
    mat4 bone_transform = mat4(1.0);
#endif

    vec4 total_pos = bone_transform * vec4(v_pos, 1.0);
    
    mat3 normal_mat = mat3(transpose(inverse(view_matrix * model_matrix * bone_transform)));
    vs_out.Normal = normalize(vec3(vec4(normal_mat * v_nor, 0.0)));

    gl_Position = camera_matrix * (model_matrix * total_pos);
} 