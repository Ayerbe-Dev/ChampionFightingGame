#include "MoveScript.h"
#include "BattleObject.h"

MoveScript::MoveScript() {
	name = "";
	move_script = []() {};
	table = nullptr;
}

MoveScript::MoveScript(std::string name, std::function<void()> move_script, MoveScriptTable* table) {
	this->name = name;
	this->move_script = move_script;
	this->table = table;
}

void MoveScript::activate() {
	while (!frames.empty()) {
		frames.pop();
	}
	conditions.clear();
	if (table == nullptr) return;
	table->set_activating_move_script(this);
	move_script();
}

bool MoveScript::has_next_frame(float frame) {
	return (!frames.empty()) && frames.front().frame <= frame;
}

ScriptFrame MoveScript::get_next_frame() {
	ScriptFrame ret = frames.front();
	frames.pop();
	return ret;
}

bool MoveScript::has_function(float frame, void(BattleObject::* func)(ScriptArg), ScriptArg* args_ret) {
	std::queue<ScriptFrame> temp_frames = frames;
	while ((!temp_frames.empty()) && temp_frames.front().frame != frame) {
		temp_frames.pop();
	}
	if (!temp_frames.empty()) {
		ScriptFrame temp_sf = temp_frames.front();
		while (!temp_sf.script_funcs.empty() && temp_sf.script_funcs.front().function_call != func) {
			temp_sf.script_funcs.pop();
		}
		if (!temp_sf.script_funcs.empty()) {
			if (args_ret != nullptr) {
				*args_ret = temp_sf.script_funcs.front().function_args;
			}
			return true;
		}
	}
	return false;
}

bool MoveScript::has_function(void(BattleObject::* func)(ScriptArg), ScriptArg* args_ret) {
	std::queue<ScriptFrame> temp_frames = frames;
	while (!temp_frames.empty()) {
		ScriptFrame temp_sf = temp_frames.front();
		while (!temp_sf.script_funcs.empty() && temp_sf.script_funcs.front().function_call != func) {
			temp_sf.script_funcs.pop();
		}
		if (!temp_sf.script_funcs.empty()) {
			if (args_ret != nullptr) {
				*args_ret = temp_sf.script_funcs.front().function_args;
			}
			return true;
		}
		temp_frames.pop();

	}
	return false;
}

bool MoveScript::has_function(float frame, void(Fighter::* func)(ScriptArg), ScriptArg* args_ret) {
	std::queue<ScriptFrame> temp_frames = frames;
	while ((!temp_frames.empty()) && temp_frames.front().frame != frame) {
		temp_frames.pop();
	}
	if (!temp_frames.empty()) {
		ScriptFrame temp_sf = temp_frames.front();
		while (!temp_sf.script_funcs.empty() && temp_sf.script_funcs.front().function_call != 
			(void (BattleObject::*)(ScriptArg))func) {
			temp_sf.script_funcs.pop();
		}
		if (!temp_sf.script_funcs.empty()) {
			if (args_ret != nullptr) {
				*args_ret = temp_sf.script_funcs.front().function_args;
			}
			return true;
		}
	}
	return false;
}

bool MoveScript::has_function(void(Fighter::* func)(ScriptArg), ScriptArg* args_ret) {
	std::queue<ScriptFrame> temp_frames = frames;
	while (!temp_frames.empty()) {
		ScriptFrame temp_sf = temp_frames.front();
		while (!temp_sf.script_funcs.empty() && temp_sf.script_funcs.front().function_call !=
			(void (BattleObject::*)(ScriptArg))func) {
			temp_sf.script_funcs.pop();
		}
		if (!temp_sf.script_funcs.empty()) {
			if (args_ret != nullptr) {
				*args_ret = temp_sf.script_funcs.front().function_args;
			}
			return true;
		}
		temp_frames.pop();
	}
	return false;
}

bool MoveScript::has_function(float frame, void(Projectile::* func)(ScriptArg), ScriptArg* args_ret) {
	std::queue<ScriptFrame> temp_frames = frames;
	while ((!temp_frames.empty()) && temp_frames.front().frame != frame) {
		temp_frames.pop();
	}
	if (!temp_frames.empty()) {
		ScriptFrame temp_sf = temp_frames.front();
		while (!temp_sf.script_funcs.empty() && temp_sf.script_funcs.front().function_call !=
			(void (BattleObject::*)(ScriptArg))func) {
			temp_sf.script_funcs.pop();
		}
		if (!temp_sf.script_funcs.empty()) {
			if (args_ret != nullptr) {
				*args_ret = temp_sf.script_funcs.front().function_args;
			}
			return true;
		}
	}
	return false;
}

bool MoveScript::has_function(void(Projectile::* func)(ScriptArg), ScriptArg* args_ret) {
	std::queue<ScriptFrame> temp_frames = frames;
	while (!temp_frames.empty()) {
		ScriptFrame temp_sf = temp_frames.front();
		while (!temp_sf.script_funcs.empty() && temp_sf.script_funcs.front().function_call !=
			(void (BattleObject::*)(ScriptArg))func) {
			temp_sf.script_funcs.pop();
		}
		if (!temp_sf.script_funcs.empty()) {
			if (args_ret != nullptr) {
				*args_ret = temp_sf.script_funcs.front().function_args;
			}
			return true;
		}
		temp_frames.pop();

	}
	return false;
}

ScriptFrame::ScriptFrame() {
	frame = -1;
}

ScriptFrame::ScriptFrame(float frame) {
	this->frame = frame;
}

void ScriptFrame::push_function(void(BattleObject::* func)(ScriptArg), ScriptArg args) {
	script_funcs.push(ScriptFunc<BattleObject>(func, args));
}

void ScriptFrame::push_true(std::string condition_name, void(BattleObject::* func)(ScriptArg), 
	ScriptArg args) {
	script_funcs.push(ScriptFunc<BattleObject>(condition_name, SCRIPT_CONDITION_KIND_TRUE, func, args));
}

void ScriptFrame::push_false(std::string condition_name, void(BattleObject::* func)(ScriptArg), 
	ScriptArg args) {
	script_funcs.push(ScriptFunc<BattleObject>(condition_name, SCRIPT_CONDITION_KIND_FALSE, func, args));
}

MoveScriptTable::MoveScriptTable() {
	activating_move_script = nullptr;
	add_script("default", []() {
		return;
	});
}

void MoveScriptTable::add_script(std::string name, std::function<void()> move_script) {
	script_map[name] = scripts.size();
	MoveScript script(name, move_script, this);
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

void MoveScriptTable::clear_scripts() {
	scripts.clear();
	script_map.clear();
	activating_move_script = nullptr;
}

void MoveScriptTable::set_activating_move_script(MoveScript* script) {
	this->activating_move_script = script;
}

void MoveScriptTable::execute_frame(float frame, std::function<void()> execute) {
	activating_script_frame = ScriptFrame(frame);
	execute();
	activating_move_script->frames.push(activating_script_frame);
	last_execute_frame = frame;
}

void MoveScriptTable::execute_wait(float frames, std::function<void()> execute) {
	activating_script_frame = ScriptFrame(frames + last_execute_frame);
	execute();
	activating_move_script->frames.push(activating_script_frame);
	last_execute_frame += frames;
}

void MoveScriptTable::push_condition(std::string condition_name, std::function<bool()> condition) {
	activating_move_script->conditions[condition_name] = condition;
}

void MoveScriptTable::push_function(void(BattleObject::* func)(ScriptArg), ScriptArg args) {
	activating_script_frame.push_function(func, args);
}

void MoveScriptTable::push_true(std::string condition_name, void(BattleObject::* func)(ScriptArg), ScriptArg args) {
	activating_script_frame.push_true(condition_name, func, args);
}

void MoveScriptTable::push_false(std::string condition_name, void(BattleObject::* func)(ScriptArg), ScriptArg args) {
	activating_script_frame.push_false(condition_name, func, args);
}