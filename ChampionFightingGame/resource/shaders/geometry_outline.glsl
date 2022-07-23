#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 Normal;
} gs_in[];

vec4 expand(vec4 position, vec3 normal) {
    float magnitude = 0.01;
    vec3 direction = normal * magnitude; 
    return position + vec4(direction, 0.0);
} 

void main() {
    for (int i = 0, max = gl_in.length(); i < max; i++) {
        gl_Position = expand(gl_in[i].gl_Position, gs_in[i].Normal);
        EmitVertex();
    }

    EndPrimitive();
}  