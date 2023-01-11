#version 330 core
out vec4 FragColor;

in vec2 tex_coords;

uniform sampler2D f_texture;
#ifdef SHADER_FEAT_COLORMOD
uniform vec3 f_colormod;
#endif
uniform float f_alphamod;

void main() {    
    vec4 tex_col = texture(f_texture, tex_coords);
    #ifdef SHADER_FEAT_COLORMOD
    tex_col.rgb += f_colormod;
    #endif
    tex_col.a -= f_alphamod;
    FragColor = tex_col;
}