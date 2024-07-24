#pragma once
#include "ScriptFunc.h"
#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <cstdarg>
#include <stdarg.h>
#include <map>

class BattleObject;
class Fighter;
class Projectile;

class ScriptFrame {
public:
	ScriptFrame();
	ScriptFrame(float frame);

	void push_function(void(BattleObject::* func)(ScriptArg), ScriptArg args);
	void push_true(std::string condition_name, void(BattleObject::* func)(ScriptArg), ScriptArg args);
	void push_false(std::string condition_name, void(BattleObject::* func)(ScriptArg), ScriptArg args);

	float frame;
	std::queue<ScriptFunc<BattleObject>> script_funcs;
};

class MoveScriptTable;

class MoveScript {
public:
	MoveScript();
	MoveScript(std::string name, std::function<void()> move_script, MoveScriptTable* table);

	void activate();
	bool has_next_frame(float frame);
	ScriptFrame get_next_frame();
	
	bool has_function(float frame, void(BattleObject::* func)(ScriptArg), ScriptArg* args_ret = nullptr);
	bool has_function(void(BattleObject::* func)(ScriptArg), ScriptArg* args_ret = nullptr);
	bool has_function(float frame, void(Fighter::* func)(ScriptArg), ScriptArg* args_ret = nullptr);
	bool has_function(void(Fighter::* func)(ScriptArg), ScriptArg* args_ret = nullptr);
	bool has_function(float frame, void(Projectile::* func)(ScriptArg), ScriptArg* args_ret = nullptr);
	bool has_function(void(Projectile::* func)(ScriptArg), ScriptArg* args_ret = nullptr);

	std::string name;
	std::queue<ScriptFrame> frames;
	std::map<std::string, std::function<bool()>> conditions;
private:
	std::function<void()> move_script;
	MoveScriptTable* table;
};

class MoveScriptTable {
public:
	MoveScriptTable();
	void add_script(std::string name, std::function<void()> move_script);
	MoveScript get_script(std::string name);
	void clear_scripts();


	void set_activating_move_script(MoveScript* script);
	void execute_frame(float frame, std::function<void()> execute);
	void execute_wait(float frames, std::function<void()> execute);
	void push_condition(std::string condition_name, std::function<bool()> condition);
	void push_function(void(BattleObject::* func)(ScriptArg), ScriptArg args);
	void push_true(std::string condition_name, void(BattleObject::* func)(ScriptArg), ScriptArg args);
	void push_false(std::string condition_name, void(BattleObject::* func)(ScriptArg), ScriptArg args);
private:
	std::vector<MoveScript> scripts;
	std::unordered_map<std::string, int> script_map;
	
	MoveScript *activating_move_script;
	ScriptFrame activating_script_frame;
	float last_execute_frame;
};