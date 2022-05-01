#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_nor;
layout (location = 2) in vec2 v_texcoords;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;
layout (location = 5) in ivec4 v_boneids;
layout (location = 6) in ivec4 v_f_boneids;
layout (location = 7) in vec4 v_weights;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bone_matrix[MAX_BONES];
uniform mat4 shadow_light_view;
uniform bool flipped;

void main() {
    mat4 bone_transform = mat4(0.0);
    float total_weights = 0.0;
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        if (flipped) {
            bone_transform += bone_matrix[v_f_boneids[i]] * v_weights[i];
        }
        else {
            bone_transform += bone_matrix[v_boneids[i]] * v_weights[i];
        }
        total_weights += v_weights[i];
    }
    if (total_weights < 1.0) {
        if (flipped) {
            bone_transform += bone_matrix[v_f_boneids[0]] * (1.0 - total_weights);
        }
        else {
            bone_transform += bone_matrix[v_boneids[0]] * (1.0 - total_weights);
        }
    }

    vec4 total_pos = bone_transform * vec4(v_pos, 1.0);
    
    FragPos = vec3(model * total_pos);
    Normal = mat3(transpose(inverse(model))) * v_nor;  
    TexCoords = v_texcoords;
    FragPosLightSpace = shadow_light_view * vec4(FragPos,1.0);

    gl_Position = projection * view * (model * total_pos);
} 