#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <queue>
#include <cstdarg>
#include <stdarg.h>

//Declares a variable of the given type and extracts its value from the va_list
#define UNWRAP(var_name, type) type var_name = va_arg(args.va, type)
//Same as the above, but for variables that were already declared
#define UNWRAP_NO_DECL(var_name, type) var_name = va_arg(args.va, type)

struct ScriptArg {
	ScriptArg();
	ScriptArg(int num_args, std::va_list va);
	void destroy();
	int num_args;
	std::va_list va;
};

class ScriptFrame {
public:
	ScriptFrame();
	ScriptFrame(float frame);

	void execute();

	float frame;

	std::queue<std::function<void(ScriptArg)>> function_calls;
	std::queue<ScriptArg> function_args;
};

class MoveScript {
public:
	MoveScript();
	MoveScript(std::string name, std::function<void()> move_script);

	void activate(); //Clear the ScriptFrame queue, then call the move_script() function to repopulate
	//it
	void execute(float frame); //Checks if the front ScriptFrame is set to this frame. If it is, then
	//call its execute function, then remove it from the queue

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