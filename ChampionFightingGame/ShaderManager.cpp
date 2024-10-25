#include "ShaderManager.h"
#include <functional>

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

	for (unsigned int feature : get_feature_permutations(
		{ SHADER_FEAT_DIM_MUL, SHADER_FEAT_BONES, SHADER_FEAT_NORMAL, SHADER_FEAT_ALPHA_PASS }
	)) {
		Shader* shader = get_shader("model", "model", "model", feature);
		shader->use();
		shader->set_int("shadow_map", 0);
		shader->set_int("material.diffuse", 1);
		shader->set_int("material.specular", 2);
		shader->set_int("material.normal", 3);
		shader->set_int("material.emission", 4);
	}
}

Shader* ShaderManager::get_shader(std::string vertex, std::string fragment, std::string geometry, unsigned int features) {
	if (!shader_map.contains(vertex)
	|| !shader_map[vertex].contains(fragment)
	|| !shader_map[vertex][fragment].contains(geometry)
	|| !shader_map[vertex][fragment][geometry].contains(features)) {
		shader_map[vertex][fragment][geometry][features].init(this, vertex, fragment, geometry, features);
	}
	return &shader_map[vertex][fragment][geometry][features];
}

Shader* ShaderManager::get_shader_switch_features(Shader* base, unsigned int remove_features, unsigned int add_features) {
	unsigned int feats = (base->features & ~remove_features) | add_features;
	return get_shader(base->vertex, base->fragment, base->geometry, feats);
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

std::vector<unsigned int> ShaderManager::get_feature_permutations(std::vector<unsigned int> feat_vec) {
	std::vector<unsigned int> ret;
	std::function<void(std::vector<unsigned int>, unsigned int)> combo_rec = 
	[&ret, &combo_rec](std::vector<unsigned int> feats, unsigned int combo) {
		if (feats.empty()) {
			ret.push_back(combo);
			return;
		}
		unsigned int curr_feat = feats.back();
		feats.pop_back();
		combo_rec(feats, combo);
		combo |= curr_feat;
		combo_rec(feats, combo);
	};
	combo_rec(feat_vec, 0);
	return ret;
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
	if (features & SHADER_FEAT_NORMAL) {
		ret += "#define SHADER_FEAT_NORMAL\n";
	}
	if (features & SHADER_FEAT_SSAO) {
		ret += "#define SHADER_FEAT_SSAO\n";
	}
	if (features & SHADER_FEAT_ALPHA_PASS) {
		ret += "#define SHADER_FEAT_ALPHA_PASS\n";
	}
	if (features & SHADER_FEAT_OUTLINE) {
		ret += "#define SHADER_FEAT_OUTLINE\n";
	}
	if (features & SHADER_FEAT_BLOOM) {
		ret += "#define SHADER_FEAT_BLOOM\n";
	}
	if (features & SHADER_FEAT_BILLBOARD_ON) {
		ret += "#define SHADER_FEAT_BILLBOARD_ON\n";
	}
	if (features & SHADER_FEAT_BILLBOARD_ON_FIX_SIZE) {
		ret += "#define SHADER_FEAT_BILLBOARD_ON_FIX_SIZE\n";
	}

	return ret;
}