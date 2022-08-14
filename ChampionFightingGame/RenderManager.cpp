#include "GameManager.h"
#include "RenderManager.h"
#include <string>
#include "SaveManager.h"
#include "utils.h"
#include "stb_image.h"
#include <iostream>

RenderManager::RenderManager() {
	SaveManager* save_manager = SaveManager::get_instance();
	float width = save_manager->get_game_setting("res_x");
	float height = save_manager->get_game_setting("res_y");
	if (save_manager->get_game_setting("fullscreen")) {
		window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	}
	SDL_GetWindowSize(window, &s_window_width, &s_window_height);
	sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
	sdl_context = SDL_GL_CreateContext(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW!" << std::endl;
	}
	SDL_GL_MakeCurrent(window, sdl_context);
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	stbi_set_flip_vertically_on_load(true);
	glClearColor(0.1, 0.1, 0.1, 0.0);
	glClearStencil(0);

	for (int i = 0; i < 16; i++) {
		glm::vec3 sample(rng_f(0.0, 1.0) * 2.0 - 1.0, rng_f(0.0, 1.0) * 2.0 - 1.0, rng_f(0.0, 1.0));

		sample = glm::normalize(sample);
		sample *= rng_f(0.0, 1.0);

		float scale = (float)i / 16.0;
		scale = lerp(0.1, 1.0, scale * scale);
		sample *= scale;

		ssao_kernel.push_back(sample);
	}

	for (int i = 0; i < 4; i++) {
		glm::vec3 noise(
			rng_f(0.0, 1.0) * 2.0 - 1.0,
			rng_f(0.0, 1.0) * 2.0 - 1.0,
			0.0
		);
		ssao_noise.push_back(noise);
	}

	shadow_map.init("vertex_shadow.glsl", "fragment_shadow.glsl");
	shadow_map.add_write_texture(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_REPEAT, 2000, 2000, GL_DEPTH_ATTACHMENT, 0, false);

	outline.init("vertex_passthrough.glsl", "fragment_passthrough.glsl");
	outline.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, width, height, GL_COLOR_ATTACHMENT0, 0);

	box_layer.init("vertex_passthrough.glsl", "fragment_passthrough.glsl");
	box_layer.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, width, height, GL_COLOR_ATTACHMENT0, 0);

	g_buffer.init("vertex_gbuffer.glsl", "fragment_gbuffer.glsl");
	g_buffer.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, width, height, GL_COLOR_ATTACHMENT0, 0); //Position
	g_buffer.add_write_texture(GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_CLAMP_TO_EDGE, width, height, GL_COLOR_ATTACHMENT1, 1); //Normal
	g_buffer.add_write_texture(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, width, height, GL_COLOR_ATTACHMENT2, 2); //Diffuse
	g_buffer.add_write_texture(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_CLAMP_TO_EDGE, width, height, GL_COLOR_ATTACHMENT3, 3); //Specular
	
	SSAO.init("vertex_ssao.glsl", "fragment_ssao.glsl");
	SSAO.add_write_texture(GL_RED, GL_RED, GL_FLOAT, GL_CLAMP_TO_EDGE, width, height, GL_COLOR_ATTACHMENT0, 0); //Output
	SSAO.add_read_texture(GL_RGBA16F, GL_RGB, GL_FLOAT, GL_REPEAT, 2, 2, 1, (void*)&ssao_noise[0]); //Noise
	SSAO.add_read_texture(g_buffer.textures[0], 2); //Position, shared w/ GBuffer
	SSAO.add_read_texture(g_buffer.textures[1], 3); //Ditto for Normals

	SSAO_blur.init("vertex_ssao_blur.glsl", "fragment_ssao_blur.glsl");
	SSAO_blur.add_read_texture(SSAO.textures[0], 0);
	g_buffer.add_read_texture(SSAO.textures[0], 4);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	box_layer.shader.use();
	box_layer.shader.set_int("f_texture", 0);

	g_buffer.shader.use();
	g_buffer.shader.set_int("g_position", 0);
	g_buffer.shader.set_int("g_normal", 1);
	g_buffer.shader.set_int("g_diffuse", 2);
	g_buffer.shader.set_int("g_specular", 3);
	g_buffer.shader.set_int("ssao", 4);

	SSAO.shader.use();
	for (int i = 0; i < 16; i++) {
		SSAO.shader.set_vec3("samples[]", ssao_kernel[i], i);
	}
	SSAO.shader.set_int("window_width", s_window_width);
	SSAO.shader.set_int("window_height", s_window_height);
	SSAO.shader.set_int("tex_noise", 1);
	SSAO.shader.set_int("g_position", 2);
	SSAO.shader.set_int("g_normal", 3);

	SSAO_blur.shader.use();
	SSAO_blur.shader.set_int("ssao", 0);

	game_texture_shader.init("vertex_2d_texture.glsl", "fragment_2d_texture.glsl");
	rect_shader.init("vertex_rect.glsl", "fragment_rect.glsl");
	effect_shader.init("vertex_effect.glsl", "fragment_effect.glsl");
	text_shader.init("vertex_text.glsl", "fragment_text.glsl");
	passthrough_shader.init("vertex_passthrough.glsl", "fragment_passthrough.glsl");

	brightness_mul = 1.0;
}

void RenderManager::add_light(Light *light, int target) {
	if (target == -1) {
		if (lights.size() == MAX_LIGHT_SOURCES) {
			GameManager::get_instance()->add_crash_log("Congrats you stupid idiot, you ran out of lights\n");
			return;
		}
		else {
			lights.push_back(light);
		}
	}
	else {
		if (lights.size() <= target) {
			lights.resize(target);
			lights.push_back(light);
		}
		else {
			lights[target] = light;
		}
	}
	buffer_event("Shader Light", [this](void* arg) {
		update_shader_lights();
	});
}

void RenderManager::remove_light(int target) {
	if (target == -1) {
		lights.clear();
	}
	else {
		for (int i = target, max = lights.size() - 1; i < max; i++) {
			lights[i] = lights[i + 1];
		}
		lights.pop_back();
	}
	buffer_event("Shader Light", [this](void* arg) {
		update_shader_lights();
	});
}

void RenderManager::dim_lights(float brightness_mul) {
	this->brightness_mul = brightness_mul;
	buffer_event("", [this](void* arg) {
		for (int i = 0, max = linked_shaders.size(); i < max; i++) {
			linked_shaders[i]->use();
			linked_shaders[i]->set_float("brightness_mul", this->brightness_mul);
		}
	});
}

//When dim_lights gets called, all shaders are updated with the new brightness multiplier. This
//includes the shader belonging to the fighter that called it, so if we want to emphasize said fighter
//during a super freeze, we call this function to keep them at a normal brightness

//This function is also literally the only reason RenderObjects can't share shaders
void RenderManager::undim_shader(Shader* shader) {
	buffer_event("", [this](void* arg) {
		Shader* shader = (Shader*)arg;
		shader->use();
		shader->set_float("brightness_mul", 1.0);

	}, (void*)shader);
}

void RenderManager::link_shader(Shader *shader) {
	linked_shaders.push_back(shader);
}

void RenderManager::unlink_all_shaders() {
	linked_shaders.clear();
}

void RenderManager::update_shader_lights() {
	glm::vec3 shadow_total = glm::vec3(0.0);
	float shadow_factor = 0.0;
	g_buffer.shader.use();
	for (int i = 0, max = MAX_LIGHT_SOURCES; i < max; i++) {
		if (i < lights.size()) {
			g_buffer.shader.set_vec3("light[0].position", lights[i]->position, i);
			g_buffer.shader.set_vec3("light[0].color", lights[i]->color, i);
			g_buffer.shader.set_float("light[0].linear", lights[i]->linear, i);
			g_buffer.shader.set_float("light[0].quadratic", lights[i]->quadratic, i);

			g_buffer.shader.set_bool("light[0].enabled", lights[i]->enabled, i);
			if (lights[i]->enabled) {
				shadow_total += lights[i]->position;
				shadow_factor++;
			}
		}
		else {
			g_buffer.shader.set_bool("lights[0].enabled", false, i);
		}
	}
	if (lights.empty()) {
		shadow_map.light_pos = glm::vec3(0.0, 1.0, 1.0);
	}
	else {
		shadow_map.light_pos = shadow_total / glm::vec3(shadow_factor);
	}
	shadow_map.update_light_pos();
}

void RenderManager::update_shader_cams() {
	glm::mat4& camera_matrix = camera.camera_matrix;
	glm::mat4& projection_matrix = camera.projection_matrix;
	glm::mat4 &view_matrix = camera.view_matrix;
	SSAO.shader.use();
	SSAO.shader.set_mat4("projection_matrix", projection_matrix);
	rect_shader.use();
	rect_shader.set_mat4("camera_matrix", camera_matrix);
	effect_shader.use();
	effect_shader.set_mat4("camera_matrix", camera_matrix);
	for (int i = 0, max = linked_shaders.size(); i < max; i++) {
		linked_shaders[i]->use();
		linked_shaders[i]->set_vec3("view_pos", camera.pos);
		linked_shaders[i]->set_mat4("camera_matrix", camera_matrix);
		linked_shaders[i]->set_mat4("view_matrix", view_matrix);
	}
}

void RenderManager::update_shader_shadows() {
	glm::mat4 shadow_matrix = (shadow_map.projection_matrix * shadow_map.view_matrix);
	for (int i = 0, max = linked_shaders.size(); i < max; i++) {
		linked_shaders[i]->use();
		linked_shaders[i]->set_mat4("shadow_matrix", shadow_matrix);
	}
}

void RenderManager::update_framebuffer_dimensions() {
	outline.update_dimensions();
	box_layer.update_dimensions();
	g_buffer.update_dimensions();
	SSAO.update_dimensions();
	SSAO_blur.update_dimensions();
	SSAO.shader.use();
	SSAO.shader.set_int("window_width", s_window_width);
	SSAO.shader.set_int("window_height", s_window_height);
}

void RenderManager::refresh_sdl_renderer() {
	SDL_RenderClear(sdl_renderer);
	SDL_DestroyRenderer(sdl_renderer);
	sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
}

void RenderManager::buffer_event(std::string name, std::function<void(void*)> function, void* buffered_arg) {
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

void RenderManager::execute_buffered_events() {
	for (int i = 0, max = buffered_events.size(); i < max; i++) {
		buffered_events[i](buffered_args[i]);
	}
	buffered_events.clear();
	buffered_args.clear();
	event_names.clear();
}

RenderManager* RenderManager::instance = nullptr;
RenderManager* RenderManager::get_instance() {
	if (instance == nullptr) {
		instance = new RenderManager;
	}
	return instance;
}

void RenderManager::destroy_instance() {
	unlink_all_shaders();
	game_texture_shader.destroy();
	effect_shader.destroy();
	rect_shader.destroy();
	text_shader.destroy();
	box_layer.destroy();
	g_buffer.destroy();
	SSAO.destroy();
	SSAO_blur.destroy();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(sdl_renderer);
	SDL_GL_DeleteContext(sdl_context);
	if (instance != nullptr) {
		delete instance;
	}
}