#pragma once
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "Animation.h"
#include "SoundPlayer.h"

class RenderManager;
class SoundManager;
class EffectManager;
class ShaderManager;
class ObjectManager;

class GameObject {
public:
	GameObject();
	GameObject(GameObject& other);
	GameObject(const GameObject& other);
	GameObject& operator=(GameObject& other);
	GameObject& operator=(const GameObject& other);
	~GameObject();

	//Process Funcs
	void process_render_pos();

	//Loading Funcs

	void load_model(std::string resource_dir, std::string texture_dir = "");
	void load_used_model(std::string resource_dir, std::string texture_dir = "");
	void init_shader();
	void load_anim_table(std::string anim_dir);
	void load_anim_table_unloaded_model(std::string anim_dir, std::string model_filename);
	void load_anim_single(std::string name, std::string anim_filename, int end_frame, bool flag_move, bool flag_no_hitlag_interp);
	void load_anim_single_unloaded_model(std::string name, std::string anim_filename, std::string model_filename, int end_frame, bool flag_move, bool flag_no_hitlag_interp);

	//Render Funcs

	bool is_in_camera_range();
	void render();
	void render_shadow();
	void render_outline();

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
	void set_scale(glm::vec3 scale);

	//Animation Funcs

	void set_rate(float rate);
	void set_frame(float frame);
	bool change_anim(std::string animation_name, float rate, float frame);
	void animate();

	//Sound Functions

	void play_sound(std::string sound, float volume_mod);
	void pause_sound(std::string sound);
	void pause_sound_all();
	void resume_sound(std::string sound);
	void resume_sound_all();
	void stop_sound(std::string sound);
	void stop_sound_all();
	void play_reserved_sound(std::string sound, float volume_mod);
	void pause_reserved_sound();
	void resume_reserved_sound();
	void stop_reserved_sound();
	void load_sound(std::string name, std::string dir);
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

	RenderManager* render_manager;
	SoundManager* sound_manager;
	EffectManager* effect_manager;
	ShaderManager* shader_manager;
	ObjectManager* object_manager;

	std::string resource_dir;

	glm::vec3 pos;
	glm::vec3 render_pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec3 scale_vec;
	glm::mat4 extra_mat;

	AnimationTable anim_table;
	Animation* anim_kind;
	Animation* prev_anim_kind;
	glm::vec3 prev_anim_offset;
	float frame;
	float rate;
	float prev_anim_rate;
	float prev_anim_frame;
	bool anim_end;

	SoundPlayer sound_player;
	int id_effect;

	Shader* shader;
	Shader* shadow_shader;
	Shader* outline_shader;
	ModelInstance model;
};