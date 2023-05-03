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
	while ((!frames.empty()) && frames.front().frame <= frame) {
		frames.front().execute(object);
		if (frames.empty()) {
			//If a script has a call that changes the active script, such as the call to 
			//REENTER_LAST_ANIM in hitstun parry scripts, then the script that requested the change
			//will no longer be active. Since we don't store the previous move script anywhere, nor
			//do we have a reason to, it's possible for the previous script to become completely
			//deallocated by the time it finishes executing. This won't cause problems on its own, but
			//trying to pop the frames queue will cause a crash since the program thinks it's already
			//empty.
			return;
		}
		frames.pop();
	}
}

bool MoveScript::has_function(float frame, void(BattleObject::* func)(ScriptArg), ScriptArg* args_ret) {
	std::queue<ScriptFrame> temp_frames = frames;
	while ((!temp_frames.empty()) && temp_frames.front().frame <= frame) {
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

bool MoveScript::has_function(void(BattleObject::* func)(ScriptArg), ScriptArg* args_ret) {
	std::queue<ScriptFrame> temp_frames = frames;
	while (!temp_frames.empty()) {
		ScriptFrame temp_sf = temp_frames.front();
		while ((!temp_sf.function_calls.empty()) && (temp_sf.function_calls.front() != func)) {
			temp_sf.function_calls.pop();
			temp_sf.function_args.pop();
		}
		if (!temp_sf.function_calls.empty()) {
			if (args_ret != nullptr) {
				*args_ret = temp_sf.function_args.front();
			}
			return true;
		}
		temp_frames.pop();
	}
	return false;
}

bool MoveScript::has_function(float frame, void(Fighter::* func)(ScriptArg), ScriptArg* args_ret) {
	return has_function(frame, (void (BattleObject::*)(ScriptArg))func, args_ret);
}

bool MoveScript::has_function(void(Fighter::* func)(ScriptArg), ScriptArg* args_ret) {
	return has_function((void (BattleObject::*)(ScriptArg))func, args_ret);
}

bool MoveScript::has_function(float frame, void(Projectile::* func)(ScriptArg), ScriptArg* args_ret) {
	return has_function(frame, (void (BattleObject::*)(ScriptArg))func, args_ret);
}

bool MoveScript::has_function(void(Projectile::* func)(ScriptArg), ScriptArg* args_ret) {
	return has_function((void (BattleObject::*)(ScriptArg))func, args_ret);
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
		if (function_calls.empty() || function_args.empty()) {
			//Same rationale as MoveScript::execute() having a check like this: It's possible for this
			//object to silently de-allocate itself, and while it can still safely return, we need to
			//make sure it doesn't pop something that the program already destroyed.
			break;
		}
		function_calls.pop();
		function_args.pop();
	}
	while (!conditions.empty()) {
		conditions.front().execute(object);
		if (conditions.empty()) {
			break;
		}
		conditions.pop();
	}
}

ScriptCondition::ScriptCondition() {
	this->condition = []() {return true; };
}

ScriptCondition::ScriptCondition(std::function<bool()> condition) {
	this->condition = condition;
}

void ScriptCondition::execute(BattleObject* object) {
	if (condition()) {
		while (!true_calls.empty()) {
			std::mem_fn(true_calls.front())(object, true_args.front());
			if (true_calls.empty() || true_args.empty()) {
				return;
			}
			true_calls.pop();
			true_args.pop();
		}
	}
	else {
		while (!false_calls.empty()) {
			std::mem_fn(false_calls.front())(object, false_args.front());
			if (false_calls.empty() || false_args.empty()) {
				return;
			}
			false_calls.pop();
			false_args.pop();
		}
	}
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