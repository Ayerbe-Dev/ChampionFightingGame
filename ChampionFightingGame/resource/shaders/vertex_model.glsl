#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_nor;
layout (location = 2) in vec2 v_texcoords;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;
layout (location = 5) in vec4 v_weights;
layout (location = 6) in ivec4 v_boneids;

out VS_OUT {
    vec4 FragPos;
    vec4 FragPosLightSpace;
    vec3 Normal;
    vec2 TexCoords;
    float Ex;
} vs_out;

uniform mat4 model_matrix;
layout(std140) uniform CameraMatrix {
    mat4 camera_matrix;
};
layout(std140) uniform ViewMatrix {
    mat4 view_matrix;
};
layout(std140) uniform ShadowMatrix {
    mat4 shadow_matrix;
};

const int MAX_BONES = 200;
#ifdef SHADER_FEAT_BONES
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 bone_matrix[MAX_BONES];
#endif
uniform bool ex_render_enabled[MAX_BONES];

void main() {
#ifdef SHADER_FEAT_BONES
    float ex = 0.0;
    mat4 bone_transform = mat4(0.0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        bone_transform += bone_matrix[v_boneids[i]] * v_weights[i];
        if (ex_render_enabled[v_boneids[i]]) {
            ex = 1.0;
        }
    }
#else
    float ex = 0.0;
    mat4 bone_transform = mat4(1.0);
#endif
    vec4 transform = model_matrix * bone_transform * vec4(v_pos, 1.0);

    vs_out.FragPos = vec4(view_matrix * transform);    
    vs_out.FragPosLightSpace = vec4(shadow_matrix * transform);
    vs_out.Normal = transpose(inverse(mat3(view_matrix * model_matrix * bone_transform))) * v_nor;
    vs_out.TexCoords = v_texcoords;
    vs_out.Ex = ex;
    gl_Position = camera_matrix * transform;
} 