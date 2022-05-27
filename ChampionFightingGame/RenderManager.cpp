#include "RenderManager.h"
#include <string>
#include "utils.h"

RenderManager::RenderManager() {
	num_lights = 0;
	s_window_width = 0;
	s_window_height = 0;
	box_FBO = 0;
}

void RenderManager::init() {
	default_2d_shader.init("vertex_2d_texture.glsl", "fragment_2d_texture.glsl");
	default_rect_shader.init("vertex_rect.glsl", "fragment_rect.glsl");
	default_effect_shader.init("vertex_effect.glsl", "fragment_effect.glsl");
	shadow_shader.init("vertex_shadow.glsl", "fragment_shadow.glsl");

	glGenFramebuffers(1, &box_FBO);
	glGenTextures(1, &box_FBO_color);
	glGenRenderbuffers(1, &box_FBO_depth);

	glBindFramebuffer(GL_FRAMEBUFFER, box_FBO);

	glBindTexture(GL_TEXTURE_2D, box_FBO_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, box_FBO_color, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, box_FBO_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, WINDOW_WIDTH, WINDOW_HEIGHT);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, box_FBO_depth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderManager::destroy() {
	unlink_all_shaders();
	default_2d_shader.destroy();
	default_rect_shader.destroy();
	shadow_shader.destroy();
}

void RenderManager::add_light(Light light, int target) {
	if (target == -1) {
		if (num_lights == MAX_LIGHT_SOURCES) {
			std::cout << "Congrats you stupid idiot, you ran out of lights" << "\n";
			return;
		}
		else {
			for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
				if (!lights[i].enabled) {
					lights[i] = light;
					num_lights++;
					break;
				}
			}
		}
	}
	else {
		lights[target] = light;
		num_lights++;
	}
}

void RenderManager::remove_light(int target) {
	if (target == -1) {
		int index = 0;
		while (num_lights != 0 && index < MAX_LIGHT_SOURCES) {
			if (lights[index].enabled) {
				lights[index].enabled = false;
				num_lights--;
			}
			index++;
		}
	}
	else {
		lights[target].enabled = false;
		num_lights--;
	}
	if (num_lights < 0) {
		num_lights = 0;
	}
}

void RenderManager::link_shader(Shader *shader) {
	linked_shaders.push_back(shader);
}

void RenderManager::unlink_all_shaders() {
	linked_shaders.clear();
}

void RenderManager::update_shader_lights() {
	for (int i = 0, max = linked_shaders.size(); i < max; i++) {
		linked_shaders[i]->use();
		linked_shaders[i]->set_float("material.shininess", 4.0f);

		for (int i2 = 0; i2 < MAX_LIGHT_SOURCES; i2++) {
			linked_shaders[i]->set_vec3("light[0].position", lights[i2].position, i2);
			linked_shaders[i]->set_vec3("light[0].ambient", lights[i2].ambient, i2);
			linked_shaders[i]->set_vec3("light[0].diffuse", lights[i2].diffuse, i2);
			linked_shaders[i]->set_vec3("light[0].specular", lights[i2].specular, i2);
			linked_shaders[i]->set_float("light[0].constant", lights[i2].constant, i2);
			linked_shaders[i]->set_float("light[0].linear", lights[i2].linear, i2);
			linked_shaders[i]->set_float("light[0].quadratic", lights[i2].quadratic, i2);
			linked_shaders[i]->set_bool("light[0].enabled", lights[i2].enabled, i2);
		}
	}
}

void RenderManager::update_shader_cam(Shader* shader) {
	camera.update_view();
	glm::mat4 view = camera.get_view();
	glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WINDOW_W_FACTOR, 0.1f, 100.0f);
	shader->set_vec3("view_pos", camera.pos);
	shader->set_mat4("camera_matrix", projection * view);
}

void RenderManager::render_model(Model *model, Shader *shader, glm::mat4 extra_mat, glm::vec3 *model_pos, glm::vec3 *model_rot, glm::vec3 *model_scale, bool flip) {
	shader->use(); 
	update_shader_cam(shader);
	shader->set_mat4("shadow_light_view", shadow_map.m_orthographic_perspective * shadow_map.m_lookat);
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, 
		*model_pos / glm::vec3(
			WINDOW_WIDTH / (100 * model_scale->x), 
			WINDOW_HEIGHT / (100 * model_scale->y), 
			WINDOW_DEPTH / (100 * model_scale->z)
		));
	model_mat *= orientate4(*model_rot);
	model_mat = scale(model_mat, *model_scale);
	model_mat *= extra_mat;
	shader->set_mat4("model_matrix", model_mat);
	model->render(shader, flip);
}

void RenderManager::render_model_shadow(Model* model, glm::mat4 extra_mat, glm::vec3* model_pos, glm::vec3* model_rot, glm::vec3* model_scale, bool flip) {
	shadow_shader.use();
	shadow_shader.set_mat4("camera_matrix", shadow_map.m_orthographic_perspective * shadow_map.m_lookat);
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, 
		*model_pos / glm::vec3(
			WINDOW_WIDTH / (100 * model_scale->x), 
			WINDOW_HEIGHT / (100 * model_scale->y), 
			WINDOW_DEPTH / (100 * model_scale->z)
		));
	model_mat *= orientate4(*model_rot);
	model_mat = scale(model_mat, *model_scale);
	model_mat *= extra_mat;
	shadow_shader.set_mat4("model_matrix", model_mat);
	model->render_shadow(&shadow_shader, flip);
}

RenderManager* RenderManager::instance = nullptr;
RenderManager* RenderManager::get_instance() {
	if (instance == nullptr) {
		instance = new RenderManager;
	}
	return instance;
}