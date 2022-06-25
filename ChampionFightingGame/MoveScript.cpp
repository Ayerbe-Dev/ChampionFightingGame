#include "MoveScript.h"
#include "BattleObject.h"

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

void MoveScript::execute(BattleObject* object, float frame) {
	if (frames.empty()) {
		return;
	}
	while ((!frames.empty()) && frames.front().frame <= frame) {
		frames.front().execute(object);
		frames.pop();
	}
}

class BattleObject;

bool MoveScript::has_function(float frame, void(BattleObject::* func)(ScriptArg), ScriptArg* args_ret) {
	std::queue<ScriptFrame> temp_frames = frames;
	while ((!temp_frames.empty()) && temp_frames.front().frame != frame) {
		temp_frames.pop();
	}
	if (temp_frames.empty()) {
		return false;
	}
	else {
		ScriptFrame temp_sf = temp_frames.front();
		while ((!temp_sf.function_calls.empty()) && (temp_sf.function_calls.front() != func)) {
			temp_sf.function_calls.pop();
			temp_sf.function_args.pop();
		}
		if (temp_sf.function_calls.empty()) {
			return false;
		}
		else {
			if (args_ret != nullptr) {
				*args_ret = temp_sf.function_args.front();
			}
			return true;
		}
	}
}

ScriptFrame::ScriptFrame() {
	frame = -1;
}

ScriptFrame::ScriptFrame(float frame) {
	this->frame = frame;
}

void ScriptFrame::execute(BattleObject* object) {
	while (!function_calls.empty()) {
		std::mem_fn(function_calls.front())(object, function_args.front());
		function_calls.pop();
		function_args.pop();
	}
}

ScriptArg::ScriptArg() {
	this->num_args = 0;
}

ScriptArg::ScriptArg(int num_args, std::queue<std::any> args) {
	this->num_args = num_args;
	this->args = args;
}

void ScriptArg::pop() {
	args.pop();
}

std::any ScriptArg::get_arg() {
	std::any ret = args.front();
	pop();
	return ret;
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