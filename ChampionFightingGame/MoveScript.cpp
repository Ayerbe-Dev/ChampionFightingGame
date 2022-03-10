#include "MoveScript.h"

MoveScript::MoveScript() {};

MoveScript::MoveScript(std::string name, std::function<void()> move_script) {
	this->name = name;
	this->move_script = move_script;
}

MoveScriptTable::MoveScriptTable() {
	add_script("default", []() {
		return;
	});
}

void MoveScriptTable::add_script(std::string name, std::function<void()> move_script) {
	script_map[name] = scripts.size();
	MoveScript script(name, move_script);
	scripts.push_back(script);
}

MoveScript MoveScriptTable::get_script(std::string script_name) {
	std::unordered_map<std::string, int>::const_iterator iterator = script_map.find(script_name);
	if (iterator == script_map.end()) {
		std::cout << "Invalid Script: " << script_name << std::endl;
		return scripts[script_map["default"]];
	}
	return scripts[script_map[script_name]];
}

void MoveScriptTable::wipe_scripts() {
	scripts.clear();
}