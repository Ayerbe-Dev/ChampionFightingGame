#version 330 core
out vec4 FragColor;

#define MAX_LIGHT_SOURCES 10

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
    sampler2D shadow_map;
    float shininess;
}; 

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
    bool enabled;
};

in vec3 FragPos;  
in vec4 FragPosLightSpace;
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 view_pos;
uniform Material material;
uniform Light light[MAX_LIGHT_SOURCES];

vec3 calc_light(Light light, vec3 normal, vec3 view_dir, vec3 diffuse_col, vec3 spec_col);
float calc_shadow(vec4 fragPosLightSpace);

void main() {
    vec3 result = vec3(0.0, 0.0, 0.0);
    vec3 normal = normalize(Normal);
    vec3 view_dir = normalize(view_pos - FragPos);
    vec3 diffuse_col = texture(material.diffuse, TexCoords).rgb;
    vec3 spec_col = texture(material.specular, TexCoords).rgb;
    for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
        if (light[i].enabled) {
            result += calc_light(light[i], normal, view_dir, diffuse_col, spec_col);
        }
    }

    float shadow_result = calc_shadow(FragPosLightSpace);
    FragColor = vec4((1.0 - shadow_result) * result, 1.0);
}

vec3 calc_light(Light light, vec3 normal, vec3 view_dir, vec3 diffuse_col, vec3 spec_col) {
    vec3 light_dir = normalize(light.position - FragPos);

    vec3 ambient = light.ambient * diffuse_col;

    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuse_col;  

    vec3 reflect_dir = reflect(-light_dir, normal);  
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * spec_col;

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient *= attenuation;  
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

float calc_shadow(vec4 fragPosLightSpace) {
    float bias = 0.005;
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(material.shadow_map, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(material.shadow_map, 0);
    for(int x = -2; x <= 2; ++x) {
        for(int y = -2; y <= 2; ++y) {
            float pcfDepth = texture(material.shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 0.5 : 0.0;        
        }    
    }
    shadow /= 25.0;

    return shadow;
}