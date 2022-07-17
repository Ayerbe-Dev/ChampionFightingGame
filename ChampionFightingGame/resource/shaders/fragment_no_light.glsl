#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
    sampler2D shadow_map;
    float shininess;
}; 

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_diffuse;
uniform sampler2D g_specular;
uniform sampler2D ssao;

void main() {
    vec3 Diffuse = texture(g_diffuse, TexCoords).rgb * texture(g_diffuse, TexCoords).a;
    float AmbientOcclusion = texture(ssao, TexCoords).r;

    vec3 result = Diffuse - vec3(AmbientOcclusion * 0.1);

    FragColor = vec4(result, 1.0);
}