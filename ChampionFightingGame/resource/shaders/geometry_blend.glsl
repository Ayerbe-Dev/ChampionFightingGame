#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 TexCoords;
} gs_in[];

out GS_OUT {
    vec2 TexCoords;
} gs_out;

void main() {
    for (int i = 0, max = gl_in.length(); i < max; i++) {
        gs_out.TexCoords = gs_in[i].TexCoords;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}