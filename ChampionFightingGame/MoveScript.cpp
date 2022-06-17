#include "MoveScript.h"

MoveScript::MoveScript() {}

MoveScript::MoveScript(std::string name, std::function<void()> move_script) {
	this->name = name;
	this->move_script = move_script;
}

void MoveScript::activate() {
	while (!frames.empty()) {
		frames.pop();
	}
	move_script();
}

void MoveScript::execute(float frame) {
	if (frames.empty()) {
		return;
	}
	if (frames.front().frame == frame) {
		frames.front().execute();
		frames.pop();
	}
}

ScriptFrame::ScriptFrame() {
	frame = -1;
}

ScriptFrame::ScriptFrame(float frame) {
	this->frame = frame;
}

void ScriptFrame::execute() {
	while (!function_calls.empty()) {
		function_calls.front()(function_args.front());
		function_args.front().destroy();
		function_calls.pop();
		function_args.pop();
	}
}

ScriptArg::ScriptArg() {
	this->num_args = 0;
	this->va = std::va_list();
}

ScriptArg::ScriptArg(int num_args, std::va_list va) {
	this->num_args = num_args;
	this->va = va;
}

void ScriptArg::destroy() {
	va_end(va);
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
		std::cout << "Invalid Script: " << script_name << "\n";
		return scripts[script_map["default"]];
	}
	return scripts[script_map[script_name]];
}

void MoveScriptTable::wipe_scripts() {
	scripts.clear();
	script_map.clear();
}