#pragma once
#include "GameObject.h"
#include "MoveScript.h"
#include "CollisionBox.h"
#include "Param.h"
#include "VariadicHelpers.h"

enum BattleObjectType {
	BATTLE_OBJECT_TYPE_FIGHTER,
	BATTLE_OBJECT_TYPE_PROJECTILE,

	BATTLE_OBJECT_TYPE_MAX
};


class Player;
class ObjectManager;
class Stage;
class Fighter;
class Projectile;

class BattleObject : public GameObject {
public:
	BattleObject();

	//Loading

	void load_params();

	//Position

	void add_pos(glm::vec3 pos);
	void set_pos(glm::vec3 pos);

	//Blockbox

	void new_blockbox(glm::vec2 anchor, glm::vec2 offset);
	void update_blockbox_pos();
	void clear_blockbox();

	//Hitbox

	void new_hitbox(int id, int multihit, glm::vec2 anchor, glm::vec2 offset,
		CollisionKind collision_kind, HitResult hit_result, HitStatus hit_status, 
		unsigned int custom_hit_status, HitMove hit_move, HitFlag hit_flags, 
		CriticalCondition critical_condition, HitStatus critical_status,
		unsigned int custom_critical_status, HitMove critical_hit_move,
		HitFlag critical_hit_flags, HitHeight hit_height, DamageKind damage_kind, 
		std::string hit_effect, std::string hit_sound
	);
	void update_hitbox_connect(int multihit_index);
	void update_hitbox_pos();
	bool is_hitbox_active(int multihit = -1);
	virtual void clear_hitbox(int id);
	virtual void clear_hitbox_all();

	//Definite Hitbox

	void set_definite_hitbox(Fighter* target, unsigned int hit_status,
		HitFlag hit_flags, int juggle_start, int juggle_increase, float damage, int damage_scale,
		float meter_gain, int hitlag, int hitstun, std::string hit_anim, HitMove hit_move, 
		DamageKind damage_kind, std::string hit_effect, std::string hit_sound
	);

	//Hurtbox

	void new_hurtbox(int id, glm::vec2 anchor, glm::vec2 offset, HurtboxKind hurtbox_kind, int armor_hits, IntangibleKind intangible_kind);
	void update_hurtbox_pos();
	void clear_hurtbox(int id);
	void clear_hurtbox_all();
	void set_hurtbox_intangible_kind(int id, IntangibleKind intangible_kind);

	//Grabbox

	void new_grabbox(int id, glm::vec2 anchor, glm::vec2 offset, GrabboxKind grabbox_kind, 
		CollisionKind hit_kind, unsigned int attacker_status_if_hit, unsigned int defender_status_if_hit);
	void update_grabbox_pos();
	void clear_grabbox(int id);
	void clear_grabbox_all();

	//Pushbox

	void new_pushbox(int id, glm::vec2 anchor, glm::vec2 offset);
	void update_pushbox_pos();
	void clear_pushbox(int id);
	void clear_pushbox_all();
	bool pushboxes_touching(BattleObject* object);
	float get_pushbox_front(size_t id);
	float get_pushbox_back(size_t id);

	//Anim Funcs

	glm::vec3 get_trans_offset();
	std::string get_anim();
	std::string get_anim_broad();
	int get_anim_length(std::string anim_name);

	//Param Funcs

	int get_param_int(std::string param);
	float get_param_float(std::string param);
	std::string get_param_string(std::string param);
	bool get_param_bool(std::string param);

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

	//Status Functions

	virtual void change_status(unsigned int new_status_kind, bool call_end_status = true);

	//Collision Functions

	void check_incoming_hitbox_collisions(std::vector<Hitbox*> hitboxes);
	bool is_valid_incoming_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox);
	virtual bool is_valid_incoming_fighter_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Fighter* attacker);
	virtual bool is_valid_incoming_projectile_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Projectile* attacker);

	void process_incoming_hitbox_collision(Hitbox* hitbox, BattleObject* attacker);
	
	void process_incoming_hitbox_collision_hit(Hitbox* hitbox, BattleObject* attacker);
	virtual void process_incoming_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* attacker);
	virtual void process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker);

	void process_outgoing_hitbox_collision_hit(Hitbox* hitbox, BattleObject* defender);
	virtual void process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender);
	virtual void process_outgoing_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* defender);

	void process_incoming_hitbox_collision_blocked(Hitbox* hitbox, BattleObject* attacker);
	virtual void process_incoming_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* attacker);
	virtual void process_incoming_projectile_hitbox_collision_blocked(Hitbox* hitbox, Projectile* attacker);

	void process_outgoing_hitbox_collision_blocked(Hitbox* hitbox, BattleObject* defender);
	virtual void process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender);

	void process_incoming_hitbox_collision_parried(Hitbox* hitbox, BattleObject* attacker);
	virtual void process_incoming_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* attacker);
	virtual void process_incoming_projectile_hitbox_collision_parried(Hitbox* hitbox, Projectile* attacker);

	void process_outgoing_hitbox_collision_parried(Hitbox* hitbox, BattleObject* defender);
	virtual void process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender);

	void process_incoming_hitbox_collision_hitstun_parried(Hitbox* hitbox, BattleObject* attacker);
	virtual void process_incoming_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* attacker);
	virtual void process_incoming_projectile_hitbox_collision_hitstun_parried(Hitbox* hitbox, Projectile* attacker);

	void process_outgoing_hitbox_collision_hitstun_parried(Hitbox* hitbox, BattleObject* defender);
	virtual void process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender);

	void process_incoming_hitbox_collision_armored(Hitbox* hitbox, BattleObject* attacker);
	virtual void process_incoming_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* attacker);
	virtual void process_incoming_projectile_hitbox_collision_armored(Hitbox* hitbox, Projectile* attacker);

	void process_outgoing_hitbox_collision_armored(Hitbox* hitbox, BattleObject* defender);
	virtual void process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender);

	void process_incoming_hitbox_collision_right_of_way_armored(Hitbox* hitbox, BattleObject* attacker);
	virtual void process_incoming_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* attacker);
	virtual void process_incoming_projectile_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Projectile* attacker);

	void process_outgoing_hitbox_collision_right_of_way_armored(Hitbox* hitbox, BattleObject* defender);
	virtual void process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender);

	void process_incoming_hitbox_collision_invincibility(Hitbox* hitbox, BattleObject* attacker);
	virtual void process_incoming_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* attacker);
	virtual void process_incoming_projectile_hitbox_collision_invincibility(Hitbox* hitbox, Projectile* attacker);

	void process_outgoing_hitbox_collision_invincibility(Hitbox* hitbox, BattleObject* defender);
	virtual void process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender);

	void process_incoming_hitbox_collision_counter(Hitbox* hitbox, BattleObject* attacker);
	virtual void process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker);
	virtual void process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker);

	void process_outgoing_hitbox_collision_counter(Hitbox* hitbox, BattleObject* defender);
	virtual void process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender);
	virtual void process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender);

	//Scripting Functions

	void set_active_script(std::string anim_name);
	void execute_active_script();
	void clear_scripts();

	void script(std::string name, std::function<void()> move_script);
	void execute_frame(float frame, std::function<void()> execute);
	void execute_wait(float frames, std::function<void()> execute);
	void push_condition(std::string condition_name, std::function<bool()> condition);

	template<typename ...T>
	void push_function(void (BattleObject::* function)(ScriptArg), T... args) {
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.push_function(function, sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	template<typename ...T>
	void push_true(std::string condition_name, void (BattleObject::* function)(ScriptArg), T... args) {
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.push_true(condition_name, function, sa);
		for (int i = 0, max = error_vec.size(); i < max; i++) {
			GameManager::get_instance()->add_crash_log("ERROR: Arg " + std::to_string(error_vec[i] + 1) +
				" of a function called in script " + active_move_script.name + " is of type unnamed-enum.");
		}
	}

	template<typename ...T>
	void push_false(std::string condition_name, void (BattleObject::* function)(ScriptArg), T... args) {
		std::vector<int> error_vec;
		std::queue<std::any> queue = extract_variadic_to_queue(&error_vec, args...);
		ScriptArg sa(sizeof...(args), queue);
		active_script_frame.push_false(condition_name, function, sa);
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

	void SET_DEFINITE_HITBOX(ScriptArg args);

	void NEW_HURTBOX(ScriptArg args);
	void CLEAR_HURTBOX(ScriptArg args);
	void CLEAR_HURTBOX_ALL(ScriptArg args);
	void SET_HURTBOX_INTANGIBLE_KIND(ScriptArg args);
	
	void NEW_GRABBOX(ScriptArg args);
	void CLEAR_GRABBOX(ScriptArg args);
	void CLEAR_GRABBOX_ALL(ScriptArg args);

	void NEW_PUSHBOX(ScriptArg args);
	void CLEAR_PUSHBOX(ScriptArg args);
	void CLEAR_PUSHBOX_ALL(ScriptArg args);

	void PLAY_SOUND(ScriptArg args);
	void PLAY_RESERVED_SOUND(ScriptArg args);

	void NEW_EFFECT(ScriptArg args);
	void NEW_EFFECT_NO_FOLLOW(ScriptArg args);

	void CLEAR_EFFECT(ScriptArg args);
	void CLEAR_EFFECT_ALL(ScriptArg args);

	void PRINT_MSG_FROM_SCRIPT(ScriptArg args);

	int id;
	BattleObjectType object_type;
	bool has_model;

	glm::vec3 prev_pos;
	float facing_dir;
	float internal_facing_dir;
	bool facing_right;
	bool internal_facing_right;

	unsigned int status_kind;
	unsigned int prev_status_kind;

	DefiniteHitbox definite_hitbox;
	Blockbox blockbox;
	Hitbox hitboxes[10];
	Hurtbox hurtboxes[10];
	Grabbox grabboxes[10];
	Pushbox pushboxes[10];
	bool multihit_connected[10];

	Hitbox* connected_hitbox;
	Grabbox* connected_grabbox;
	IncomingCollisionKind incoming_collision_kind;
	unsigned int post_collision_status;

	MoveScriptTable move_script_table;
	MoveScript active_move_script;
	ScriptFrame active_script_frame;
	float last_execute_frame;

	ParamTable param_table;

	Player* player;
	Stage* stage;
};