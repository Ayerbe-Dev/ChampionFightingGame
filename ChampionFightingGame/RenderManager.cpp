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
	bg_shader = Shader("vertex_sdl_overlay.glsl", "fragment_sdl_overlay.glsl");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(screen_coords), screen_coords, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
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

void RenderManager::render_sdl_as_gl(SDL_Texture* background) {
	bg_shader.use();
	bg_shader.set_int("texture_diffuse1", 0);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	SDL_GL_BindTexture(background, nullptr, nullptr);
	auto test = SDL_GetError();
	cout << test << endl;
	glDrawArrays(GL_TRIANGLES, 0, 1);
}

void RenderManager::render(Model *model, Shader *shader, vec3 *model_pos, vec3 *model_rot, vec3 *model_scale) {
	shader->use(); //Because each shader is model specific, we need to set this no matter what
	update_shader_cam(shader);
	mat4 model_mat = mat4(1.0); //MAYBE we can make this a member of Model and just reset it every frame so there's no new mat4 being generated
	model_mat = scale(model_mat, *model_scale);	//Derefing takes less time than putting an entire vec3 on the stack
	model_mat = translate(model_mat, *model_pos); //Ditto
	model_mat *= orientate4(*model_rot); //Ditto and also using orientate4 here means it's only derefed once instead of 3 times as opposed to 3 calls
	//to rotate() which all require the rotation to be derefed again
	shader->set_mat4("model", model_mat); //Can't be avoided, maybe set_mat4 can be overloaded to take a pointer since a mat4 is bigger than a &mat4
	model->render(shader); //2-3 ms to execute, definitely the biggest part of this
}