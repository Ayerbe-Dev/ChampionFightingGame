#version 330 core
out float FragColor;
  
in vec2 TexCoords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D tex_noise;

uniform vec3 samples[16];
uniform int window_width;
uniform int window_height;

uniform mat4 projection_matrix;

int num_kernels = 16;
float radius = 0.1;
float bias = 0.3;

void main() {    
	vec2 noise_scale = vec2(window_width / 4, window_height / 4);

	vec3 fragPos = texture(g_position, TexCoords).xyz;
	vec3 normal = texture(g_normal, TexCoords).rgb;
	vec3 random_vec = texture(tex_noise, TexCoords * noise_scale).xyz;

	vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);  

	int occlusion = 0;
    for (int i = 0; i < num_kernels; i++) {
        vec3 sample_pos = TBN * samples[i];
        sample_pos = fragPos + sample_pos * radius; 
        
        vec4 offset = vec4(sample_pos, 1.0);
        offset = projection_matrix * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
        
        float depth = texture(g_position, offset.xy).z;
        
        if (depth < sample_pos.z - bias) {
            occlusion++;
        }
    }
    FragColor = float(bool(occlusion >= 10));
}