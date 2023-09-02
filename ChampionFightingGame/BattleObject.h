#pragma once
#include "GameObject.h"
#include "MoveScript.h"
#include "Box.h"
#include "Param.h"
#include "VariadicHelpers.h"
#include "BattleObjectConstants.h"

class Player;
class BattleObjectManager;
class Stage;

class BattleObject : public GameObject {
public:
	BattleObject();

	int id;
	BattleObjectType object_type;
	bool has_model;

	glm::vec3 prev_pos = glm::vec3(0.0);
	glm::vec3 extra_rot = glm::vec3(0.0);
	float facing_dir = 1.0;
	float internal_facing_dir = 1.0;
	bool facing_right = true;
	bool internal_facing_right = true;

	unsigned int status_kind = 0;
	unsigned int situation_kind = 0;

	Blockbox blockbox;
	Hitbox hitboxes[10];
	Hurtbox hurtboxes[10];
	Grabbox grabboxes[10];
	Pushbox pushboxes[10];
	bool multihit_connected[10] = { false };

	MoveScriptTable move_script_table;
	MoveScript active_move_script;
	ScriptFrame active_script_frame;
	ScriptCondition *active_script_condition;
	float last_execute_frame;

	ParamTable stats;
	ParamTable params;

	Player* player;
	Stage* stage;
	BattleObjectManager* battle_object_manager;

	//Loading

	void load_stats();
	void load_params();

	//Blockbox

	void new_blockbox(glm::vec2 anchor, glm::vec2 offset);
	void update_blockbox_pos();
	void clear_blockbox();

	//Hitbox

	void new_hitbox(int id, int multihit, float damage, float chip_damage,
		int damage_scale, float meter_gain, glm::vec2 anchor, glm::vec2 offset, HitKind hit_kind,
		AttackLevel attack_level, AttackHeight attack_height, int hitlag, int blocklag, int hitstun,
		int blockstun, float hit_pushback, float block_pushback, HitStatus hit_status,
		HitStatus counterhit_status, CounterhitType counterhit_type, int juggle_start, int juggle_increase,
		int juggle_max, ClankKind clank_kind, DamageKind ko_kind, bool continue_launch,
		bool disable_hitstun_parry, float launch_init_y, float launch_gravity_y,
		float launch_max_fall_speed, float launch_speed_x
	);
	void update_hitbox_connect(int multihit_index);
	void update_hitbox_pos();
	bool is_hitbox_active(int multihit = -1);
	virtual void clear_hitbox(int id);
	virtual void clear_hitbox_all();

	//Hurtbox

	void new_hurtbox(int id, glm::vec2 anchor, glm::vec2 offset, HurtboxKind hurtbox_kind, bool armor, IntangibleKind intangible_kind);
	void update_hurtbox_pos();
	void clear_hurtbox(int id);
	void clear_hurtbox_all();

	//Grabbox

	void new_grabbox(int id, glm::vec2 anchor, glm::vec2 offset, GrabboxKind grabbox_kind, 
		HitKind hit_kind, unsigned int attacker_status_if_hit, unsigned int defender_status_if_hit);
	void update_grabbox_pos();
	void clear_grabbox(int id);
	void clear_grabbox_all();

	//Pushbox

	void new_pushbox(int id, glm::vec2 anchor, glm::vec2 offset);
	void update_pushbox_pos();
	void clear_pushbox(int id);
	void clear_pushbox_all();
	bool pushboxes_touching(BattleObject* object);
	float get_pushbox_front();
	float get_pushbox_back();

	//Anim Funcs

	glm::vec3 get_trans_offset();
	std::string get_anim();
	std::string get_anim_broad();
	int get_anim_length(std::string anim_name);

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
	glm::vec3 get_bone_angle(int base_id, int angle_id);

	//Effect Functions

	void new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
		glm::vec4 rgba, int bone_id, glm::vec3 bone_offset, glm::vec3 pos_frame, glm::vec3 rot_frame,
		glm::vec3 scale_frame, glm::vec4 rgba_frame, int* interp_var = nullptr, float rate = 1.0, float frame = 0.0);

	void new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
		glm::vec4 rgba, std::string bone_name, glm::vec3 bone_offset, glm::vec3 pos_frame,
		glm::vec3 rot_frame, glm::vec3 scale_frame, glm::vec4 rgba_frame, int* interp_var = nullptr, float rate = 1.0,
		float frame = 0.0);

	void new_effect(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
		glm::vec4 rgba, glm::vec3 pos_frame, glm::vec3 rot_frame, glm::vec3 scale_frame,
		glm::vec4 rgba_frame, int* interp_var = nullptr, float rate = 1.0, float frame = 0.0);

	void new_effect_no_follow(std::string name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
		glm::vec4 rgba, glm::vec3 pos_frame, glm::vec3 rot_frame, glm::vec3 scale_frame,
		glm::vec4 rgba_frame, int* interp_var = nullptr, float rate = 1.0, float frame = 0.0);

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
	void push_condition(std::function<bool()> condition);

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

	template<typename ...T>
	void push_true(void (BattleObject::* function)(ScriptArg), T... args) {
		if (active_script_condition == nullptr) {
			GameManager::get_instance()->add_crash_log("ERROR: Tried to push conditional function to script "
				+ active_move_script.name + " without declaring a condition.");
			return;
		}
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_condition->true_calls.push(function);
		active_script_condition->true_args.push(sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	template<typename ...T>
	void push_false(void (BattleObject::* function)(ScriptArg), T... args) {
		if (active_script_condition == nullptr) {
			GameManager::get_instance()->add_crash_log("ERROR: Tried to push conditional function to script "
				+ active_move_script.name + " without declaring a condition.");
			return;
		}
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_condition->false_calls.push(function);
		active_script_condition->false_args.push(sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	//Script Wrappers

	void SET_RATE(ScriptArg args);
	void SET_FRAME(ScriptArg args);

	void NEW_BLOCKBOX(ScriptArg args);

	void NEW_HITBOX(ScriptArg args);
	void CLEAR_HITBOX(ScriptArg args);
	void CLEAR_HITBOX_ALL(ScriptArg args);

	void NEW_HURTBOX(ScriptArg args);
	void CLEAR_HURTBOX(ScriptArg args);
	void CLEAR_HURTBOX_ALL(ScriptArg args);
	
	void NEW_GRABBOX(ScriptArg args);
	void CLEAR_GRABBOX(ScriptArg args);
	void CLEAR_GRABBOX_ALL(ScriptArg args);

	void NEW_PUSHBOX(ScriptArg args);
	void CLEAR_PUSHBOX(ScriptArg args);
	void CLEAR_PUSHBOX_ALL(ScriptArg args);

	void PLAY_SE(ScriptArg args);
	void PLAY_VC(ScriptArg args);

	void NEW_EFFECT(ScriptArg args);
	void NEW_EFFECT_NO_FOLLOW(ScriptArg args);

	void CLEAR_EFFECT(ScriptArg args);
	void CLEAR_EFFECT_ALL(ScriptArg args);

	void PRINT_MSG_FROM_SCRIPT(ScriptArg args);
};