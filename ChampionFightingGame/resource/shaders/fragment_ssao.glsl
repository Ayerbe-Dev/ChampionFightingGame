#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

const int kernelSize = 16;
uniform float radius = 5.0;
uniform float bias = 0.01;

uniform sampler2D g_position;
uniform sampler2D g_normal;
uniform sampler2D tex_noise;

uniform vec3 samples[kernelSize];

layout(std140) uniform WindowWidth {
    int window_width;
};

layout(std140) uniform WindowHeight {
    int window_height;
};

layout(std140) uniform ProjectionMatrix {
    mat4 projection_matrix;
};

void main() {
	vec2 noise_scale = vec2(window_width / 2, window_height / 2);

    vec4 pos_outline = texture(g_position, TexCoords);
	vec3 fragPos = pos_outline.xyz / pos_outline.w;
	vec3 normal = normalize(texture(g_normal, TexCoords).rgb);
	vec3 randomVec = normalize(texture(tex_noise, TexCoords * noise_scale).xyz);

	vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);
    
    float occlusion = 0.0;
    int div = kernelSize;
    for (int i = 0; i < kernelSize; i++) {
        vec3 samplePos = TBN * samples[i];
        if (dot(samplePos, normal) < 0.05) {
            div--;
            continue;
        }
        samplePos = fragPos + samplePos * radius; 
        
        vec4 offset = vec4(samplePos, 1.0);
        offset = projection_matrix * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;
        
        float sampleDepth = texture(g_position, offset.xy).z / texture(g_position, offset.xy).w;
        
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= samplePos.z + bias ? 1.0 : 0.0) * rangeCheck;
    }
    if (div != 0) {
        occlusion /= div;
        FragColor = vec4(1.0 - occlusion, 0.0, 0.0, 1.0);
    }
    else {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
};