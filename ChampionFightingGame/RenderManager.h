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

	Shader bg_shader;
	unsigned int bg_texture;
	unsigned int VAO;
	unsigned int VBO;

	const float screen_coords[8] = {
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0
	};

	RenderManager();

	void init();

	void add_light(Light light, int target = -1);
	void remove_light(int target = -1);

	void update_shader_lights(Shader *shader);
	void update_shader_cam(Shader* shader);

	void render_sdl_as_gl(SDL_Texture* background);
	void render(Model *model, Shader *shader, vec3 *pos, vec3 *rot, vec3 *scale);
};