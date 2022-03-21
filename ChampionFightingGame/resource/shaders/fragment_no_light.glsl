#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

in vec2 TexCoords;

uniform Material material;

void main() {    
    FragColor = vec4(texture(material.diffuse, TexCoords).rgb, 1.0);
}