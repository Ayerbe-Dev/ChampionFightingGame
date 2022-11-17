#pragma once
#include <iostream>
#include <map>
#include "Shader.h"

class ShaderManager {
public:
	ShaderManager(ShaderManager& other) = delete;
	void operator=(const ShaderManager& other) = delete;

	Shader* get_shader(std::string vertex, std::string fragment, std::string geometry, unsigned int features);
	Shader* get_shader_switch_features(Shader* base, unsigned int remove_features, unsigned int add_features);

	void set_global_bool(const std::string& name, bool value, int index = -1) const;
	void set_global_int(const std::string& name, int value, int index = -1) const;
	void set_global_float(const std::string& name, float value, int index = -1) const;
	void set_global_vec2(const std::string& name, const glm::vec2& value, int index = -1) const;
	void set_global_vec2(const std::string& name, float x, float y, int index = -1) const;
	void set_global_vec3(const std::string& name, const glm::vec3& value, int index = -1) const;
	void set_global_vec3(const std::string& name, float x, float y, float z, int index = -1) const;
	void set_global_vec4(const std::string& name, const glm::vec4& value, int index = -1) const;
	void set_global_vec4(const std::string& name, float x, float y, float z, float w, int index = -1) const;
	void set_global_mat2(const std::string& name, const glm::mat2& mat, int index = -1) const;
	void set_global_mat3(const std::string& name, const glm::mat3& mat, int index = -1) const;
	void set_global_mat4(const std::string& name, const glm::mat4& mat, int index = -1) const;

	void add_ubo(std::string name, int shader_id, unsigned int size);
	void add_type_size(std::string name, unsigned int size = 0);
	unsigned int get_type_size(std::string name);

	static ShaderManager* get_instance();
	void destroy_instance();
private:
	ShaderManager();

	std::map<std::string, std::pair<unsigned int, unsigned int>> UBO;
	std::map<std::string, unsigned int> types;
	std::map<std::string, std::map<std::string, std::map<std::string, std::map<unsigned int, Shader>>>> shader_map;

	static ShaderManager* instance;
};