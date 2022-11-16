#pragma once
#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include "Shader.h"
#include "Model.h"
#include "GameRect.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowMap.h"
#include "Framebuffer.h"
#include <functional>
#include <set>
#include <mutex>
#include "GameTexture.h"
#include "ScriptArg.h"

#define MAX_LIGHT_SOURCES 10

class RenderManager {
public:
	RenderManager(RenderManager& other) = delete;
	void operator=(const RenderManager& other) = delete;

	void add_light(Light* light, int target = -1);
	void remove_light(int target = -1);

	void dim_lights(float brightness_mul, Shader** shader);

	void update_shader_lights();
	void update_shader_cams();
	void update_shader_shadows();
	void update_framebuffer_dimensions();

	void reset_gl_environment();
	void refresh_sdl_renderer();

	void buffer_event(std::string name, std::function<void(ScriptArg)> function, ScriptArg buffer_arg = ScriptArg());
	void execute_buffered_events();

	SDL_Window* window;
	SDL_Renderer* sdl_renderer;
	SDL_GLContext sdl_context;

	Camera camera;
	std::vector<Light*>lights;

	std::vector<std::function<void(ScriptArg)>> buffered_events;
	std::vector<ScriptArg> buffered_args;
	std::set<std::string> event_names;
	std::mutex event_mutex;

	ShadowMap shadow_map;
	Framebuffer outline;
	Framebuffer box_layer;
	Framebuffer g_buffer;
	Framebuffer SSAO;
	Framebuffer SSAO_blur;

	std::vector<glm::vec3> ssao_kernel;
	std::vector<glm::vec3> ssao_noise;

	int s_window_width;
	int s_window_height;

	static RenderManager* get_instance();
	void destroy_instance();
private:
	RenderManager();
	static RenderManager* instance;
};