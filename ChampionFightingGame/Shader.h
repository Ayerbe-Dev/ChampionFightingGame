#pragma once
#include <iostream>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

const int SHADER_FEAT_DIM_MUL = 1;
const int SHADER_FEAT_HAS_BONES = 2;
const int SHADER_FEAT_COLORMOD = 4;

class Shader {
public:
	Shader();
	Shader(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir, unsigned int features);
	Shader(Shader& other);
	Shader(const Shader& other);
	Shader& operator=(Shader& other);
	Shader& operator=(const Shader& other);
	~Shader();
	unsigned int id = 0;
	std::string vertex;
	std::string fragment;
	std::string geometry;
	unsigned int features;

	void init(std::string vertex_dir, std::string fragment_dir, std::string geometry_dir, unsigned int features);
	void destroy();

	void use();
	void set_active_uniform_location(const std::string& name);

	void set_bool(const std::string& name, bool value, int index = 0) const;
	void set_active_bool(bool value, int index = 0) const;
	void set_int(const std::string& name, int value, int index = 0) const;
	void set_active_int(int value, int index = 0) const;
	void set_float(const std::string& name, float value, int index = 0) const;
	void set_active_float(float value, int index = 0) const;
	void set_vec2(const std::string& name, const glm::vec2& value, int index = 0) const;
	void set_active_vec2(const glm::vec2& value, int index = 0) const;
	void set_vec2(const std::string& name, float x, float y, int index = 0) const;
	void set_active_vec2(float x, float y, int index = 0) const;
	void set_vec3(const std::string& name, const glm::vec3& value, int index = 0) const;
	void set_active_vec3(const glm::vec3& value, int index = 0) const;
	void set_vec3(const std::string& name, float x, float y, float z, int index = 0) const;
	void set_active_vec3(float x, float y, float z, int index = 0) const;
	void set_vec4(const std::string& name, const glm::vec4& value, int index = 0) const;
	void set_active_vec4(const glm::vec4& value, int index = 0) const;
	void set_vec4(const std::string& name, float x, float y, float z, float w, int index = 0) const;
	void set_active_vec4(float x, float y, float z, float w, int index = 0) const;
	void set_mat2(const std::string& name, const glm::mat2& mat, int index = 0) const;
	void set_active_mat2(const glm::mat2& mat, int index = 0) const;
	void set_mat3(const std::string& name, const glm::mat3& mat, int index = 0) const;
	void set_active_mat3(const glm::mat3& mat, int index = 0) const;
	void set_mat4(const std::string& name, const glm::mat4& mat, int index = 0) const;
	void set_active_mat4(const glm::mat4& mat, int index = 0) const;
private:
	unsigned int active_uniform_location;

	bool loaded;
};

std::string get_includes(unsigned int features);