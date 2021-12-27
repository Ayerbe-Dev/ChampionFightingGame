#include "RenderManager.h"

bool load_shaders(u32 *program) {
	char info_log[512];
	int success;

	string input;
	string source;

	ifstream shader_file;

	shader_file.open("resource/shaders/vertex_core.glsl");
	if (shader_file.fail()) {
		cout << "Could not open Vertex Core Shader File!" << endl;
		return false;
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
		return false;
	}

	input = "";
	source = "";

	shader_file.open("resource/shaders/fragment_core.glsl");
	if (shader_file.fail()) {
		cout << "Could not open Fragment Core Shader File!" << endl;
		return false;
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
		return false;
	}

	*program = glCreateProgram();
	glAttachShader(*program, vertexShader);
	glAttachShader(*program, fragmentShader);
	glLinkProgram(*program);

	glGetShaderiv(*program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(*program, 512, NULL, info_log);
		cout << "Could not link Program!" << info_log << endl;
		return false;
	}

	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}