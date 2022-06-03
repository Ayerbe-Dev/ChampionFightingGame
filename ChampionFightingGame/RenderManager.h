#pragma once
#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include "Shader.h"
#include "Model.h"
#include "GameRect.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowMap.h"
#include "OverlayLayer.h"

#define MAX_LIGHT_SOURCES 5

class RenderManager {
public:
	RenderManager(RenderManager& other) = delete;
	void operator=(const RenderManager& other) = delete;

	SDL_Window* window;
	SDL_Renderer* sdl_renderer;
	SDL_GLContext sdl_context;

	Camera camera;
	Light lights[MAX_LIGHT_SOURCES];
	int num_lights;
	std::vector<Shader*> linked_shaders;

	Shader default_2d_shader;
	Shader default_rect_shader;
	Shader default_effect_shader;
	Shader shadow_shader;

	ShadowMap shadow_map;
	OverlayLayer box_layer;

	int s_window_width;
	int s_window_height;

	void init();
	void destroy();

	void add_light(Light light, int target = -1);
	void remove_light(int target = -1);

	void link_shader(Shader *shader);
	void unlink_all_shaders();

	void update_shader_lights();
	void update_shader_cam(Shader* shader);

	void render_model(Model *model, Shader *shader, glm::mat4 extra_mat, glm::vec3 *pos, glm::vec3 *rot, glm::vec3 *scale, bool flip);
	void render_model_shadow(Model *model, glm::mat4 extra_mat, glm::vec3 *pos, glm::vec3 *rot, glm::vec3 *scale, bool flip);

	void refresh_sdl_renderer();

	static RenderManager* get_instance();
private:
	RenderManager();
	static RenderManager* instance;
};