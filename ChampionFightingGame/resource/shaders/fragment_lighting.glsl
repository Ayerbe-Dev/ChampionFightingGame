#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
  
uniform vec3 light_pos; 
uniform vec3 view_pos;
uniform vec3 light_col;
uniform vec3 object_col;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * light_col;

    vec3 norm = normalize(Normal);
    vec3 light_dir = normalize(light_pos - FragPos);  
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * light_col;

    float spec_strength = 0.5;
	vec3 view_dir = normalize(view_pos - FragPos);
	vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
    vec3 specular = spec_strength * spec * light_col;  

    vec3 result = (ambient + diffuse + specular) * object_col;
    FragColor = vec4(result, 1.0);
}