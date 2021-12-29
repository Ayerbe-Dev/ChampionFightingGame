#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "utils.h"
using namespace glm;

class Shader {
public:
	Shader(string vertexPath, string fragmentPath);
	~Shader();
	unsigned int program;

	void use();
	void set_bool(const std::string& name, bool value) const;
	void set_int(const std::string& name, int value) const;
	void set_float(const std::string& name, float value) const;
	void set_vec2(const std::string& name, const vec2& value) const;
	void set_vec2(const std::string& name, float x, float y) const;
	void set_vec3(const std::string& name, const vec3& value) const;
	void set_vec3(const std::string& name, float x, float y, float z) const;
	void set_vec4(const std::string& name, const vec4& value) const;
	void set_vec4(const std::string& name, float x, float y, float z, float w);
	void set_mat2(const std::string& name, const mat2& mat) const;
	void set_mat3(const std::string& name, const mat3& mat) const;
	void set_mat4(const std::string& name, const mat4& mat) const;
};