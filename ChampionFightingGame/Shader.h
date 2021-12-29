#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "utils.h"
#include "Camera.h"
using namespace glm;

class Shader {
public:
	Shader(string vertexPath, string fragmentPath, Camera* camera);
	~Shader();
	unsigned int program;

	vec3 point_lights[4] = { vec3(0.0, 0.0, 0.0) };
	Camera* camera;

	bool active_dir_light = false;
	int active_point_lights = 0;
	bool active_spot_light = false;

	void use();

	void use_default_dir_light(bool enable);
	void use_default_point_light(int num);
	void use_default_spot_light(bool enable);
	
	void update_active_lights();

	void set_point_lights(vec3 p1 = vec3(0.0, 0.0, 0.0), vec3 p2 = vec3(0.0, 0.0, 0.0), vec3 p3 = vec3(0.0, 0.0, 0.0), vec3 p4 = vec3(0.0, 0.0, 0.0));

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