#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D g_buffer_out;
#ifdef SHADER_FEAT_BLOOM
uniform sampler2D bloom;
#endif
uniform float exposure;

void main() {
    const float gamma = 2.2;
#ifdef SHADER_FEAT_BLOOM
    vec3 hdrColor = texture(g_buffer_out, TexCoords).rgb + texture(bloom, TexCoords).rgb;
#else
    vec3 hdrColor = texture(g_buffer_out, TexCoords).rgb;
#endif
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}