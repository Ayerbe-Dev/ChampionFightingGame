#version 330 core
layout (location = 0) out vec4 g_diffuse;
layout (location = 1) out vec4 g_pos_outline;
layout (location = 2) out vec4 g_normal_spec;
layout (location = 3) out vec4 g_tangent_ex;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
}; 

in GS_OUT {
    vec4 FragPos;
    vec4 FragPosLightSpace;
    vec3 Tangent;
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

layout(std140) uniform Outline {
    float outline;
};

uniform Material material;

float calc_shadow(vec4 fragPosLightSpace);

void main() {
    float shadow = calc_shadow(fs_in.FragPosLightSpace);

    g_pos_outline.xyz = vec3(fs_in.FragPos);
    g_pos_outline.a = outline;    
    
    g_normal_spec.xyz = normalize(fs_in.Normal);
    g_normal_spec.a = 0.5;

    g_tangent_ex.xyz = fs_in.Tangent;
    g_tangent_ex.a = fs_in.Ex;

    vec4 diffuse = texture(material.diffuse, fs_in.TexCoords).rgba;
#ifdef SHADER_FEAT_DIM_MUL
    g_diffuse.rgb = (1.0 - shadow) * dim_mul * diffuse.rgb;
#else
    g_diffuse.rgb = (1.0 - shadow) * diffuse.rgb;
#endif
    g_diffuse.a = diffuse.a * alpha;
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