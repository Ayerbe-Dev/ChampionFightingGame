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

uniform sampler2D g_diffuse;
uniform sampler2D g_pos_outline;
uniform sampler2D g_normal_spec;
#ifdef SHADER_FEAT_SSAO
uniform sampler2D ssao;
#endif
void main() {
    const float linear = 0.007;
	const float quadratic = 0.0002;

    vec4 pos_outline = texture(g_pos_outline, TexCoords);
#ifdef SHADER_FEAT_OUTLINE
	if (pos_outline.a != 1.0) {
	    vec2 texelSize = 1.0 / vec2(textureSize(g_pos_outline, 0));
		for (int x = -2; x <= 2; x++) {
			for (int y = -2; y <= 2; y++) {
				vec2 offset = vec2(float(x), float(y)) * texelSize;
				if (texture(g_pos_outline, TexCoords + offset).a == 1.0) {
					FragColor = vec4(0.0, 0.0, 0.0, 1.0);
					return;
				}
			}
		}
	}
#endif
    vec3 FragPos = pos_outline.rgb / pos_outline.a;

#ifdef SHADER_FEAT_DIFFUSE
    vec3 Diffuse = texture(g_diffuse, TexCoords).rgb * texture(g_diffuse, TexCoords).a;
#else
    vec3 Diffuse = vec3(1.0);
#endif

vec4 normal_spec = texture(g_normal_spec, TexCoords);
vec3 Normal = normal_spec.rgb / normal_spec.a;

#ifdef SHADER_FEAT_SPECULAR
    float Specular = max(0.0, normal_spec.a - 1.0);
#else
    float Specular = 0.0;
#endif

    vec3 result = vec3(ambient * Diffuse);

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

#ifdef SHADER_FEAT_SSAO
    FragColor = vec4(result * texture(ssao, TexCoords).r, 1.0);
#else
    FragColor = vec4(result, 1.0);
#endif
}