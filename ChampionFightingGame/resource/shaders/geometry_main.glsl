#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 6) out;

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
    gl_Position = gl_in[0].gl_Position; 
    gs_out.FragPos = gs_in[0].FragPos;
    gs_out.Normal = gs_in[0].Normal;
    gs_out.TexCoords = gs_in[0].TexCoords;
    gs_out.FragPosLightSpace = gs_in[0].FragPosLightSpace;
    EmitVertex();
    EndPrimitive();
}  