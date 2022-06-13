#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

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
    vec3 color;
	
    float linear;
    float quadratic;
    bool enabled;
};
  
uniform vec3 view_pos;
uniform Light light[MAX_LIGHT_SOURCES];

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_albedo;

void main() {
    vec3 FragPos = texture(g_position, TexCoords).rgb;
    vec3 Normal = texture(g_normal, TexCoords).rgb;
    vec3 Diffuse = texture(g_albedo, TexCoords).rgb;
    float Specular = texture(g_albedo, TexCoords).a;

    vec3 result = Diffuse * 0.1;
    vec3 view_dir = normalize(view_pos - FragPos);

    for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
        if (light[i].enabled) {
            float distance = length(light[i].position - FragPos);
            vec3 light_dir = normalize(light[i].position - FragPos);
            vec3 diffuse = max(dot(Normal, light_dir), 0.0) * Diffuse * light[i].color;

            vec3 halfway_dir = normalize(light_dir + view_dir);
            float spec = pow(max(dot(Normal, halfway_dir), 0.0), 16.0);
            vec3 specular = light[i].color * spec * Specular;

            float attenuation = 1.0 / (1.0 + light[i].linear * distance + light[i].quadratic * distance * distance);    

            diffuse *= attenuation;
            specular *= attenuation;

//            result += diffuse + specular;
            result += diffuse;
        }
    }

    FragColor = vec4(result, 1.0);
}