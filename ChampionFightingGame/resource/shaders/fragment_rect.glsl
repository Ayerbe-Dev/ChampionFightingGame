#version 330 core
out vec4 FragColor;

uniform vec4 f_rgba;

void main() {    
    FragColor = f_rgba;
}