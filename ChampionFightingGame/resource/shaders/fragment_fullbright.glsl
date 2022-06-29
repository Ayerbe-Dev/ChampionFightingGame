#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
    sampler2D shadow_map;
    float shininess;
}; 

in GS_OUT {
    vec3 FragPos;  
    vec3 Normal;  
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform Material material;

void main() {    
    FragColor = vec4(texture(material.diffuse, fs_in.TexCoords).rgb + texture(material.specular, fs_in.TexCoords).rgb, 1.0);
}