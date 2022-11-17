#version 330 core
out float FragColor;
  
in vec2 TexCoords;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D tex_noise;

uniform vec3 samples[16];
uniform int window_width;
uniform int window_height;

layout(std140) uniform ProjectionMatrix {
    mat4 projection_matrix;
};

int kernelSize = 16;
float radius = 0.1;
float bias = 0.01;

void main() {    
	vec2 noise_scale = vec2(window_width / 2, window_height / 2);

	vec3 fragPos = texture(g_position, TexCoords).xyz;
	vec3 normal = texture(g_normal, TexCoords).rgb;
	vec3 randomVec = texture(tex_noise, TexCoords * noise_scale).xyz;

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);  

	float occlusion = 0.0;
    for (int i = 0; i < kernelSize; i++) {
        vec3 samplePos = TBN * samples[i];
        samplePos = fragPos + samplePos * radius; 
        
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection_matrix * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
        
        float sampleDepth = texture(g_position, offset.xy).z;
        
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion /= kernelSize;
    
    FragColor = occlusion;
};