#pragma once
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "GameRect.h"
#include "Camera.h"
#include "Light.h"
#include "Framebuffer.h"
#include <functional>
#include <set>
#include <mutex>
#include "GameTexture.h"
#include "ScriptFunc.h"
#include "CircularBuffer.h"

#define MAX_LIGHT_SOURCES 10

class WindowManager {
public:
	WindowManager(WindowManager& other) = delete;
	void operator=(const WindowManager& other) = delete;

	Light* add_light(Light light);
	Light* add_light(glm::vec3 pos, glm::vec3 col, float brightness);
	void remove_lights();

	void dim_lights(float dim_mul, Shader** shader);

	void start_fade_sequence(unsigned char fade_frames, std::function<void()> mid_fade_func);

	void update_shader_lights();
	void update_shader_cams();
	void update_framebuffer_dimensions();

	void set_resolution(int width, int height);

	void reset_gl_environment();

	void buffer_event(std::string name, std::function<void(ScriptArg)> function, ScriptArg buffer_arg = ScriptArg());
	void execute_buffered_events();

	void render_ssao();
	void render_bloom();
	void render_debug_textures();

	void update_screen();
	void clear_screen();

	GLFWwindow* window;

	Camera camera;
	glm::vec3 ambient_col;
	std::vector<Light>lights;

	std::vector<std::function<void(ScriptArg)>> buffered_events;
	std::vector<ScriptArg> buffered_args;
	std::set<std::string> event_names;
	std::mutex event_mutex;

	Framebuffer g_buffer;
	Framebuffer hdr_buffer;
	Framebuffer SSAO;
	Framebuffer blur;
	Framebuffer box_layer;
	Framebuffer shadow_map;

	std::vector<GameTexture> debug_textures;
	GameTexture fade_texture;

	float hdr_exposure;
	std::vector<glm::vec3> ssao_samples;
	std::vector<glm::vec3> ssao_noise;
	float shadow_fov;
	float shadow_depth;

	int res_width;
	int res_height;

	int window_width;
	int window_height;

	bool normals_enabled;

	static WindowManager* get_instance();
	void destroy_instance();
private:
	WindowManager();
	static WindowManager* instance;

	unsigned char fade_frames;
	std::function<void()> mid_fade_func;
	bool fading;
};

void window_resize_callback(GLFWwindow* window, int width, int height);
void window_close_callback(GLFWwindow* window);
void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);