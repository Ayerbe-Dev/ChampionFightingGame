#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_nor;
layout (location = 2) in vec2 v_texcoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(v_pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * v_nor;  
    TexCoords = v_texcoords;

    gl_Position = projection * view * model * vec4(FragPos, 1.0);
} 