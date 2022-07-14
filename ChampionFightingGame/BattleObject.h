#pragma once
#include "RenderObject.h"
#include "Animation.h"
#include "MoveScript.h"
#include "Box.h"
#include "Param.h"
#include "FighterStatus.h"
#include "FighterAttribute.h"
#include "FighterInt.h"
#include "FighterFloat.h"
#include "FighterFlag.h"
#include  "VariadicHelpers.h"

class Player;
class BattleObjectManager;
class SoundManager;
class EffectManager;
class Stage;

class BattleObject : public RenderObject {
public:
	BattleObject();

	int id;
	int object_type;
	bool has_model;
	std::string resource_dir;

	glm::vec3 prev_pos = glm::vec3(0.0);
	glm::vec3 extra_rot = glm::vec3(0.0);
	float facing_dir = 1.0;
	float internal_facing_dir = 1.0;
	bool facing_right = true;
	bool internal_facing_right = true;

	unsigned int status_kind = FIGHTER_STATUS_WAIT;
	unsigned int situation_kind = FIGHTER_SITUATION_GROUND;

	GameRect jostle_box;
	glm::vec2 base_jostle_anchor;
	glm::vec2 base_jostle_offset;

	AnimationTable anim_table;
	Animation* anim_kind;
	Animation* prev_anim_kind;
	glm::vec3 prev_anim_offset;
	float frame;
	float rate;
	float prev_anim_rate;
	float prev_anim_frame;
	bool is_anim_end = false;

	Blockbox blockbox;
	Hitbox hitboxes[10];
	Grabbox grabboxes[10];
	Hurtbox hurtboxes[10];
	bool multihit_connected[10] = { false };

	MoveScriptTable move_script_table;
	MoveScript active_move_script;
	ScriptFrame active_script_frame;
	float last_execute_frame;

	ParamTable stats;
	ParamTable params;

	Player* player;
	Stage* stage;
	BattleObjectManager* battle_object_manager;
	SoundManager* sound_manager;
	EffectManager* effect_manager;

	//Loading

	void load_stats();
	void load_params();

	//Animation

	void set_rate(float rate);
	void set_frame(float frame);
	

	//Blockbox

	void new_blockbox(glm::vec2 anchor, glm::vec2 offset);
	void update_blockbox_pos();
	void clear_blockbox();

	//Hitbox

	void update_hitbox_connect(int multihit_index);
	void update_hitbox_pos();
	bool is_hitbox_active(int multihit = -1);

	//Grabbox

	void update_grabbox_pos();
	void clear_grabbox(int id);
	void clear_grabbox_all();

	//Hurtbox

	void update_hurtbox_pos();
	void clear_hurtbox(int id);
	void clear_hurtbox_all();

	//Param Funcs

	int get_local_param_int(std::string param);
	int get_local_param_int(std::string param, ParamTable param_table);
	float get_local_param_float(std::string param);
	float get_local_param_float(std::string param, ParamTable param_table);
	std::string get_local_param_string(std::string param);
	std::string get_local_param_string(std::string param, ParamTable param_table);
	bool get_local_param_bool(std::string param);
	bool get_local_param_bool(std::string param, ParamTable param_table);

	//Model Funcs

	glm::vec3 get_bone_position(std::string bone_name, glm::vec3 offset = glm::vec3(0.0));
	glm::vec3 get_bone_position(int bone_id, glm::vec3 offset = glm::vec3(0.0));
	glm::vec3 get_relative_bone_position(std::string bone_name, glm::vec3 offset = glm::vec3(0.0));
	glm::vec3 get_relative_bone_position(int bone_id, glm::vec3 offset = glm::vec3(0.0));
	glm::quat get_bone_rotation_quat(std::string bone_name);
	glm::quat get_bone_rotation_quat(int bone_id);
	glm::vec3 get_bone_rotation(std::string bone_name);
	glm::vec3 get_bone_rotation(int bone_id);
	glm::vec3 get_rotated_bone_position(std::string bone_name, glm::vec3 offset = glm::vec3(0.0));
	glm::vec3 get_rotated_bone_position(int bone_id, glm::vec3 offset = glm::vec3(0.0));
	glm::vec3 get_rotated_relative_bone_position(std::string bone_name, glm::vec3 offset = glm::vec3(0.0));
	glm::vec3 get_rotated_relative_bone_position(int bone_id, glm::vec3 offset = glm::vec3(0.0));

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

	//Scripting functions

	void script(std::string name, std::function<void()> move_script);
	void wipe_scripts();
	void set_current_move_script(std::string anim_name);
	void execute_frame(float frame, std::function<void()> execute);
	void execute_wait(float frames, std::function<void()> execute);

	template<typename ...T>
	void push_function(void (BattleObject::* function)(ScriptArg), T... args) {
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.function_calls.push(function);
		active_script_frame.function_args.push(sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	//Script Wrappers

	void SET_RATE(ScriptArg args);
	void SET_FRAME(ScriptArg args);

	void NEW_BLOCKBOX(ScriptArg args);

	void PLAY_SE(ScriptArg args);
	void PLAY_VC(ScriptArg args);

	void NEW_EFFECT(ScriptArg args);
	void NEW_EFFECT_NO_FOLLOW(ScriptArg args);
};