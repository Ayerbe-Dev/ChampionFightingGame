#include "WindowManager.h"
#include "FontManager.h"
#include "GameManager.h"
#include "InputManager.h"
#include "SaveManager.h"
#include "ShaderManager.h"
#include "utils.h"
#include "stb_image.h"
#include <iostream>
#include <random>
#include <string>
#include "debug.h"
#ifdef DEBUG
#include "cotr_imgui_debugger.h"
#endif

WindowManager::WindowManager() {
	SaveManager* save_manager = SaveManager::get_instance();
	res_width = save_manager->get_game_setting("res_x");
	res_height = save_manager->get_game_setting("res_y");
	if (save_manager->get_game_setting("fullscreen") == 1) {
		window = glfwCreateWindow(res_width, res_height, "Champions of the Ring", glfwGetPrimaryMonitor(), nullptr);
	}
	else {
		window = glfwCreateWindow(res_width, res_height, "Champions of the Ring", nullptr, nullptr);
	}
	glfwMakeContextCurrent(window);
	glfwGetWindowSize(window, &window_width, &window_height);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW!\n";
	}

	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(window, window_resize_callback);
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetKeyCallback(window, window_key_callback);

	glViewport(0, 0, window_width, window_height);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	stbi_set_flip_vertically_on_load(true);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	hdr_exposure = 1.5;
	int num_ssao_samples = 16;
	for (int i = 0; i < num_ssao_samples; i++) {
		glm::vec3 sample = glm::normalize(
			glm::vec3(
				rng_f(0.0, 1.0) * 2.0 - 1.0, 
				rng_f(0.0, 1.0) * 2.0 - 1.0, 
				rng_f(0.0, 1.0)
			)
		) * rng_f(0.0, 1.0);

		float scale = (float)i / (float)num_ssao_samples;
		scale = lerp(0.1, 1.0, scale * scale);
		sample *= scale;
		ssao_samples.push_back(sample);
	}
	for (int i = 0; i < 4; i++) {
		glm::vec3 noise(
			rng_f(0.0, 1.0) * 2.0 - 1.0,
			rng_f(0.0, 1.0) * 2.0 - 1.0,
			0.0
		);
		ssao_noise.push_back(noise);
	}
	shadow_fov = 100.0f;
	shadow_depth = 256.0f;

	g_buffer.init("passthrough", "gbuffer", "", 
		SHADER_FEAT_DIFFUSE 
		| SHADER_FEAT_NORMAL
		| SHADER_FEAT_SSAO
		| SHADER_FEAT_OUTLINE, res_width, res_height
	);
	g_buffer.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, res_width, res_height, GL_COLOR_ATTACHMENT0, 0); //Diffuse
	g_buffer.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, res_width, res_height, GL_COLOR_ATTACHMENT1, 1); //Position + Outline
	g_buffer.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, res_width, res_height, GL_COLOR_ATTACHMENT2, 2); //Normal + Specular
	g_buffer.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, res_width, res_height, GL_COLOR_ATTACHMENT3, 3); //Emission

	hdr_buffer.init("passthrough", "hdr", "", SHADER_FEAT_BLOOM, res_width, res_height);
	hdr_buffer.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, res_width, res_height, GL_COLOR_ATTACHMENT0, 0);

	SSAO.init("passthrough", "ssao", "", 0, res_width, res_height);
	SSAO.add_write_texture(GL_RED, GL_RED, GL_FLOAT, GL_CLAMP_TO_EDGE, res_width, res_height, GL_COLOR_ATTACHMENT0, 0); //Output
	SSAO.add_read_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_REPEAT, 2, 2, 1, (void*)&ssao_noise[0]); //Noise
	SSAO.add_read_texture(g_buffer.textures[1], 2); //Position, shared w/ GBuffer
	SSAO.add_read_texture(g_buffer.textures[2], 3); //Ditto for Normals

	blur.init("passthrough", "blur", "", 0, res_width, res_height);
	blur.add_write_texture(GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, res_width, res_height, GL_COLOR_ATTACHMENT0, 0); //Output

	box_layer.init("passthrough", "passthrough", "", 0, res_width, res_height);
	box_layer.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, res_width, res_height, GL_COLOR_ATTACHMENT0, 0);

	shadow_map.init("shadow", "shadow", "", 0, res_width, res_height);
	shadow_map.add_write_texture(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_EDGE, 2048, 2048, GL_DEPTH_ATTACHMENT, 0, false);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	g_buffer.add_uniform("g_diffuse", 0);
	g_buffer.add_uniform("g_pos_outline", 1);
	g_buffer.add_uniform("g_normal_spec", 2);

	hdr_buffer.add_uniform("g_buffer_out", 0);

	hdr_buffer.shader->use();
	hdr_buffer.shader->set_float("exposure", hdr_exposure);

	SSAO.shader->use();
	for (int i = 0; i < num_ssao_samples; i++) {
		SSAO.shader->set_vec3("samples[]", ssao_samples[i], i);
	}
	SSAO.add_uniform("tex_noise", 1);
	SSAO.add_uniform("g_position", 2);
	SSAO.add_uniform("g_normal", 3);

	box_layer.add_uniform("f_texture", 0);

	g_buffer.bind_uniforms();
	hdr_buffer.bind_uniforms();
	SSAO.bind_uniforms();
	box_layer.bind_uniforms();

	debug_textures.push_back(ScreenTexture(g_buffer.textures[0]));
	debug_textures.push_back(ScreenTexture(g_buffer.textures[1]));
	debug_textures.push_back(ScreenTexture(g_buffer.textures[2]));
	debug_textures.push_back(ScreenTexture(blur.textures[0]));
	for (int i = 0, max = debug_textures.size(); i < max; i++) {
		debug_textures[i].set_scale(0.2).set_pos(glm::vec3(0.0, 250 * i, 0.0)).set_orientation(TEXTURE_BOTTOM_RIGHT);
	}
	fade_texture.init("resource/misc/black.png").set_alpha(0);
	fade_frames = 0;
	fading = false;
	mid_fade_func = nullptr;

	normals_enabled = true;

	ShaderManager* shader_manager = ShaderManager::get_instance();
	shader_manager->set_global_int("WindowWidth", window_width);
	shader_manager->set_global_int("WindowHeight", window_height);
	shader_manager->set_global_mat4("ShadowMatrix",
		glm::ortho(-shadow_fov, shadow_fov, -shadow_fov, shadow_fov, 0.1f, shadow_depth) *
		glm::mat4(
			1.0, 0.0, 0.0, 0.0,
			0.0, 0.707107, 0.707107, 0.0,
			0.0, -0.707107, 0.707107, 0.0,
			0.0, 0.0, -1.414214, 1.0
		)
	);

	ambient_col = glm::vec3(1.0);
}

Light* WindowManager::add_light(Light light) {
	if (lights.size() == MAX_LIGHT_SOURCES) {
		GameManager::get_instance()->add_crash_log("Congrats you stupid idiot, you ran out of lights\n");
		return nullptr;
	}
	else {
		lights.push_back(light);
	}
	buffer_event("Shader Light", [this](ScriptArg args) {
		update_shader_lights();
	});
	return &lights.back();
}

Light* WindowManager::add_light(glm::vec3 pos, glm::vec3 col, float brightness) {
	return add_light(Light(pos, col, brightness));
}

void WindowManager::remove_lights() {
	lights.clear();
	buffer_event("Shader Light", [this](ScriptArg args) {
		update_shader_lights();
	});
}

void WindowManager::dim_lights(float dim_mul, Shader** shader) {
	ScriptArg args;
	args.push_arg(dim_mul);
	args.push_arg(shader);
	buffer_event("", [this](ScriptArg args) {
		ShaderManager* shader_manager = ShaderManager::get_instance();
		UNWRAP(dim_mul, float);
		UNWRAP(shader, Shader**);
		shader_manager->set_global_float("DimMul", dim_mul);
		if (shader != nullptr) {
			*shader = shader_manager->get_shader_switch_features(*shader, SHADER_FEAT_DIM_MUL, 0);
		}
	}, args);
}

void WindowManager::start_fade_sequence(unsigned char fade_frames, std::function<void()> mid_fade_func) {
	if (fading) return;
	fade_texture.set_alpha(255, fade_frames);
	this->fade_frames = fade_frames;
	this->mid_fade_func = mid_fade_func;
	fading = true;
}

void WindowManager::update_shader_lights() {
	ShaderManager* shader_manager = ShaderManager::get_instance();
	glm::vec3 shadow_total = glm::vec3(0.0);
	float shadow_factor = 0.0;
	g_buffer.shader->use();
	g_buffer.shader->set_vec3("ambient", ambient_col);

	for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
		if (i < lights.size()) {
			g_buffer.shader->set_vec3("light[0].position", glm::vec4(lights[i].position, 1.0f) * camera.view_matrix, i);
			g_buffer.shader->set_vec3("light[0].color", lights[i].color * glm::vec3(lights[i].brightness), i);

			g_buffer.shader->set_bool("light[0].enabled", lights[i].enabled, i);
			if (lights[i].enabled) {
				shadow_total += lights[i].position;
				shadow_factor++;
			}
		}
		else {
			g_buffer.shader->set_bool("light[0].enabled", false, i);
		}
	}
	if (lights.empty()) {
		shader_manager->set_global_mat4("ShadowMatrix", 
			glm::ortho(-shadow_fov, shadow_fov, -shadow_fov, shadow_fov, 0.1f, shadow_depth) *
			glm::mat4(
				1.0, 0.0, 0.0, 0.0, 
				0.0, 0.707107, 0.707107, 0.0, 
				0.0, -0.707107, 0.707107, 0.0, 
				0.0, 0.0, -1.414214, 1.0
			)
		);
		shader_manager->set_global_vec3("ShadowLight", glm::vec3(0.0, 1.0, 1.0));
	}
	else {
		shadow_total /= glm::vec3(shadow_factor);
		shader_manager->set_global_mat4("ShadowMatrix", 
			glm::ortho(-shadow_fov, shadow_fov, -shadow_fov, shadow_fov, 0.1f, shadow_depth) *
			glm::lookAt(shadow_total, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0))
		);
		shader_manager->set_global_vec3("ShadowLight", shadow_total);
	}
}

void WindowManager::update_shader_cams() {
	ShaderManager* shader_manager = ShaderManager::get_instance();
	glm::mat4& camera_matrix = camera.camera_matrix;
	glm::mat4& projection_matrix = camera.projection_matrix;
	glm::mat4& view_matrix = camera.view_matrix;

	shader_manager->set_global_vec3("ViewPos", camera.pos);
	shader_manager->set_global_mat4("CameraMatrix", camera_matrix);
	shader_manager->set_global_mat4("ProjectionMatrix", projection_matrix);
	shader_manager->set_global_mat4("ViewMatrix", view_matrix);
}

void WindowManager::update_framebuffer_dimensions() {
	ShaderManager* shader_manager = ShaderManager::get_instance();
	shader_manager->set_global_int("WindowWidth", window_width);
	shader_manager->set_global_int("WindowHeight", window_height);
	g_buffer.update_dimensions();
	hdr_buffer.update_dimensions();
	SSAO.update_dimensions();
	blur.update_dimensions();
	box_layer.update_dimensions();
}

void WindowManager::set_resolution(int width, int height) {
	this->res_width = width;
	this->res_height = height;
	glfwSetWindowSize(window, width, height);
}

void WindowManager::reset_gl_environment() {
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	buffered_events.clear();
	buffered_args.clear();
	event_names.clear();

	camera.set_fov(45.0);
	camera.update_view();
	ambient_col = glm::vec3(1.0);
	remove_lights();
	execute_buffered_events();
}

void WindowManager::buffer_event(std::string name, std::function<void(ScriptArg)> function, ScriptArg buffered_arg) {
	event_mutex.lock();
	if (name == "" || !event_names.contains(name)) {
		buffered_events.push_back(function);
		buffered_args.push_back(buffered_arg);
		if (name != "") {
			event_names.insert(name);
		}
	}
	event_mutex.unlock();
}

void WindowManager::execute_buffered_events() {
	for (int i = 0, max = buffered_events.size(); i < max; i++) {
		buffered_events[i](buffered_args[i]);
	}
	buffered_events.clear();
	buffered_args.clear();
	event_names.clear();
}

void WindowManager::render_ssao() {
	if (g_buffer.shader->features & SHADER_FEAT_SSAO) {
		SSAO.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SSAO.render();

		blur.use();
		blur.shader->use();
		blur.shader->set_int("blur_amount", 2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		blur.bind_ex_uniforms({ {"f_texture", SSAO.textures[0]} });
		blur.render();
		g_buffer.bind_ex_uniforms({ {"ssao", blur.textures[0]} });
	}
}

void WindowManager::render_bloom() {
	if (hdr_buffer.shader->features & SHADER_FEAT_BLOOM) {
		blur.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		blur.bind_ex_uniforms({ {"f_texture", g_buffer.textures[3]} });
		blur.shader->use();
		blur.shader->set_int("blur_amount", 5);
		blur.render();
		hdr_buffer.bind_ex_uniforms({ {"bloom", blur.textures[0]} });
	}

}

void WindowManager::render_debug_textures() {
	for (int i = 0, max = debug_textures.size(); i < max; i++) {
		debug_textures[i].render();
	}
}

void WindowManager::update_screen() {
	if (fading) {
		if (fade_texture.get_alpha() == 0 && fade_frames == 0) {
			fading = false;
		}
		if (fade_texture.get_alpha() == 255) {
			fade_texture.set_alpha(0, fade_frames);
			mid_fade_func();
			mid_fade_func = nullptr;
			fade_frames = 0;
		}

		fade_texture.render();
	}
#ifdef DEBUG
	render_debug_textures();
#endif
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void WindowManager::clear_screen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

WindowManager* WindowManager::instance = nullptr;
WindowManager* WindowManager::get_instance() {
	if (instance == nullptr) {
		instance = new WindowManager;
	}
	return instance;
}

void WindowManager::destroy_instance() {
	box_layer.destroy();
	g_buffer.destroy();
	SSAO.destroy();
	blur.destroy();

	for (int i = 0, max = debug_textures.size(); i < max; i++) {
		debug_textures[i].destroy();
	}
	debug_textures.clear();
	fade_texture.destroy();

	if (instance != nullptr) {
		delete instance;
	}
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
	WindowManager* window_manager = WindowManager::get_instance();
	window_manager->res_width = width;
	window_manager->res_height = height;
	glfwGetWindowSize(window, &window_manager->window_width, &window_manager->window_height);
	glViewport(0, 0, window_manager->window_width, window_manager->window_height);
	window_manager->update_framebuffer_dimensions();
}

void window_close_callback(GLFWwindow* window) {
	GameManager::get_instance()->update_scene(SCENE_CLOSE);
}

void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	InputManager* input_manager = InputManager::get_instance();
	if (input_manager->is_using_text_input()) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			input_manager->modify_text_input(key, mods);
		}
		return;
	}
	input_manager->keyboard_state[key] = (bool)action;
}