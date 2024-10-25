#pragma once
#include <iostream>
#include <map>
#include <set>
#include "GameController.h"
#include "VectorBoolPointer.h"

class Fighter;

enum InputKind {
	INPUT_KIND_NORMAL,
	INPUT_KIND_236,
	INPUT_KIND_214,
	INPUT_KIND_623,
	INPUT_KIND_41236,
	INPUT_KIND_63214,
	INPUT_KIND_632,
	INPUT_KIND_46,
	INPUT_KIND_28,
	INPUT_KIND_66,
	INPUT_KIND_44,
	INPUT_KIND_22,
	INPUT_KIND_236236,
	INPUT_KIND_214214,
	INPUT_KIND_4646,
};

enum SpecialLevelSetting {
	SPECIAL_LEVEL_SETTING_N,
	SPECIAL_LEVEL_SETTING_L_M_H_EX,
	SPECIAL_LEVEL_SETTING_L_M_H,
	SPECIAL_LEVEL_SETTING_N_EX,

	SPECIAL_LEVEL_SETTING_MAX,
};

enum CancelKind {
	CANCEL_KIND_ANY,
	CANCEL_KIND_HIT,
	CANCEL_KIND_BLOCK,
	CANCEL_KIND_CONTACT,
};

#define MOVESET_DIR_ALL { 1, 2, 3, 4, 5, 6, 7, 8, 9 }
#define MOVESET_DIR_NEUTRAL { 4, 5, 6, 7, 8, 9 }
#define MOVESET_DIR_UP { 7, 8, 9 }
#define MOVESET_DIR_DOWN { 1, 2, 3 }
#define MOVESET_DIR_FORWARD { 3, 6, 9 }
#define MOVESET_DIR_BACK { 1, 4, 7}

class FighterMoveListEntry {
public:
	FighterMoveListEntry();
	FighterMoveListEntry(std::string name, unsigned int status_kind, unsigned short required_buttons,
		unsigned char num_required_buttons, std::set<unsigned int> stick_dirs, bool recover_crouching, 
		VBP disable_flag, float meter_cost_normal, float meter_cost_cancel, 
		bool super_meter, std::set<std::string> allowed_states
	);
	FighterMoveListEntry(std::string name, unsigned int status_kind, InputKind input_kind,
		unsigned short required_buttons, unsigned char num_required_buttons, int charge_req,
		SpecialLevelSetting special_level_setting, bool recover_crouching, 
		VBP disable_flag, float meter_cost_normal, float meter_cost_cancel, 
		bool super_meter, std::set<std::string> allowed_states
	);

	bool check_input(Fighter* fighter);
	bool try_change_status(Fighter* fighter);

	std::string name;
	unsigned int status_kind;
	InputKind input_kind;
	unsigned short required_buttons;
	unsigned char num_required_buttons;
	std::set<unsigned int> valid_stick_dirs;
	int charge_req;
	SpecialLevelSetting special_level_setting;
	std::set<std::string> allowed_states;
	bool recover_crouching;
	VBP disable_flag;
	float meter_cost_normal;
	float meter_cost_cancel;
	bool super_meter;

	bool hit_cancel_enabled;
	bool block_cancel_enabled;
	bool cancel_enabled;
	int special_level;
	short buttons_pressed;
};

class FighterMoveList {
public:
	FighterMoveList();
	
	void add_movelist_entry(std::string name, unsigned int status_kind, unsigned short required_buttons, 
		unsigned char num_required_buttons, std::set<unsigned int> stick_dirs, bool recover_crouching, 
		VBP disable_flag = VBP(), float meter_cost_normal = 0.0f, float meter_cost_cancel = 0.0f, 
		bool super_meter = false, std::set<std::string> allowed_states = {}
	);
	void add_movelist_entry(std::string name, unsigned int status_kind, InputKind input_kind,
		unsigned short required_buttons, unsigned char num_required_buttons, int charge_req,
		SpecialLevelSetting special_level_setting, bool recover_crouching,
		VBP disable_flag = VBP(), float meter_cost_normal = 0.0f, float meter_cost_cancel = 0.0f, 
		bool super_meter = false, std::set<std::string> allowed_states = {}
	);

	void check_inputs(Fighter* fighter);
	void try_changing_to_buffered_status(Fighter* fighter);
	bool is_curr_move_recover_crouching(Fighter* fighter);
	bool is_enable_cancel(Fighter* fighter, std::string move);
	void enable_cancel(std::string move, CancelKind cancel_kind);
	void disable_cancel(std::string move, CancelKind cancel_kind);
	void disable_all_cancels();
	void swap_buffers(std::string a, std::string b);
	bool is_buffered(std::string move);
	FighterMoveListEntry& get_move(std::string move);
private:
	std::vector<FighterMoveListEntry> move_list;
	std::map<std::string, int> move_map;
	std::set<int> buffered_moves;
};