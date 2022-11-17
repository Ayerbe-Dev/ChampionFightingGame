#include "ShaderManager.h"

ShaderManager::ShaderManager() {
	add_type_size("bool", 4);
	add_type_size("int", 4);
	add_type_size("unsigned int", 4);
	add_type_size("float", 4);
	add_type_size("sampler2D", 4);
	add_type_size("vec2", 8);
	add_type_size("vec3", 12);
	add_type_size("vec4", 16);
	add_type_size("mat2", 32);
	add_type_size("mat3", 48);
	add_type_size("mat4", 64);
}

Shader* ShaderManager::get_shader(std::string vertex, std::string fragment, std::string geometry, unsigned int features) {
	if (!shader_map.contains(vertex)
	|| !shader_map[vertex].contains(fragment)
	|| !shader_map[vertex][fragment].contains(geometry)
	|| !shader_map[vertex][fragment][geometry].contains(features)) {
		shader_map[vertex][fragment][geometry][features].init(vertex, fragment, geometry, features);
	}
	return &shader_map[vertex][fragment][geometry][features];
}

Shader* ShaderManager::get_shader_switch_features(Shader* base, unsigned int remove_features, unsigned int add_features) {
	return get_shader(base->vertex, base->fragment, base->geometry, (base->features | add_features) & ~remove_features);
}

void ShaderManager::set_global_bool(const std::string& name, bool value, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), (int*)&value);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(int), sizeof(int), &value);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_int(const std::string& name, int value, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(int), &value);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(int), sizeof(int), &value);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_float(const std::string& name, float value, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &value);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(float), sizeof(float), &value);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_vec2(const std::string& name, const glm::vec2& value, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &value);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::vec2), sizeof(glm::vec2), &value);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_vec2(const std::string& name, float x, float y, int index) const {
	if (UBO.contains(name)) {
		glm::vec2 value(x, y);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec2), &value);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::vec2), sizeof(glm::vec2), &value);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_vec3(const std::string& name, const glm::vec3& value, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &value);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::vec3), sizeof(glm::vec3), &value);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_vec3(const std::string& name, float x, float y, float z, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		glm::vec3 value(x, y, z);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &value);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::vec3), sizeof(glm::vec3), &value);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_vec4(const std::string& name, const glm::vec4& value, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &value);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::vec4), sizeof(glm::vec4), &value);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_vec4(const std::string& name, float x, float y, float z, float w, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		glm::vec4 value(x, y, z, w);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &value);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::vec4), sizeof(glm::vec4), &value);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_mat2(const std::string& name, const glm::mat2& mat, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat2), &mat);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::mat2), sizeof(glm::mat2), &mat);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_mat3(const std::string& name, const glm::mat3& mat, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat3), &mat);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::mat3), sizeof(glm::mat3), &mat);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::set_global_mat4(const std::string& name, const glm::mat4& mat, int index) const {
	if (UBO.contains(name)) {
		glBindBuffer(GL_UNIFORM_BUFFER, UBO.at(name).first);
		if (index == -1) {
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &mat);
		}
		else {
			glBufferSubData(GL_UNIFORM_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &mat);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

void ShaderManager::add_ubo(std::string name, int shader_id, unsigned int size) {
	unsigned int buffer;
	unsigned int binding_point;
	if (UBO.contains(name)) {
		buffer = UBO[name].first;
		binding_point = UBO[name].second;
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	}
	else {
		binding_point = UBO.size();
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		UBO[name].first = buffer;
		UBO[name].second = binding_point;
	}
	unsigned int block_index = glGetUniformBlockIndex(shader_id, name.c_str());
	glUniformBlockBinding(shader_id, block_index, binding_point);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

void ShaderManager::add_type_size(std::string name, unsigned int size) {
	if (!types.contains(name)) {
		types[name] = size;
	}
	else {
		types[name] += size;
	}
}

unsigned int ShaderManager::get_type_size(std::string name) {
	if (!types.contains(name)) {
		return 0;
	}
	else {
		return types[name];
	}
}

ShaderManager* ShaderManager::instance = nullptr;
ShaderManager* ShaderManager::get_instance() {
	if (instance == nullptr) {
		instance = new ShaderManager;
	}
	return instance;
}

void ShaderManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}