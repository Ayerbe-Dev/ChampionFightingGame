#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_nor;
layout (location = 2) in vec2 v_texcoords;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;
layout (location = 5) in ivec4 v_boneids;
layout (location = 6) in vec4 v_weights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 model_matrix;
uniform mat4 camera_matrix;
uniform mat4 shadow_matrix;

void main() {
    vec4 total_pos = vec4(v_pos, 1.0);

    FragPos = vec3(model_matrix * total_pos);
    Normal = mat3(transpose(inverse(model_matrix))) * v_nor;  
    TexCoords = v_texcoords;
    FragPosLightSpace = shadow_matrix * vec4(FragPos,1.0);
    gl_Position = camera_matrix * (model_matrix * total_pos);
} 