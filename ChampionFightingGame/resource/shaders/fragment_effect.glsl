#version 330 core
layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_diffuse;
layout (location = 3) out vec4 g_specular;

in GS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D f_texture;
uniform vec4 f_colormod;

void main() {
    g_diffuse = texture(f_texture, fs_in.TexCoords);
    g_diffuse.rgb += f_colormod.rgb;
    g_diffuse.a -= f_colormod.a;
}