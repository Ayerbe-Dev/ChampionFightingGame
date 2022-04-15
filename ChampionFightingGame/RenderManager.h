#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "GameRect.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowMap.h"

#define MAX_LIGHT_SOURCES 5

class RenderManager {
public:
	RenderManager(RenderManager& other) = delete;
	void operator=(const RenderManager& other) = delete;

	Camera camera;
	Light lights[MAX_LIGHT_SOURCES];
	int num_lights;
	glm::vec3 window_scaler = glm::vec3(1.0);
	std::vector<Shader*> linked_shaders;

	Shader default_2d_shader;
	Shader default_rect_shader;
	Shader shadow_shader;

	ShadowMap shadow_map;

	int s_window_width, s_window_height;

	const float screen_coords[8] = {
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0
	};

	void init();

	void add_light(Light light, int target = -1);
	void remove_light(int target = -1);

	void link_shader(Shader *shader);
	void unlink_all_shaders();

	void update_shader_lights();
	void update_shader_cam(Shader* shader);

	void render_model(Model *model, Shader *shader, glm::mat4 extra_mat, glm::vec3 *pos, glm::vec3 *rot, glm::vec3 *scale, bool flip);
	void render_model_shadow(Model *model, glm::mat4 extra_mat, glm::vec3 *pos, glm::vec3 *rot, glm::vec3 *scale, bool flip);

	//complete scuffedness
	static RenderManager* get_instance();
private:
	RenderManager();
	static RenderManager* instance;
};