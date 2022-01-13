#include "RenderManager.h"
using namespace glm;

Light::Light(vec3 pos) {
	position = pos;
	ambient = vec3(0.2, 0.2, 0.2);
	diffuse = vec3(0.5, 0.5, 0.5);
	specular = vec3(1.0, 1.0, 1.0);
	constant = 1.0;
	linear = 0.09;
	quadratic = 0.032;
	enabled = true;
}

RenderManager::RenderManager() {}

void RenderManager::init() {
	default_2d_shader.init("vertex_2d_texture.glsl", "fragment_2d_texture.glsl");
}

void RenderManager::add_light(Light light, int target) {
	if (target == -1) {
		if (num_lights == MAX_LIGHT_SOURCES) {
			cout << "Congrats you stupid idiot, you ran out of lights" << endl;
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
		for (int i = 0; i < num_lights; i++) {
			lights[i].enabled = false;
			num_lights--;
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

void RenderManager::update_shader_lights(Shader *shader) {
	shader->use();
	shader->set_float("material.shininess", 16.0f);

	for (int i = 0; i < MAX_LIGHT_SOURCES; i++) {
		string light = "light[" + to_string(i) + "].";
		shader->set_vec3(light + "position", lights[i].position);
		shader->set_vec3(light + "ambient", lights[i].ambient);
		shader->set_vec3(light + "diffuse", lights[i].diffuse);
		shader->set_vec3(light + "specular", lights[i].specular);
		shader->set_float(light + "constant", lights[i].constant);
		shader->set_float(light + "linear", lights[i].linear);
		shader->set_float(light + "quadratic", lights[i].quadratic);
		shader->set_bool(light + "enabled", lights[i].enabled);
	}
}

void RenderManager::update_shader_cam(Shader* shader) {
	camera.update_view();
	mat4 view = camera.get_view();
	mat4 projection = perspective(radians(camera.fov), (float)WINDOW_W_FACTOR, 0.1f, 100.0f);
	shader->set_vec3("view_pos", camera.pos);

	shader->set_mat4("projection", projection);
	shader->set_mat4("view", view);
}

void RenderManager::render_model(Model *model, Shader *shader, vec3 *model_pos, vec3 *model_rot, vec3 *model_scale) {
	shader->use(); //Because each shader is model specific, we need to set this no matter what
	update_shader_cam(shader);
	mat4 model_mat = mat4(1.0);
	model_mat = scale(model_mat, *model_scale);
	model_mat = translate(model_mat, *model_pos);
	model_mat *= orientate4(*model_rot);
	shader->set_mat4("model", model_mat);
	model->render(shader);
}