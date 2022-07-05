#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_nor;
layout (location = 2) in vec2 v_texcoords;
layout (location = 3) in vec3 v_tangent;
layout (location = 4) in vec3 v_bitangent;
layout (location = 5) in ivec4 v_boneids;
layout (location = 6) in vec4 v_weights;

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
uniform bool flipped;

void main() {
    vec4 total_pos = vec4(v_pos, 1.0);

    vs_out.FragPos = vec3(view_matrix * model_matrix * total_pos);
    vec3 Normal = flipped ? -v_nor : v_nor;
    vs_out.Normal = mat3(transpose(inverse(view_matrix * model_matrix))) * Normal;  
    vs_out.TexCoords = v_texcoords;
    vs_out.FragPosLightSpace = shadow_matrix * vec4(vec3(model_matrix * total_pos), 1.0);
    gl_Position = camera_matrix * (model_matrix * total_pos);
} 