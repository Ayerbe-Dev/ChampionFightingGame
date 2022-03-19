#include "RenderManager.h"
#include <string>
#include "utils.h"

RenderManager::RenderManager() {
	window_scaler.x = WINDOW_WIDTH / 5;
	window_scaler.y = WINDOW_HEIGHT / 5;
	window_scaler.z = 1;
}

void RenderManager::init() {
	default_2d_shader.init("vertex_2d_texture.glsl", "fragment_2d_texture.glsl");
	default_rect_shader.init("vertex_rect.glsl", "fragment_rect.glsl");
}

void RenderManager::add_light(Light light, int target) {
	if (target == -1) {
		if (num_lights == MAX_LIGHT_SOURCES) {
			std::cout << "Congrats you stupid idiot, you ran out of lights" << "\n";
			return;
		}
		else {
			for (int i = 0; i < num_lights; i++) {
				if (!lights[i].enabled) {
					lights[i] = light;
					num_lights++;
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
	for (int i = 0; i < linked_shaders.size(); i++) {
		linked_shaders[i]->use();
		linked_shaders[i]->set_float("material.shininess", 4.0f);

		for (int i2 = 0; i2 < MAX_LIGHT_SOURCES; i2++) {
			std::string light = "light[" + std::to_string(i2) + "].";
			linked_shaders[i]->set_vec3(light + "position", lights[i2].position);
			linked_shaders[i]->set_vec3(light + "ambient", lights[i2].ambient);
			linked_shaders[i]->set_vec3(light + "diffuse", lights[i2].diffuse);
			linked_shaders[i]->set_vec3(light + "specular", lights[i2].specular);
			linked_shaders[i]->set_float(light + "constant", lights[i2].constant);
			linked_shaders[i]->set_float(light + "linear", lights[i2].linear);
			linked_shaders[i]->set_float(light + "quadratic", lights[i2].quadratic);
			linked_shaders[i]->set_bool(light + "enabled", lights[i2].enabled);
		}
	}
}

void RenderManager::update_shader_cam(Shader* shader) {
	camera.update_view();
	glm::mat4 view = camera.get_view();
	glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)WINDOW_W_FACTOR, 0.1f, 100.0f);
	shader->set_vec3("view_pos", camera.pos);

	shader->set_mat4("projection", projection);
	shader->set_mat4("view", view);
}

void RenderManager::render_model(Model *model, Shader *shader, glm::mat4 extra_mat, glm::vec3 *model_pos, glm::vec3 *model_rot, glm::vec3 *model_scale) {
	shader->use(); //Because each shader is model specific, we need to set this no matter what
	update_shader_cam(shader);
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, *model_pos / glm::vec3(WINDOW_WIDTH / (100 * model_scale->x), WINDOW_HEIGHT / (100 * model_scale->y), 1.0));
	model_mat *= orientate4(*model_rot);
	model_mat = scale(model_mat, *model_scale);
	model_mat *= extra_mat;
	shader->set_mat4("model", model_mat);
	model->render(shader);
}