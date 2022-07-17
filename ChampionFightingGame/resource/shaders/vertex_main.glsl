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
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 view_matrix;
uniform mat4 shadow_matrix;
uniform mat4 bone_matrix[MAX_BONES];

void main() {
    mat4 bone_transform = mat4(0.0);
    float total_weights = 0.0;
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        bone_transform += bone_matrix[v_boneids[i]] * v_weights[i];
        total_weights += v_weights[i];
    }
    if (total_weights < 1.0) {
        bone_transform += bone_matrix[v_boneids[0]] * (1.0 - total_weights);
    }

    vec4 total_pos = bone_transform * vec4(v_pos, 1.0);
    
    vs_out.FragPos = vec3(view_matrix * model_matrix * total_pos);
    vs_out.Normal = mat3(transpose(inverse(view_matrix * model_matrix * bone_transform))) * v_nor;  
    vs_out.TexCoords = v_texcoords;
    vs_out.FragPosLightSpace = shadow_matrix * vec4(vec3(model_matrix * total_pos), 1.0);
    gl_Position = camera_matrix * (model_matrix * total_pos);
} 