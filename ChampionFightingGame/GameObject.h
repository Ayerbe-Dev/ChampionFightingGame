#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "Animation.h"

class RenderManager;
class SoundManager;
class EffectManager;

class GameObject {
public:
	GameObject();
	GameObject(Model model, Shader shader);

	int id_effect = -1;
	int id_sound = -1;

	glm::vec3 pos = glm::vec3(0.0);
	glm::vec3 rot = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
	glm::mat4 extra_mat = glm::mat4(1.0);

	AnimationTable anim_table;
	Animation* anim_kind;
	Animation* prev_anim_kind;
	glm::vec3 prev_anim_offset;
	float frame;
	float rate;
	float prev_anim_rate;
	float prev_anim_frame;
	bool is_anim_end = false;

	Shader shader;
	Shader shadow_shader;
	Shader outline_shader;
	Model model;

	RenderManager* render_manager;
	SoundManager* sound_manager;
	EffectManager* effect_manager;

	//Render Funcs

	void render(bool flip = false);
	void render_shadow(bool flip = false);
	void render_outline(bool flip = false);

	//Animation Funcs

	void set_rate(float rate);
	void set_frame(float frame);
	bool change_anim(std::string animation_name, float rate, float frame);
	void animate();

	//Model Funcs

	glm::vec3 get_bone_position(std::string bone_name, glm::vec3 offset = glm::vec3(0.0));
	glm::vec3 get_bone_position(int bone_id, glm::vec3 offset = glm::vec3(0.0));
	glm::vec3 get_relative_bone_position(std::string bone_name, glm::vec3 offset = glm::vec3(0.0));
	glm::vec3 get_relative_bone_position(int bone_id, glm::vec3 offset = glm::vec3(0.0));
	glm::quat get_bone_rotation_quat(std::string bone_name);
	glm::quat get_bone_rotation_quat(int bone_id);
	glm::vec3 get_bone_rotation(std::string bone_name);
	glm::vec3 get_bone_rotation(int bone_id);
	glm::vec3 get_bone_angle(int base_id, int angle_id);
	void load_model(std::string resource_dir, std::string texture_dir = "");
	void init_shader();

	//Sound Functions

	void play_se(std::string se, int vol = -1);
	void play_vc(std::string vc, int vol = -1);
	void stop_se(std::string se);
	void stop_se_all();
	void stop_vc(std::string vc);
	void stop_vc_all();
	void load_sound(std::string name);
	void unload_sound(std::string name);
	void unload_all_sounds();

	//Effect Functions

	void new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
		glm::vec4 rgba, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame, glm::vec3 rot_frame,
		glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate = 1.0, float frame = 0.0);

	void new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
		glm::vec4 rgba, std::string bone_name, glm::vec3 bone_offset, glm::vec3 pos_frame,
		glm::vec3 rot_frame, glm::vec3 scale_frame, glm::vec4 rgba_frame, float rate = 1.0,
		float frame = 0.0);

	void new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
		glm::vec4 rgba, glm::vec3 pos_frame, glm::vec3 rot_frame, glm::vec3 scale_frame,
		glm::vec4 rgba_frame, float rate = 1.0, float frame = 0.0);

	void new_effect_no_follow(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
		glm::vec4 rgba, glm::vec3 pos_frame, glm::vec3 rot_frame, glm::vec3 scale_frame,
		glm::vec4 rgba_frame, float rate = 1.0, float frame = 0.0);

	void clear_effect(std::string name, int instance = 0);
	void clear_effect_all();

	void load_effect(std::string name);
	void unload_effect(std::string name);
	void unload_all_effects();
};