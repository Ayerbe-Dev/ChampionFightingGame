#version 330 core
layout (location = 0) out vec4 g_diffuse;
layout (location = 1) out vec4 g_specular;
layout (location = 2) out vec4 g_position;
layout (location = 3) out vec4 g_normal;
layout (location = 4) out vec4 g_diffuse_ex;
layout (location = 5) out vec4 g_vert_ex;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
}; 

in GS_OUT {
    vec4 VertPos;
    vec4 FragPos;
    vec4 FragPosLightSpace;
    vec3 Normal;
    vec2 TexCoords;
    float Ex;
} fs_in;

#ifdef SHADER_FEAT_DIM_MUL
layout(std140) uniform DimMul {
    float dim_mul;
};
#endif
uniform float alpha;
uniform sampler2D shadow_map;
layout(std140) uniform ShadowLight {
    vec3 shadow_light;
};

uniform Material material;

float calc_shadow(vec4 fragPosLightSpace);

void main() {
    float shadow = calc_shadow(fs_in.FragPosLightSpace);

    g_position = vec4(fs_in.FragPos);
    g_normal = vec4(normalize(fs_in.Normal), 1.0);

#ifdef SHADER_FEAT_DIM_MUL
    g_diffuse.rgb = (1.0 - shadow) * dim_mul * texture(material.diffuse, fs_in.TexCoords).rgb;
#else
    g_diffuse.rgb = (1.0 - shadow) * texture(material.diffuse, fs_in.TexCoords).rgb;
#endif
    g_specular.rgba = texture(material.specular, fs_in.TexCoords).rgba;
    g_diffuse.a = alpha;
    g_specular.a *= alpha;
    g_diffuse_ex = g_diffuse;
    g_diffuse_ex.a *= fs_in.Ex;
    g_vert_ex = fs_in.VertPos * vec4(fs_in.Ex);
}

float calc_shadow(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;

    if (currentDepth > 1.0) {
        return 0.0;
    }

    float closestDepth = texture(shadow_map, projCoords.xy).r; 
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(shadow_light - vec3(fs_in.FragPos));
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x < 1; ++x) {
        for(int y = -1; y < 1; ++y) {
            float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }    
    }
    shadow /= 9.0;

    return shadow;
}