#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cstdarg>
#include <stdarg.h>
#include <any>

//Declares a variable of the given type and extracts its value from the queue
#define UNWRAP(var_name, type) type var_name = std::any_cast<type>(args.get_arg())
//Same as the above, but for variables that were already declared
#define UNWRAP_NO_DECL(var_name) if (!args.args.empty()) { var_name = std::any_cast<decltype(var_name)>(args.get_arg()); }

class BattleObject;

struct ScriptArg {
	ScriptArg();
	ScriptArg(int num_args, std::queue<std::any> args);

	std::any get_arg();
	void pop();
	int num_args;
	
	std::queue<std::any> args;
};

class ScriptFrame {
public:
	ScriptFrame();
	ScriptFrame(float frame);

	void execute(BattleObject* object);

	float frame;
	std::queue<void(BattleObject::*)(ScriptArg)> function_calls;
	std::queue<ScriptArg> function_args;
};

class MoveScript {
public:
	MoveScript();
	MoveScript(std::string name, std::function<void()> move_script);

	void activate(); //Clear the ScriptFrame queue, then call the move_script() function to repopulate it
	void execute(BattleObject* object, float frame); //Checks if the front ScriptFrame is set to this frame. If it is, then call its execute function and remove it from the queue
	bool has_function(float frame, void(BattleObject::* func)(ScriptArg), ScriptArg* args_ret = nullptr);

	std::string name;
	std::queue<ScriptFrame> frames;
private:
	std::function<void()> move_script{ []() {} };
};

class MoveScriptTable {
public:
	MoveScriptTable();
	void add_script(std::string name, std::function<void()> move_script);
	MoveScript get_script(std::string name);
	void wipe_scripts();
private:
	std::vector<MoveScript> scripts;
	std::unordered_map<std::string, int> script_map;
};