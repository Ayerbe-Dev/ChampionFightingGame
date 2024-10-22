#pragma once
#include <iostream>
#include <map>
#include "Shader.h"

//Features used by Model shaders

const int SHADER_FEAT_DIM_MUL = 1;
const int SHADER_FEAT_BONES = 2;
const int SHADER_FEAT_NORMAL = 4;
const int SHADER_FEAT_ALPHA_PASS = 8;

//Features used by WorldTexture shaders

const int SHADER_FEAT_BILLBOARD_ON = 1;
const int SHADER_FEAT_BILLBOARD_ON_FIX_SIZE = 2;

//Features used by the GBuffer shader

const int SHADER_FEAT_DIFFUSE = 1;
const int SHADER_FEAT_SPECULAR = 2;
const int SHADER_FEAT_SSAO = 4;
const int SHADER_FEAT_OUTLINE = 8;

//Features used by the HDR buffer shader

const int SHADER_FEAT_BLOOM = 1;

const int SHADER_FEAT_COLORMOD = 1; //Used by GameTextures, but will be removed as Screen/WorldTextures
//don't use it

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
	std::vector<unsigned int> get_feature_permutations(std::vector<unsigned int> feat_vec);

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

std::string get_includes(unsigned int features);