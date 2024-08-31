#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec4 FragPos;
    vec4 FragPosLightSpace;
    vec3 Normal;
    mat3 TBN;
    vec2 TexCoords;
    float Ex;
} gs_in[];

out GS_OUT {
    vec4 FragPos;
    vec4 FragPosLightSpace;
    vec3 Normal;
    mat3 TBN;
    vec2 TexCoords;
    float Ex;
} gs_out;

void main() {
    for (int i = 0, max = gl_in.length(); i < max; i++) {
        gs_out.FragPos = gs_in[i].FragPos;
        gs_out.FragPosLightSpace = gs_in[i].FragPosLightSpace;
        gs_out.Normal = gs_in[i].Normal;
        gs_out.TBN = gs_in[i].TBN;
        gs_out.TexCoords = gs_in[i].TexCoords;
        gs_out.Ex = gs_in[i].Ex;
        
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}  