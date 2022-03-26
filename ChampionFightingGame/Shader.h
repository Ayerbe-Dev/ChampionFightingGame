#pragma once
#include <iostream>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

struct UniformType {
	std::string name;
	int num_members;
};

struct UniformVar {
	std::string name;
	UniformType type;
};

class Shader {
public:
	Shader();
	Shader(std::string vertex_dir, std::string fragment_dir);
	~Shader();
	unsigned int program;

	void init(std::string vertex_dir, std::string fragment_dir);

	void use();

	void set_bool(const std::string& name, bool value, int index = 0) const;
	void set_int(const std::string& name, int value, int index = 0) const;
	void set_float(const std::string& name, float value, int index = 0) const;
	void set_vec2(const std::string& name, const glm::vec2& value, int index = 0) const;
	void set_vec2(const std::string& name, float x, float y, int index = 0) const;
	void set_vec3(const std::string& name, const glm::vec3& value, int index = 0) const;
	void set_vec3(const std::string& name, float x, float y, float z, int index = 0) const;
	void set_vec4(const std::string& name, const glm::vec4& value, int index = 0) const;
	void set_vec4(const std::string& name, float x, float y, float z, float w, int index = 0) const;
	void set_mat2(const std::string& name, const glm::mat2& mat, int index = 0) const;
	void set_mat3(const std::string& name, const glm::mat3& mat, int index = 0) const;
	void set_mat4(const std::string& name, const glm::mat4& mat, int index = 0) const;
};