#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

#define MAX_LIGHT_SOURCES 10

struct Light {
    vec3 position;
    vec3 color;
	
    float linear;
    float quadratic;
    bool enabled;
};

layout(std140) uniform ViewPos {
    vec3 view_pos;
};
uniform Light light[MAX_LIGHT_SOURCES];

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D g_diffuse;
uniform sampler2D g_specular;
uniform sampler2D ssao;

void main() {
    vec3 FragPos = texture(g_position, TexCoords).rgb;
    vec3 Normal = texture(g_normal, TexCoords).rgb;
    vec3 Diffuse = texture(g_diffuse, TexCoords).rgb * texture(g_diffuse, TexCoords).a;
    vec3 Specular = texture(g_specular, TexCoords).rgb * texture(g_specular, TexCoords).a;
    float AmbientOcclusion = texture(ssao, TexCoords).r;

    vec3 result = vec3(0.3 * (Diffuse - AmbientOcclusion));
    vec3 view_dir = normalize(view_pos - FragPos);

    for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
        if (light[i].enabled) {
            float distance = length(light[i].position - FragPos);
            vec3 light_dir = normalize(light[i].position - FragPos);
            vec3 diffuse = max(dot(Normal, light_dir), 0.0) * Diffuse * light[i].color;

            vec3 halfway_dir = normalize(light_dir + view_dir);
            float spec = pow(max(dot(Normal, halfway_dir), 0.0), 32.0);
            vec3 specular = light[i].color * spec * Specular;

            float attenuation = 1.0 / (1.0 + light[i].linear * distance + light[i].quadratic * distance * distance);

            diffuse *= attenuation;
            specular *= attenuation;
            
            result += diffuse + specular;
        }
    }


    FragColor = vec4(result, 1.0);
}