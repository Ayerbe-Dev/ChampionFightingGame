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
    mat3 TBN;
    vec2 TexCoords;
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

    mat3 normal_matrix = transpose(inverse(mat3(view_matrix * model_matrix * bone_transform)));
    vec3 T = normalize(normal_matrix * v_tangent);
    vec3 B = normalize(normal_matrix * v_bitangent);
    vec3 N = normalize(normal_matrix * v_nor);
    vs_out.FragPos = vec4(view_matrix * transform);    
    vs_out.FragPosLightSpace = vec4(shadow_matrix * transform);
    vs_out.TBN = mat3(T, B, N);
    vs_out.Normal = normal_matrix * v_nor;
    vs_out.TexCoords = v_texcoords;
    gl_Position = camera_matrix * transform;
} 