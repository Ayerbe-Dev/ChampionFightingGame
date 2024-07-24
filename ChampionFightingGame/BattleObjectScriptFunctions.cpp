#include "BattleObject.h"

void BattleObject::change_script(std::string script_name) {
	move_script = move_script_table.get_script(script_name);
	move_script.activate();
	execute_active_script();
}

void BattleObject::execute_active_script() {
	std::string active_script_name = move_script.name;
	while (move_script.has_next_frame(frame)) {
		ScriptFrame frame = move_script.get_next_frame();
		while (!frame.script_funcs.empty()) {
			ScriptFunc<BattleObject> func = frame.script_funcs.front();
			if (func.condition_kind == SCRIPT_CONDITION_KIND_NONE
			||	func.condition_kind == move_script.conditions[func.condition_name]()) {
				func.execute(this);
			}

			frame.script_funcs.pop();
			if (move_script.name != active_script_name) return;
		}
	}
}

void BattleObject::clear_scripts() {
	move_script_table.clear_scripts();
}

void BattleObject::script(std::string name, std::function<void()> move_script) {
	move_script_table.add_script(name, move_script);
}

void BattleObject::execute_frame(float frame, std::function<void()> execute) {
	move_script_table.execute_frame(frame, execute);
}

void BattleObject::execute_wait(float frames, std::function<void()> execute) {
	move_script_table.execute_wait(frames, execute);
}

void BattleObject::push_condition(std::string condition_name, std::function<bool()> condition) {
	move_script_table.push_condition(condition_name, condition);
}