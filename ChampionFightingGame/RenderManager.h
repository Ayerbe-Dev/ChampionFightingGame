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

#define MAX_LIGHT_SOURCES 10

class RenderManager {
public:
	RenderManager(RenderManager& other) = delete;
	void operator=(const RenderManager& other) = delete;

	SDL_Window* window;
	SDL_Renderer* sdl_renderer;
	SDL_GLContext sdl_context;

	Camera camera;
	std::vector<Light*>lights;
	std::vector<Shader*> linked_shaders;

	Shader game_texture_shader;
	Shader rect_shader;
	Shader effect_shader;
	Shader text_shader;
	Shader shadow_shader;

	ShadowMap shadow_map;
	OverlayLayer box_layer;

	int s_window_width;
	int s_window_height;

	void add_light(Light *light, int target = -1);
	void remove_light(int target = -1);

	void link_shader(Shader *shader);
	void unlink_all_shaders();

	void update_shader_lights();
	void update_shader_cams();
	void update_shader_shadows();

	void refresh_sdl_renderer();

	static RenderManager* get_instance();
	void destroy_instance();
private:
	RenderManager();
	static RenderManager* instance;
};