#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

#define MAX_LIGHT_SOURCES 10

struct Light {
    vec3 position;
    vec3 color;
	
    bool enabled;
};

layout(std140) uniform ViewPos {
    vec3 view_pos;
};
uniform vec3 ambient;
uniform Light light[MAX_LIGHT_SOURCES];

#ifdef SHADER_FEAT_DIFFUSE
uniform sampler2D g_diffuse;
#endif

#ifdef SHADER_FEAT_SPECULAR
uniform sampler2D g_specular;
#endif

#ifdef SHADER_FEAT_POSITION
uniform sampler2D g_position;
#endif

#ifdef SHADER_FEAT_NORMAL
uniform sampler2D g_normal;
#endif

#ifdef SHADER_FEAT_SSAO
uniform sampler2D ssao;
#endif

void main() {
    const float linear = 0.007;
	const float quadratic = 0.0002;
#ifdef SHADER_FEAT_DIFFUSE
    vec3 Diffuse = texture(g_diffuse, TexCoords).rgb * texture(g_diffuse, TexCoords).a;
#else
    vec3 Diffuse = vec3(1.0);
#endif

#ifdef SHADER_FEAT_SPECULAR
    vec3 Specular = texture(g_specular, TexCoords).rgb * texture(g_specular, TexCoords).a;
#else
    vec3 Specular = vec3(0.0);
#endif

#ifdef SHADER_FEAT_POSITION
    vec3 FragPos = texture(g_position, TexCoords).rgb;
#else
    vec3 FragPos = vec3(0.0);
#endif

#ifdef SHADER_FEAT_NORMAL
    vec3 Normal = texture(g_normal, TexCoords).rgb;
#else
    vec3 Normal = vec3(0.0);
#endif

#ifdef SHADER_FEAT_SSAO
    float AmbientOcclusion = texture(ssao, TexCoords).r;
#else
    float AmbientOcclusion = 1.0;
#endif

    vec3 result = vec3(ambient * Diffuse * AmbientOcclusion);
    vec3 view_dir = normalize(view_pos - FragPos);

    for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
        if (light[i].enabled) {
            vec3 light_dir = normalize(light[i].position - FragPos);
            vec3 diffuse = max(dot(light_dir, Normal), 0.0) * Diffuse * light[i].color;

            vec3 halfway_dir = normalize(light_dir + view_dir);
            float spec = pow(max(dot(Normal, halfway_dir), 0.0), 16.0);
            vec3 specular = light[i].color * spec * Specular;

            float distance = length(light[i].position - FragPos);
            float attenuation = 1.0 / (1.0 + linear * distance + quadratic * distance * distance);

            diffuse *= attenuation;
            specular *= attenuation;
            
            result += diffuse + specular;
        }
    }

    FragColor = vec4(result, 1.0);
}