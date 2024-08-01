#pragma once
#include "FighterMovelist.h"
#include "CircularBuffer.h"
#include "CollisionBox.h"
#include "glm/glm.hpp"
#include <list>
#include <set>
#include <deque>

struct CPUHitbox {
	glm::vec2 anchor;
	glm::vec2 offset;
	CollisionKind collision_kind;
	HitResult hit_result;
	HitStatus hit_status;
	unsigned int custom_hit_status;
	HitMove hit_move;
	HitFlag hit_flags;
	CriticalCondition critical_condition;
	HitStatus critical_status;
	unsigned int custom_critical_status;
	HitMove critical_hit_move;
	HitFlag critical_hit_flags;
	HitHeight hit_height;
};

struct CPUHurtbox {
	glm::vec2 anchor;
	glm::vec2 offset;
	HurtboxKind hurtbox_kind;
	int armor_hits;
	IntangibleKind intangible_kind;
};

struct CPUGrabbox {
	glm::vec2 anchor;
	glm::vec2 offset;
	GrabboxKind grabbox_kind;
	CollisionKind collision_kind;
	unsigned int attacker_status_if_hit;
	unsigned int defender_status_if_hit;
};

struct CPUFrameData {
	CPUFrameData(float pos_offset, std::map<int, CPUHitbox> hitboxes,
		std::map<int, CPUHurtbox> hurtboxes, std::map<int, CPUGrabbox> grabboxes,
		std::deque<std::string> hit_cancel_options, 
		std::deque<std::string> block_cancel_options,
		std::deque<std::string> cancel_options);
	float pos_offset;
	std::map<int, CPUHitbox> hitboxes;
	std::map<int, CPUHurtbox> hurtboxes;
	std::map<int, CPUGrabbox> grabboxes;
	std::deque<std::string> hit_cancel_options;
	std::deque<std::string> block_cancel_options;
	std::deque<std::string> cancel_options;
};

enum CPUTag {
	CPU_TAG_EVADE_ATK,
	CPU_TAG_IGNORE_STRIKE,
	CPU_TAG_IGNORE_THROW,
	CPU_TAG_AIR_ATK_CROSSUP,
	CPU_TAG_AIR_ATK_RISING,

	CPU_TAG_MAX,
};

struct CPUAction {
	CPUAction();
	void init(Fighter* owner, std::string script_name, std::string anim_name, int fighter_context, InputKind input_kind,
		unsigned short required_buttons, unsigned int pref_stick_dir, std::set<unsigned int> stick_dirs,
		std::vector<CPUTag> tags, std::vector<std::string> auto_followups, float resource_cost, 
		bool allow_interrupt, bool manual_input
	);
	void add_movement_info(float x_speed, float x_accel, float x_max, 
		float y_speed, float y_accel, float y_max);


	std::string name;
	int fighter_context;
	InputKind input_kind;
	unsigned short required_buttons;
	unsigned int pref_stick_dir;
	std::set<unsigned int> stick_dirs;

	int startup;
	int active;
	int recovery;
	int total;

	float x_speed;
	float x_accel;
	float x_max;

	float y_speed;
	float y_accel;
	float y_max;

	bool is_strike;
	bool is_throw;

	std::vector<CPUFrameData> frame_data;

	glm::vec2 attack_range_x;
	glm::vec2 attack_range_y;
	glm::vec2 defense_range_x;
	glm::vec2 defense_range_y;
	
	std::vector<CPUTag> tags;
	std::vector<std::string> auto_followups;
	std::set<std::string> hit_cancel_options;
	std::set<std::string> block_cancel_options;
	std::set<std::string> cancel_options;
	float resource_cost;
	bool allow_interrupt;
	bool manual_input;
};

enum CPUActionKind {
	CPU_ACTION_KIND_NORMAL,
	CPU_ACTION_KIND_HITSTUN,
	CPU_ACTION_KIND_BLOCKSTUN,
	CPU_ACTION_KIND_WAKEUP,
};

struct CPUFighterState {
	CPUFighterState();
	CPUAction *action;
	CPUActionKind action_kind;
	int context;
	int frame;
	float anim_frame;
	float facing_dir;
	glm::vec2 pos;
	glm::vec2 base_pos;
	glm::vec2 speed;
	bool actionable;
	int frames_until_bufferable;
};

struct FighterMoveContext {
	float distance_between_opponents;
	float player_corner_distance;
	float opponent_corner_distance;
	int time_spent_within_distance;
	int time_since_last_hit;
	int health;
	int opponent_bars;
	int airtime;
	int opponent_airtime;
	int frame_advantage;
};

struct OpponentResponseContext {
	float distance_between_opponents;
	float player_corner_distance;
	float opponent_corner_distance;
	int time_since_last_hit;
	int health;
	int opponent_bars;
	int airtime;
	int opponent_airtime;
	int block_advantage;
};

enum CPUMode {
	CPU_MODE_NEUTRAL,
	CPU_MODE_ATTACK,
	CPU_MODE_HITSTUN,
	CPU_MODE_WAKEUP_BLOCKSTUN,

	CPU_MODE_MAX
};

enum CPUDecisionMode {
	CPU_DECISION_MODE_NONE,
	CPU_DECISION_MODE_WALK_BUTTON,
	CPU_DECISION_MODE_WALK_COUNTERPOKE,
	CPU_DECISION_MODE_WALK_SHIMMY,
	CPU_DECISION_MODE_DASH_BUTTON,
	CPU_DECISION_MODE_JUMP_BUTTON,
	CPU_DECISION_MODE_ADVANCE,
	CPU_DECISION_MODE_SPECIAL,
};

const int CPU_JUMP_KIND_SHORTHOP_FORWARD = 0;
const int CPU_JUMP_KIND_FULLHOP_FORWARD = 1;
const int CPU_JUMP_KIND_NEUTRAL = 2;

class FighterCPU;
struct CPUDecision {
	CPUDecision();
	CPUDecision(CPUDecisionMode mode, FighterCPU* owner);
	CPUDecisionMode mode;
	CPUAction action;
	glm::vec2 target_dist_x;
	int frames;
	int misc;
};

class FighterCPU {
public:
	FighterCPU();

	void init(Fighter* owner, Fighter* opponent);
	CPUAction& add_action(std::string script_name, std::string anim_name, int fighter_context, InputKind input_kind, 
		unsigned short required_buttons, unsigned int pref_stick_dir, std::set<unsigned int> stick_dirs,
		std::vector<CPUTag> tags, std::vector<std::string> auto_followups, float resource_cost, 
		bool allow_interrupt
	);
	CPUAction& add_action(std::string script_name, std::string anim_name, std::vector<CPUTag> tags,
		std::vector<std::string> auto_followups, 
		bool allow_interrupt
	);
	void process();
	void process_neutral();
	void process_attack();
	void process_hitstun();
	void process_wakeup_blockstun();

	Fighter* owner;
	Fighter* opponent;

	//Input Variables
	
	int execution_error_margin; //When the CPU makes inputs, it passes a combination of
	//stick and button inputs which should result in a given action. This variable can
	//add random delays to the frame a button or stick input is made, which has a chance
	//to result in misinputs depending on where the delays are.

	//Reaction Variables

	int reaction_time; //Normally, the CPU can only access the opponent's state from
	//this many frames ago.
	int correct_prediction_time; //But in prediction mode, it can access the opponent's
	//state from this many, assuming that state includes an action that we were 
	//predicting
	int incorrect_prediction_time; //For actions we weren't predicting, the state has
	//to be this old.
	bool prediction_mode; //When this is false, the CPU will make decisions based on 
	//the opponent's "current" state. When this is true, the CPU will make decisions
	//based on what it predicts the opponent's state will be.
	int hit_confirm_skill; //This variable affects how good the CPU is at hit 
	//confirming.
	int block_recognition_skill; //This variable affects how well the CPU can tell whether or not
	//it needs to block high/low.


	//State Determination Variables

	int movement_error_margin; //When the CPU is calculating distances, it does so by
	//predicting how many frames it will take before it is a certain distance away from
	//the opponnt. This varible is randomly added to or subtracted from that number of
	//frames.
	float move_range_error_margin; //When the CPU is determining the range of either its
	//own moves or the opponent's, this margin is randomly added to or subtracted from 
	//that.
	int mental_stack_update_interval; //When the CPU is in prediction mode, it reacts
	//extra fast to options in its mental stack, but extra slow to other options. This
	//variable determines how often the mental stack is updated.
	int decision_update_interval; //When the CPU is trying to decide what to do outside
	//of reactions, this determines how often it's allowed to change outside of a neutral 
	//state

	//Decision Variables

	int cautiousness; //When making guesses outside of prediction mode, the CPU needs 
	//to guess the same action this many times.
	int impulse; //This variable affects how likely the CPU is to commit to an action
	//without verifying that said option is safe or optimal


	//Misc. Variables

	int tilt_modifier;
	int comeback_factor;
	bool locked_in;
	int stamina;

	std::map<std::string, CPUAction> actions;
	CircularBuffer<CPUFighterState> states;
	CircularBuffer<CPUFighterState> opponent_states;

	CPUMode cpu_mode;
	CPUDecision curr_decision;
private:
	void execute_action(CPUAction action);
	void buffer_action(CPUAction action);
	void determine_states();
	int get_hit_frame(CPUAction atk_action, int atk_frame, float atk_facing_dir, glm::vec2 atk_base_pos,
		CPUAction def_action, int def_frame, float def_facing_dir, glm::vec2 def_base_pos, bool this_atk
	);
	bool check_contact(CPUFighterState& state, CPUFighterState& opp_state);
	bool check_contact(CPUFrameData& frame_data, glm::vec2 pos, float facing_dir, 
		CPUFrameData& opp_frame_data, glm::vec2 opp_pos, float opp_facing_dir);
	void add_input(unsigned int input_stick, unsigned int input_buttons);
	int get_frames_to_input(CPUAction action);
	unsigned int numpad_to_bits(unsigned int numpad_dir);

	int input_frames;
};