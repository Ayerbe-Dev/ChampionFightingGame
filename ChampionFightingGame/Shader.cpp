#include "Shader.h"

Shader::Shader(string vertex_dir, string fragment_dir) {
	char info_log[512];
	int success;

	string input;
	string source;

	ifstream shader_file;

	shader_file.open("resource/shaders/" + vertex_dir);
	if (shader_file.fail()) {
		cout << "Could not open Vertex Core Shader File!" << endl;
		return;
	}

	while (getline(shader_file, input)) {
		source += input + "\n";
	}

	shader_file.close();

	u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = source.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, info_log);
		cout << "Could not compile Vertex Core!" << info_log << endl;
	}

	input = "";
	source = "";

	shader_file.open("resource/shaders/" + fragment_dir);
	if (shader_file.fail()) {
		cout << "Could not open Fragment Core Shader File!" << endl;
		return;
	}

	while (getline(shader_file, input)) {
		source += input + "\n";
	}

	shader_file.close();

	u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = source.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, info_log);
		cout << "Could not compile Fragment Core!" << info_log << endl;
		return;
	}

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetShaderiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, info_log);
		cout << "Could not link Program!" << info_log << endl;
	}

	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::use() {
	glUseProgram(program);
}

void Shader::set_bool(const string& name, bool value) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void Shader::set_int(const string& name, int value) const {
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::set_float(const string& name, float value) const {
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::set_vec2(const string& name, const vec2& value) const {
	glUniform2fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::set_vec2(const string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(program, name.c_str()), x, y);
}

void Shader::set_vec3(const string& name, const vec3& value) const {
	glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void Shader::set_vec3(const string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(program, name.c_str()), x, y, z);
}

void Shader::set_vec4(const string& name, const vec4& value) const {
	glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}
void Shader::set_vec4(const string& name, float x, float y, float z, float w) {
	glUniform4f(glGetUniformLocation(program, name.c_str()), x, y, z, w);
}

void Shader::set_mat2(const string& name, const mat2& mat) const {
	glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat3(const string& name, const mat3& mat) const {
	glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::set_mat4(const string& name, const mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}