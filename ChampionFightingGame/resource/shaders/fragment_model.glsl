#version 330 core
layout (location = 0) out vec3 g_position;
layout (location = 1) out vec3 g_normal;
layout (location = 2) out vec4 g_diffuse;
layout (location = 3) out vec4 g_specular;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
}; 

in GS_OUT {
    vec3 FragPos;  
    vec3 Normal;  
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

#ifdef SHADER_FEAT_DIM_MUL
layout(std140) uniform DimMul {
    float dim_mul;
};
#endif
uniform sampler2D shadow_map;
uniform Material material;

float calc_shadow(vec4 fragPosLightSpace);

void main() {
    float shadow = calc_shadow(fs_in.FragPosLightSpace);

    g_position = fs_in.FragPos;
    g_normal = normalize(fs_in.Normal);

#ifdef SHADER_FEAT_DIM_MUL
    g_diffuse.rgb = (1.0 - shadow) * dim_mul * texture(material.diffuse, fs_in.TexCoords).rgb;
#else
    g_diffuse.rgb = (1.0 - shadow) * texture(material.diffuse, fs_in.TexCoords).rgb;
#endif

    g_diffuse.a = 1.0;
    g_specular.rgb = texture(material.specular, fs_in.TexCoords).rgb;
    g_specular.a = 0.0; //We don't actually have specular textures lmao this will be used later
}

float calc_shadow(vec4 fragPosLightSpace) {
    float bias = 0.005;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadow_map, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x < 1; ++x) {
        for(int y = -1; y < 1; ++y) {
            float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += float(currentDepth - bias > pcfDepth);        
        }    
    }
    shadow /= 12.5;

    return shadow;
}