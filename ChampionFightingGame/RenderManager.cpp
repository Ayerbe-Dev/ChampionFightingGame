#include "GameManager.h"
#include "RenderManager.h"
#include <string>
#include "SaveManager.h"
#include "utils.h"
#include "stb_image.h"

RenderManager::RenderManager() {
	init();
}

void RenderManager::init() {
	SaveManager* save_manager = SaveManager::get_instance();
	if (save_manager->get_game_setting("fullscreen")) {
		window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, save_manager->get_game_setting("res_x"), save_manager->get_game_setting("res_y"), SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {
		window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, save_manager->get_game_setting("res_x"), save_manager->get_game_setting("res_y"), SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	stbi_set_flip_vertically_on_load(true);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	shadow_map.init();
	box_layer.init();
	default_2d_shader.init("vertex_2d_texture.glsl", "fragment_2d_texture.glsl");
	default_rect_shader.init("vertex_rect.glsl", "fragment_rect.glsl");
	default_effect_shader.init("vertex_effect.glsl", "fragment_effect.glsl");
	shadow_shader.init("vertex_shadow.glsl", "fragment_shadow.glsl");
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
	update_shader_lights();
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
	update_shader_lights();
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
	for (int i = 0, max = MAX_LIGHT_SOURCES; i < max; i++) {
		if (i < lights.size()) {
			for (int i2 = 0, max2 = linked_shaders.size(); i2 < max2; i2++) {
				linked_shaders[i2]->use();
				linked_shaders[i2]->set_vec3("light[0].position", lights[i]->position, i);
				linked_shaders[i2]->set_vec3("light[0].ambient", lights[i]->ambient, i);
				linked_shaders[i2]->set_vec3("light[0].diffuse", lights[i]->diffuse, i);
				linked_shaders[i2]->set_vec3("light[0].specular", lights[i]->specular, i);
				linked_shaders[i2]->set_float("light[0].constant", lights[i]->constant, i);
				linked_shaders[i2]->set_float("light[0].linear", lights[i]->linear, i);
				linked_shaders[i2]->set_float("light[0].quadratic", lights[i]->quadratic, i);
				linked_shaders[i2]->set_bool("light[0].enabled", lights[i]->enabled, i);
			}
			if (lights[i]->enabled) {
				shadow_total += lights[i]->position;
				shadow_factor++;
			}
		}
		else {
			for (int i2 = 0, max2 = linked_shaders.size(); i2 < max2; i2++) {
				linked_shaders[i2]->use();
				linked_shaders[i2]->set_bool("lights[0].enabled", false, i);
			}
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
	glm::mat4 camera_matrix = glm::perspective(glm::radians(camera.fov), (float)WINDOW_W_FACTOR, 0.1f, 100.0f) * camera.get_view();
	default_rect_shader.use();
	default_rect_shader.set_mat4("camera_matrix", camera_matrix);
	default_effect_shader.use();
	default_effect_shader.set_mat4("camera_matrix", camera_matrix);
	for (int i = 0, max = linked_shaders.size(); i < max; i++) {
		linked_shaders[i]->use();
		linked_shaders[i]->set_vec3("view_pos", camera.pos);
		linked_shaders[i]->set_mat4("camera_matrix", camera_matrix);
	}
}

void RenderManager::update_shader_shadows() {
	glm::mat4 shadow_matrix = (shadow_map.perspective * shadow_map.lookat);
	shadow_shader.use();
	shadow_shader.set_mat4("camera_matrix", shadow_matrix);
	for (int i = 0, max = linked_shaders.size(); i < max; i++) {
		linked_shaders[i]->use();
		linked_shaders[i]->set_mat4("shadow_matrix", shadow_matrix);
	}
}

void RenderManager::refresh_sdl_renderer() {
	SDL_RenderClear(sdl_renderer);
	SDL_DestroyRenderer(sdl_renderer);
	sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED);
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
	default_2d_shader.destroy();
	default_rect_shader.destroy();
	shadow_shader.destroy();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(sdl_renderer);
	SDL_GL_DeleteContext(sdl_context);
	if (instance != nullptr) {
		delete instance;
	}
}