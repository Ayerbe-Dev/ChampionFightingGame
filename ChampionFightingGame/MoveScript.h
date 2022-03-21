#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <unordered_map>

class MoveScript {
public:
	std::string name;
	std::function<void()> move_script{ []() {} };

	MoveScript();
	MoveScript(std::string name, std::function<void()> move_script);
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