#version 330 core
out vec4 FragColor;

#define MAX_LIGHT_SOURCES 10

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
in vec3 Normal;  
in vec2 TexCoords;
in vec4 FragPosLightSpace;
  
uniform vec3 view_pos;
uniform Material material;
uniform Light light[MAX_LIGHT_SOURCES];


float ShadowCalculation(vec4 fragPosLightSpace) {
    float bias = 0.005;
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    ////IMPL WITHOUT ANTI ALIAS
        // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(material.shadow_map, projCoords.xy).r; 
        // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
        // check whether current frag pos is in shadow .. 0.005 is bias
    //float shadow = currentDepth - 0.005 > closestDepth  ? 1.0 : 0.0;

    ////IMPL WITH ANTI ALIAS
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(material.shadow_map, 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(material.shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 0.5 : 0.0;        
        }    
    }
    shadow /= 25.0;

    return shadow;
}

void main() {
    vec3 result = vec3(0.0, 0.0, 0.0);
    for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
        if (light[i].enabled) {
            vec3 ambient = light[i].ambient * texture(material.diffuse, TexCoords).rgb;
  	 
            vec3 norm = normalize(Normal);
            vec3 light_dir = normalize(light[i].position - FragPos);
            float diff = max(dot(norm, light_dir), 0.0);
            vec3 diffuse = light[i].diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
            vec3 view_dir = normalize(view_pos - FragPos);
            vec3 reflect_dir = reflect(-light_dir, norm);  
            float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
            vec3 specular = light[i].specular * spec * texture(material.specular, TexCoords).rgb;  
    
            float distance = length(light[i].position - FragPos);
            float attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));    

            ambient *= attenuation;  
            diffuse *= attenuation;
            specular *= attenuation;   
        
            result += ambient + diffuse + specular;
        }
    }

    float shadow_result = ShadowCalculation(FragPosLightSpace);
    vec3 color = texture(material.diffuse, TexCoords).rgb;
    FragColor = vec4((1.0 - shadow_result) * result, 1.0);
}