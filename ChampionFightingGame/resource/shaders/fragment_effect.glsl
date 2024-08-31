#version 330 core
layout (location = 0) out vec4 g_diffuse;
layout (location = 1) out vec4 g_pos_outline;
layout (location = 2) out vec4 g_normal_spec;
layout (location = 3) out vec4 g_emission;

in GS_OUT {
    vec2 TexCoords;
} fs_in;

uniform sampler2D f_texture;
uniform vec4 f_colormod;

void main() {
    g_diffuse = texture(f_texture, fs_in.TexCoords);
    if (g_diffuse.a < 0.2) {
        discard;
    }
    g_diffuse.rgb += f_colormod.rgb;
    g_diffuse.a -= f_colormod.a;
    g_emission = g_diffuse;
}