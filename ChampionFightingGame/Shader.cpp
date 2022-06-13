#include "Shader.h"
#include <string>
#include <fstream>


Shader::Shader() {
	loaded = false;
}

Shader::Shader(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir) {
	loaded = false;
	init(vertex_dir, fragment_dir, geometry_dir);
}

Shader::~Shader() {
	if (loaded) {
		destroy();
	}
}


void Shader::init(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir) {
	program = glCreateProgram();
	name = vertex_dir + ", " + fragment_dir;
	if (geometry_dir != "") {
		name += ", " + geometry_dir;
	}
	int success;
	char info_log[512];
	std::ifstream shader_file;

	std::string input = "";
	std::string source = "";

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);

	shader_file.open("resource/shaders/" + vertex_dir);
	if (shader_file.fail()) {
		std::cout << "Could not open Vertex Core Shader File: " << vertex_dir << "\n";
		shader_file.close();
	}
	else {
		while (getline(shader_file, input)) {
			source += input + "\n";
		}

		shader_file.close();

		const GLchar* vert_src = source.c_str();
		glShaderSource(vertex, 1, &vert_src, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, info_log);
			std::cout << info_log << "\n";
		}
		else {
			glAttachShader(program, vertex);
		}
	}

	if (geometry_dir != "") {
		input = "";
		source = "";

		shader_file.open("resource/shaders/" + geometry_dir);
		if (shader_file.fail()) {
			std::cout << "Could not open Geometry Core Shader File: " << geometry_dir << "\n";
			shader_file.close();
		}
		else {
			while (getline(shader_file, input)) {
				source += input + "\n";
			}

			shader_file.close();

			const GLchar* geom_src = source.c_str();
			glShaderSource(geometry, 1, &geom_src, NULL);
			glCompileShader(geometry);

			glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(geometry, 512, NULL, info_log);
				std::cout << info_log << "\n";
			}
			else {
				glAttachShader(program, geometry);
			}
		}
	}

	input = "";
	source = "";

	shader_file.open("resource/shaders/" + fragment_dir);
	if (shader_file.fail()) {
		std::cout << "Could not open Fragment Core Shader File: " << fragment_dir << "\n";
		shader_file.close();
	}
	else {
		while (getline(shader_file, input)) {
			if (geometry_dir == "" && input == "in GS_OUT {") {
				//This block makes it so that if we're using a frag shader designed to work with
				//a geometry shader and there is no geometry shader, we don't have to make another
				//copy of the frag shader, we can just tell it to read directly from the vertex
				//shader instead.

				//That being said, because whether or not a projection matrix is used will usually 
				//depend on the presence of a geometry shader, this probably won't matter in the
				//long run
				source += "in VS_OUT {\n";
			}
			else {
				source += input + "\n";
			}
		}

		shader_file.close();

		const GLchar* frag_src = source.c_str();
		glShaderSource(fragment, 1, &frag_src, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, info_log);
			std::cout << info_log << "\n";
		}
		else {
			glAttachShader(program, fragment);
		}
	}

	glLinkProgram(program);

	glGetShaderiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, info_log);
		std::cout << info_log << "\n";
	}

	glUseProgram(0);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
	loaded = true;
}

void Shader::destroy() {
	if (loaded) {
		glDeleteProgram(program);
		loaded = false;
	}
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