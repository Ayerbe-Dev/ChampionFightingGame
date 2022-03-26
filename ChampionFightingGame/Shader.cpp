#include "Shader.h"
#include <string>
#include <fstream>


Shader::Shader() {
}

Shader::Shader(std::string vertex_dir, std::string fragment_dir) {
	init(vertex_dir, fragment_dir);
}

Shader::~Shader() {
	glDeleteProgram(program);
}


void Shader::init(std::string vertex_dir, std::string fragment_dir) {
	char info_log[512];
	int success;

	std::string input;
	std::string source;

	std::ifstream shader_file;

	shader_file.open("resource/shaders/" + vertex_dir);
	if (shader_file.fail()) {
		std::cout << "Could not open Vertex Core Shader File!" << "\n";
		return;
	}

	std::map<std::string, int> constant_map;

	while (getline(shader_file, input)) {
		source += input + "\n";
	}

	shader_file.close();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = source.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, info_log);
		std::cout << "Could not compile Vertex Core!" << info_log << "\n";
	}

	input = "";
	source = "";

	shader_file.open("resource/shaders/" + fragment_dir);
	if (shader_file.fail()) {
		std::cout << "Could not open Fragment Core Shader File!" << "\n";
		return;
	}

	while (getline(shader_file, input)) {
		source += input + "\n";
	}

	shader_file.close();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = source.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, info_log);
		std::cout << "Could not compile Fragment Core!" << info_log << "\n";
		return;
	}

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetShaderiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, info_log);
		std::cout << "Could not link Program!" << info_log << "\n";
	}

	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() {
	glUseProgram(program);
}

void Shader::set_bool(const std::string& name, bool value, int index) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()) + index, (int)value);
}

void Shader::set_int(const std::string& name, int value, int index) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()) + index, value);
}

void Shader::set_float(const std::string& name, float value, int index) const {
	glUniform1f(glGetUniformLocation(program, name.c_str()) + index, value);
}

void Shader::set_vec2(const std::string& name, const glm::vec2& value, int index) const {
	glUniform2fv(glGetUniformLocation(program, name.c_str()) + index, 1, &value[0]);
}

void Shader::set_vec2(const std::string& name, float x, float y, int index) const {
	glUniform2f(glGetUniformLocation(program, name.c_str()) + index, x, y);
}

void Shader::set_vec3(const std::string& name, const glm::vec3& value, int index) const {
	glUniform3fv(glGetUniformLocation(program, name.c_str()) + index, 1, &value[0]);
}

void Shader::set_vec3(const std::string& name, float x, float y, float z, int index) const {
	glUniform3f(glGetUniformLocation(program, name.c_str()) + index, x, y, z);
}

void Shader::set_vec4(const std::string& name, const glm::vec4& value, int index) const {
	glUniform4fv(glGetUniformLocation(program, name.c_str()) + index, 1, &value[0]);
}

void Shader::set_vec4(const std::string& name, float x, float y, float z, float w, int index) const {
	glUniform4f(glGetUniformLocation(program, name.c_str()) + index, x, y, z, w);
}

void Shader::set_mat2(const std::string& name, const glm::mat2& mat, int index) const {
	glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()) + index, 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat3(const std::string& name, const glm::mat3& mat, int index) const {
	glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()) + index, 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat4(const std::string& name, const glm::mat4& mat, int index) const {
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()) + index, 1, GL_FALSE, &mat[0][0]);
}