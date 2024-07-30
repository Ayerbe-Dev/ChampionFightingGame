#include "Shader.h"
#include <string>
#include <fstream>
#include "ShaderManager.h"

const int SHADER_STATE_UNIFORM_BLOCK = 1;
const int SHADER_STATE_STRUCT = 2;

Shader::Shader() {
	loaded = false;
	vertex = "";
	fragment = "";
	geometry = "";
	features = 0;
	active_uniform_location = 0;
}

Shader::Shader(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir, unsigned int features) {
	loaded = false;
	active_uniform_location = 0;
	init(vertex_dir, fragment_dir, geometry_dir, features);
}

Shader::Shader(Shader& other) {
	this->features = other.features;
	this->vertex = other.vertex;
	this->fragment = other.fragment;
	this->geometry = other.geometry;
	this->id = other.id;
	this->loaded = true;
	other.loaded = false;
	this->active_uniform_location = other.active_uniform_location;
}

Shader::Shader(const Shader& other) {
	this->features = other.features;
	this->vertex = other.vertex;
	this->fragment = other.fragment;
	this->geometry = other.geometry;
	this->id = other.id;
	this->loaded = true;
	this->active_uniform_location = other.active_uniform_location;
}

Shader& Shader::operator=(Shader& other) {
	if (this == &other) {
		return *this;
	}

	this->features = other.features;
	this->vertex = other.vertex;
	this->fragment = other.fragment;
	this->geometry = other.geometry;
	this->id = other.id;
	other.loaded = false;
	this->loaded = true;
	this->active_uniform_location = other.active_uniform_location;

	return *this;
}

Shader& Shader::operator=(const Shader& other) {
	if (this == &other) {
		return *this;
	}

	this->features = other.features;
	this->vertex = other.vertex;
	this->fragment = other.fragment;
	this->geometry = other.geometry;
	this->id = other.id;
	this->loaded = true;
	this->active_uniform_location = other.active_uniform_location;

	return *this;
}

Shader::~Shader() {
	if (loaded) {
		destroy();
	}
}


void Shader::init(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir, unsigned int features) {
	ShaderManager* shader_manager = ShaderManager::get_instance();
	id = glCreateProgram();
	this->vertex = vertex_dir;
	this->fragment = fragment_dir;
	this->geometry = geometry_dir;
	this->features = features;

	std::string includes = get_includes(features);

	int shader_state = 0;
	std::string type_name = "";
	std::vector<std::pair<std::string, unsigned int>> uniform_blocks;

	int success;
	char info_log[512];
	std::ifstream shader_file;

	std::string input = "";
	std::string source = "";

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);

	shader_file.open("resource/shaders/vertex_" + vertex_dir + ".glsl");
	if (shader_file.fail()) {
		std::cout << "Could not open Vertex Core Shader File: " << vertex_dir << "\n";
		shader_file.close();
	}
	else {
		getline(shader_file, input);
		source += input;
		source += includes;
		while (getline(shader_file, input)) {
			if (shader_state & SHADER_STATE_UNIFORM_BLOCK) {
				if (input.starts_with("}")) {
					shader_state &= ~SHADER_STATE_UNIFORM_BLOCK;
				}
				else {
					unsigned int type_size = shader_manager->get_type_size(
						input.substr(input.find_first_not_of(" "), input.find_last_of(" ") - input.find_first_not_of(" "))
					);
					if (input.ends_with("];")) {
						unsigned int num_elements = 0;
						for (int i = 0, pos = input.size() - 2; input[--pos] != '['; i++) {
							num_elements += input[pos] * pow(10, i);
						}
						type_size *= num_elements;
					}
					uniform_blocks.back().second += type_size;
				}
			}
			else {
				if (input.starts_with("layout(std140) uniform ")) {
					uniform_blocks.push_back(std::pair(input.substr(23, input.find(" {") - 23), 0));
					shader_state |= SHADER_STATE_UNIFORM_BLOCK;
				}
			}

			if (shader_state & SHADER_STATE_STRUCT) {
				if (input.starts_with("}")) {
					shader_state &= ~SHADER_STATE_STRUCT;
				}
				else {
					unsigned int type_size = shader_manager->get_type_size(
						input.substr(input.find_first_not_of(" "), input.find_last_of(" ") - input.find_first_not_of(" "))
					);
					if (input.ends_with("];")) {
						unsigned int num_elements = 0;
						for (int i = 0, pos = input.size() - 2; input[--pos] != '['; i++) {
							num_elements += input[pos] * pow(10, i);
						}
						type_size *= num_elements;
					}
					shader_manager->add_type_size(type_name, type_size);
				}
			}
			else {
				if (input.starts_with("struct ")) {
					type_name = input.substr(7, input.find(" {") - 7);
					if (shader_manager->get_type_size(type_name) == 0) {
						shader_manager->add_type_size(type_name);
						shader_state |= SHADER_STATE_STRUCT;
					}
				}
			}

			source += input + "\n";
		}
		shader_state = 0;

		shader_file.close();

		const GLchar* vert_src = source.c_str();
		glShaderSource(vertex, 1, &vert_src, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, info_log);
			std::cout << info_log << ", Shader File: " << vertex_dir << "\n";
		}
		else {
			glAttachShader(id, vertex);
		}
	}

	if (geometry_dir != "") {
		input = "";
		source = "";

		shader_file.open("resource/shaders/geometry_" + geometry_dir + ".glsl");
		if (shader_file.fail()) {
			std::cout << "Could not open Geometry Core Shader File: " << geometry_dir << "\n";
			shader_file.close();
		}
		else {
			getline(shader_file, input);
			source += input;
			source += includes;
			while (getline(shader_file, input)) {
				if (shader_state & SHADER_STATE_UNIFORM_BLOCK) {
					if (input.starts_with("}")) {
						shader_state &= ~SHADER_STATE_UNIFORM_BLOCK;
					}
					else {
						unsigned int type_size = shader_manager->get_type_size(
							input.substr(input.find_first_not_of(" "), input.find_last_of(" ") - input.find_first_not_of(" "))
						);
						if (input.ends_with("];")) {
							unsigned int num_elements = 0;
							for (int i = 0, pos = input.size() - 2; input[--pos] != '['; i++) {
								num_elements += input[pos] * pow(10, i);
							}
							type_size *= num_elements;
						}
						uniform_blocks.back().second += type_size;
					}
				}
				else {
					if (input.starts_with("layout(std140) uniform ")) {
						uniform_blocks.push_back(std::pair(input.substr(23, input.find(" {") - 23), 0));
						shader_state |= SHADER_STATE_UNIFORM_BLOCK;
					}
				}

				if (shader_state & SHADER_STATE_STRUCT) {
					if (input.starts_with("}")) {
						shader_state &= ~SHADER_STATE_STRUCT;
					}
					else {
						unsigned int type_size = shader_manager->get_type_size(
							input.substr(input.find_first_not_of(" "), input.find_last_of(" ") - input.find_first_not_of(" "))
						);
						if (input.ends_with("];")) {
							unsigned int num_elements = 0;
							for (int i = 0, pos = input.size() - 2; input[--pos] != '['; i++) {
								num_elements += input[pos] * pow(10, i);
							}
							type_size *= num_elements;
						}
						shader_manager->add_type_size(type_name, type_size);
					}
				}
				else {
					if (input.starts_with("struct ")) {
						type_name = input.substr(7, input.find(" {") - 7);
						if (shader_manager->get_type_size(type_name) == 0) {
							shader_manager->add_type_size(type_name);
							shader_state |= SHADER_STATE_STRUCT;
						}
					}
				}

				source += input + "\n";
			}
			shader_state = 0;

			shader_file.close();

			const GLchar* geom_src = source.c_str();
			glShaderSource(geometry, 1, &geom_src, NULL);
			glCompileShader(geometry);

			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(geometry, 512, NULL, info_log);
				std::cout << info_log << ", Shader File: " << geometry_dir << "\n";
			}
			else {
				glAttachShader(id, geometry);
			}
		}
	}

	input = "";
	source = "";

	shader_file.open("resource/shaders/fragment_" + fragment_dir + ".glsl");
	if (shader_file.fail()) {
		std::cout << "Could not open Fragment Core Shader File: " << fragment_dir << "\n";
		shader_file.close();
	}
	else {
		getline(shader_file, input);
		source += input;
		source += includes;
		while (getline(shader_file, input)) {
			if (geometry_dir == "" && input == "in GS_OUT {") {
				/*
					This block makes it so that if we're using a frag shader designed to work with
					a geometry shader and there is no geometry shader, we don't have to make another
					copy of the frag shader, we can just tell it to read directly from the vertex
					shader instead.
					
					That being said, because whether or not a projection matrix is used will usually 
					depend on the presence of a geometry shader, this probably won't matter in the
					long run
				*/
				source += "in VS_OUT {\n";
			}
			else {
				if (shader_state & SHADER_STATE_UNIFORM_BLOCK) {
					if (input.starts_with("}")) {
						shader_state &= ~SHADER_STATE_UNIFORM_BLOCK;
					}
					else {
						unsigned int type_size = shader_manager->get_type_size(
							input.substr(input.find_first_not_of(" "), input.find_last_of(" ") - input.find_first_not_of(" "))
						);
						if (input.ends_with("];")) {
							unsigned int num_elements = 0;
							for (int i = 0, pos = input.size() - 2; input[--pos] != '['; i++) {
								num_elements += input[pos] * pow(10, i);
							}
							type_size *= num_elements;
						}
						uniform_blocks.back().second += type_size;
					}
				}
				else {
					if (input.starts_with("layout(std140) uniform ")) {
						uniform_blocks.push_back(std::pair(input.substr(23, input.find(" {") - 23), 0));
						shader_state |= SHADER_STATE_UNIFORM_BLOCK;
					}
				}

				if (shader_state & SHADER_STATE_STRUCT) {
					if (input.starts_with("}")) {
						shader_state &= ~SHADER_STATE_STRUCT;
					}
					else {
						unsigned int type_size = shader_manager->get_type_size(
							input.substr(input.find_first_not_of(" "), input.find_last_of(" ") - input.find_first_not_of(" "))
						);
						if (input.ends_with("];")) {
							unsigned int num_elements = 0;
							for (int i = 0, pos = input.size() - 2; input[--pos] != '['; i++) {
								num_elements += input[pos] * pow(10, i);
							}
							type_size *= num_elements;
						}
						shader_manager->add_type_size(type_name, type_size);
					}
				}
				else {
					if (input.starts_with("struct ")) {
						type_name = input.substr(7, input.find(" {") - 7);
						if (shader_manager->get_type_size(type_name) == 0) {
							shader_manager->add_type_size(type_name);
							shader_state |= SHADER_STATE_STRUCT;
						}
					}
				}

				source += input + "\n";
			}
		}
		shader_state = 0;

		shader_file.close();

		const GLchar* frag_src = source.c_str();
		glShaderSource(fragment, 1, &frag_src, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, info_log);
			std::cout << info_log << ", Shader File: " << fragment_dir << "\n";
		}
		else {
			glAttachShader(id, fragment);
		}
	}

	glLinkProgram(id);

	glGetShaderiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, info_log);
		std::cout << info_log << "\n";
	}

	for (int i = 0, max = uniform_blocks.size(); i < max; i++) {
		shader_manager->add_ubo(uniform_blocks[i].first, id, uniform_blocks[i].second);
	}

	glUseProgram(0);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
	loaded = true;
}

void Shader::destroy() {
	if (loaded) {
		glDeleteProgram(id);
		loaded = false;
	}
}

void Shader::use() {
	glUseProgram(id);
}

void Shader::set_active_uniform_location(const std::string& name) {
	this->active_uniform_location = glGetUniformLocation(id, name.c_str());
}

void Shader::set_bool(const std::string& name, bool value, int index) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()) + index, value);
}

void Shader::set_active_bool(bool value, int index) const {
	glUniform1i(active_uniform_location + index, (int)value);
}

void Shader::set_int(const std::string& name, int value, int index) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()) + index, value);
}

void Shader::set_active_int(int value, int index) const {
	glUniform1i(active_uniform_location + index, value);
}

void Shader::set_float(const std::string& name, float value, int index) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()) + index, value);
}

void Shader::set_active_float(float value, int index) const {
	glUniform1f(active_uniform_location + index, value);
}

void Shader::set_vec2(const std::string& name, const glm::vec2& value, int index) const {
	glUniform2fv(glGetUniformLocation(id, name.c_str()) + index, 1, &value[0]);
}

void Shader::set_active_vec2(const glm::vec2& value, int index) const {
	glUniform2fv(active_uniform_location + index, 1, &value[0]);
}

void Shader::set_vec2(const std::string& name, float x, float y, int index) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()) + index, x, y);
}

void Shader::set_active_vec2(float x, float y, int index) const {
	glUniform2f(active_uniform_location + index, x, y);
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value, int index) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()) + index, 1, &value[0]);
}

void Shader::set_active_vec3(const glm::vec3& value, int index) const {
	glUniform3fv(active_uniform_location + index, 1, &value[0]);
}

void Shader::set_vec3(const std::string& name, float x, float y, float z, int index) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()) + index, x, y, z);
}

void Shader::set_active_vec3(float x, float y, float z, int index) const {
	glUniform3f(active_uniform_location + index, x, y, z);
}

void Shader::set_vec4(const std::string& name, const glm::vec4& value, int index) const {
	glUniform4fv(glGetUniformLocation(id, name.c_str()) + index, 1, &value[0]);
}

void Shader::set_active_vec4(const glm::vec4& value, int index) const {
	glUniform4fv(active_uniform_location + index, 1, &value[0]);
}

void Shader::set_vec4(const std::string& name, float x, float y, float z, float w, int index) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()) + index, x, y, z, w);
}

void Shader::set_active_vec4(float x, float y, float z, float w, int index) const {
	glUniform4f(active_uniform_location + index, x, y, z, w);
}

void Shader::set_mat2(const std::string& name, const glm::mat2& mat, int index) const {
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()) + index, 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_active_mat2(const glm::mat2& mat, int index) const {
	glUniformMatrix2fv(active_uniform_location + index, 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat3(const std::string& name, const glm::mat3& mat, int index) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()) + index, 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_active_mat3(const glm::mat3& mat, int index) const {
	glUniformMatrix3fv(active_uniform_location + index, 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat4(const std::string& name, const glm::mat4& mat, int index) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()) + index, 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_active_mat4(const glm::mat4& mat, int index) const {
	glUniformMatrix4fv(active_uniform_location + index, 1, GL_FALSE, &mat[0][0]);
}

std::string get_includes(unsigned int features) {
	std::string ret = "\n";

	if (features & SHADER_FEAT_DIM_MUL) {
		ret += "#define SHADER_FEAT_DIM_MUL\n";
	}
	if (features & SHADER_FEAT_BONES) {
		ret += "#define SHADER_FEAT_BONES\n";
	}
	if (features & SHADER_FEAT_COLORMOD) {
		ret += "#define SHADER_FEAT_COLORMOD\n";
	}
	if (features & SHADER_FEAT_DIFFUSE) {
		ret += "#define SHADER_FEAT_DIFFUSE\n";
	}
	if (features & SHADER_FEAT_SPECULAR) {
		ret += "#define SHADER_FEAT_SPECULAR\n";
	}
	if (features & SHADER_FEAT_POSITION) {
		ret += "#define SHADER_FEAT_POSITION\n";
	}
	if (features & SHADER_FEAT_NORMAL) {
		ret += "#define SHADER_FEAT_NORMAL\n";
	}
	if (features & SHADER_FEAT_SSAO) {
		ret += "#define SHADER_FEAT_SSAO\n";
	}

	return ret;
}