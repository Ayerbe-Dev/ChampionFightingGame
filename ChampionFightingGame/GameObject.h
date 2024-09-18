#pragma once
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "Animation.h"
#include "SoundPlayer.h"
#include "CircularBuffer.h"

class WindowManager;
class SoundManager;
class EffectManager;
class ShaderManager;
class ObjectManager;

class GameObject : public SoundPlayer {
public:
	GameObject();
	GameObject(GameObject& other);
	GameObject(const GameObject& other);
	GameObject& operator=(GameObject& other);
	GameObject& operator=(const GameObject& other);
	~GameObject();

	//Process Funcs

	void process_animate();

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
	void set_alpha(unsigned char alpha);
	void add_alpha(short alpha);
	unsigned char get_alpha() const;
	void render();
	void render_shadow();

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

	//Transformation Funcs

	void add_pos(glm::vec3 pos);
	void set_pos(glm::vec3 pos);
	void set_pos_x(float pos);
	void set_pos_y(float pos);
	void set_pos_z(float pos);
	glm::vec3 get_scaled_pos() const;
	glm::vec3 get_pos() const;
	void add_rot(glm::vec3 rot);
	void set_rot(glm::vec3 rot);
	void set_rot_x(float rot);
	void set_rot_y(float rot);
	void set_rot_z(float rot);
	glm::vec3 get_rot() const;	
	void add_scale(glm::vec3 scale);
	void set_scale(glm::vec3 scale);
	glm::vec3 get_scale() const;
	glm::vec3 get_scale_vec() const;
	glm::vec3 get_front() const;
	glm::vec3 get_right() const;
	glm::vec3 get_up() const;

	//Animation Funcs

	void set_rate(float rate);
	void set_frame(float frame);
	bool change_anim(std::string animation_name, float rate, float frame);

	//Camera Funcs

	void start_cinematic_sequence(std::string anim_kind, float anim_rate, float anim_frame, float world_brightness, bool dim_self, float world_rate);
	void stop_cinematic_sequence();

	void play_camera_anim(std::string anim_kind, float rate, float frame);
	void stop_camera_anim();

	void slow_world(float world_rate);
	void reset_world_rate();

	void dim_world(float world_brightness, bool dim_self);
	void reset_world_brightness();

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

	EffectManager* effect_manager;
	WindowManager* window_manager;
	ShaderManager* shader_manager;
	ObjectManager* object_manager;

	std::string resource_dir;

	glm::vec3 prev_pos;
	glm::mat4 extra_mat;

	AnimationTable anim_table;
	Animation* anim_kind;
	Animation* prev_anim_kind;
	glm::vec3 prev_anim_offset;
	float frame;
	float frame_delta;
	float rate;
	float prev_anim_rate;
	float prev_anim_frame;
	bool anim_end;

	int id_effect;

	Shader* shader;
	Shader* shadow_shader;
	CircularBuffer<GLuint> trail_buffer;
	ModelInstance model;
private:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	glm::vec3 scale_vec;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
};