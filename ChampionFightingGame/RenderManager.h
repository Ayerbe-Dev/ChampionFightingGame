#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "GameRect.h"
#include "Camera.h"

#define MAX_LIGHT_SOURCES 10

class Light {
public:
	Light(glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0));

	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

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
	glm::vec3 window_scaler = glm::vec3(1.0);

	Shader default_2d_shader;
	Shader default_rect_shader;

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

	void render_model(Model *model, Shader *shader, glm::mat4 extra_mat, glm::vec3 *pos, glm::vec3 *rot, glm::vec3 *scale);
};