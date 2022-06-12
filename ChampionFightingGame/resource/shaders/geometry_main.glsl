#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {

    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} gs_in[];

out GS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} gs_out;

void main() {
    for (int i = 0, max = gl_in.length(); i < max; i++) {
        gl_Position = gl_in[i].gl_Position; 
        gs_out.FragPos = gs_in[i].FragPos;
        gs_out.Normal = gs_in[i].Normal;
        gs_out.TexCoords = gs_in[i].TexCoords;
        gs_out.FragPosLightSpace = gs_in[i].FragPosLightSpace;
        EmitVertex();
    }
    EndPrimitive();
}  