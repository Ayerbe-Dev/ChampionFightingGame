#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

#define MAX_LIGHT_SOURCES 10

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
  
uniform vec3 view_pos;
uniform Material material;
uniform Light light[MAX_LIGHT_SOURCES];

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

    FragColor = vec4(result, 1.0);
}