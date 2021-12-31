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

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 final_bones_matrices[MAX_BONES];

void main() {
    vec4 total_pos = vec4(0.0);
    for (int i = 0 ; i < MAX_BONE_INFLUENCE; i++) {
        if (v_boneids[i] == -1) {
            continue;
        }
        if (v_boneids[i] >= MAX_BONES) {
            total_pos = vec4(v_pos, 1.0f);
            break;
        }
        vec4 local_pos = final_bones_matrices[v_boneids[i]] * vec4(v_pos, 1.0f);
        total_pos += local_pos * v_weights[i];
        vec3 local_nor = mat3(final_bones_matrices[v_boneids[i]]) * v_nor;
    }

    FragPos = vec3(model * vec4(v_pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * v_nor;  
    TexCoords = v_texcoords;

    gl_Position = projection * view * model * total_pos;
} 