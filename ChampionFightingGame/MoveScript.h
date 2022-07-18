#pragma once
#include "ScriptArg.h"
#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>
#include <cstdarg>
#include <stdarg.h>

class BattleObject;
class Fighter;
class Projectile;

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
	bool has_function(void(BattleObject::* func)(ScriptArg), ScriptArg* args_ret = nullptr);
	bool has_function(float frame, void(Fighter::* func)(ScriptArg), ScriptArg* args_ret = nullptr);
	bool has_function(void(Fighter::* func)(ScriptArg), ScriptArg* args_ret = nullptr);
	bool has_function(float frame, void(Projectile::* func)(ScriptArg), ScriptArg* args_ret = nullptr);
	bool has_function(void(Projectile::* func)(ScriptArg), ScriptArg* args_ret = nullptr);

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