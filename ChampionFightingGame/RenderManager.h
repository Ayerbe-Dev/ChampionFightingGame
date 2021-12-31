#pragma once
#include "utils.h"
#include "Shader.h"
#include "Model.h"
using namespace glm;

class Light {
public:
	Light(vec3 pos = vec3(0.0, 0.0, 0.0));

	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	bool enabled;
};

class RenderManager {
public:
	Camera camera;
	Light lights[MAX_LIGHT_SOURCES];
	int num_lights;
	Shader shaders[MAX_SHADERS];

	RenderManager();

	void add_light(Light light, int target = -1);
	void remove_light(int target = -1);

	void update_shader(Shader *shader);

	void render(Model *model, Shader *shader, vec3 *pos, vec3 *rot, vec3 *scale);
};