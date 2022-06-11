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
#include <functional>
#include <set>

#define MAX_LIGHT_SOURCES 10

class RenderManager {
public:
	RenderManager(RenderManager& other) = delete;
	void operator=(const RenderManager& other) = delete;

	void add_light(Light* light, int target = -1);
	void remove_light(int target = -1);

	void dim_lights(float brightness_mul);
	void undim_shader(Shader* shader);

	void link_shader(Shader* shader);
	void unlink_all_shaders();

	void update_shader_lights();
	void update_shader_cams();
	void update_shader_shadows();

	void refresh_sdl_renderer();

	void buffer_event(std::string name, std::function<void(void*)> function, void* buffer_arg = (void*)nullptr);
	void execute_buffered_events();

	SDL_Window* window;
	SDL_Renderer* sdl_renderer;
	SDL_GLContext sdl_context;

	Camera camera;
	std::vector<Light*>lights;
	std::vector<Shader*> linked_shaders;

	std::vector<std::function<void(void*)>> buffered_events;
	std::vector<void*> buffered_args;
	std::set<std::string> event_names;

	Shader game_texture_shader;
	Shader rect_shader;
	Shader effect_shader;
	Shader text_shader;
	Shader shadow_shader;

	ShadowMap shadow_map;
	OverlayLayer box_layer;

	int s_window_width;
	int s_window_height;

	float brightness_mul;

	static RenderManager* get_instance();
	void destroy_instance();
private:
	RenderManager();
	static RenderManager* instance;
};