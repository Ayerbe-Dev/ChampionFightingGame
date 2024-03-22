#include "BattleObject.h"

/// <summary>
/// Sets the active move script to one corresponding to the given anim_name, or the default move script if no such script exists for the specified 
/// anim_name.
/// </summary>
/// <param name="anim_name">: The animation to find a move script for.</param>
void BattleObject::set_active_script(std::string anim_name) {
	//TODO: Add something to make sure the current move script isn't actively being executed while we
	//want to change it. Probably shouldn't be a mutex either bc that would potentially allow for
	//a move script to be changed either before or after the current one ran, so probably better to
	//wait until the move script has been executed for the current frame?

	//Note: This problem currently seems to only affect cases where one player changes the other player's
	//move script as part of theirs, namely throws. It MIGHT be the case that the "grabbed" anim doesn't
	//need a script at all, which would potentially fix the issue as well

	active_move_script = move_script_table.get_script(anim_name);
	active_move_script.activate();
	execute_active_script();
}

/// <summary>
/// Executes the active move script. If the active move script changes during this function, it
/// immediately returns.
/// </summary>
void BattleObject::execute_active_script() {
	std::string active_script_name = active_move_script.name;
	while (active_move_script.has_next_frame(frame)) {
		ScriptFrame frame = active_move_script.get_next_frame();
		while (!frame.script_funcs.empty()) {
			ScriptFunc<BattleObject> func = frame.script_funcs.front();
			if (func.condition_kind == SCRIPT_CONDITION_KIND_NONE
			||	func.condition_kind == active_move_script.conditions[func.condition_name]()) {
				func.execute(this);
			}
			frame.script_funcs.pop();
			if (active_move_script.name != active_script_name) return;
		}
	}
}

/// <summary>
/// Clears all move scripts.
/// </summary>
void BattleObject::clear_scripts() {
	move_script_table.clear_scripts();
}

/// <summary>
/// Finds the first empty move_script slot, and assigns it a MoveScript that contains a name and a script to execute.
/// </summary>
/// <param name="name">: The anim_kind that the user should be in for this script to run.</param>
/// <param name="move_script">: The script that will run for the given anim_kind.</param>
void BattleObject::script(std::string name, std::function<void()> move_script) {
	move_script_table.add_script(name, move_script);
}

void BattleObject::execute_frame(float frame, std::function<void()> execute) {
	active_script_frame = ScriptFrame(frame);
	execute();
	active_move_script.frames.push(active_script_frame);
	last_execute_frame = frame;
}

void BattleObject::execute_wait(float frames, std::function<void()> execute) {
	active_script_frame = ScriptFrame(frames + last_execute_frame);
	execute();
	active_move_script.frames.push(active_script_frame);
	last_execute_frame += frames;
}

void BattleObject::push_condition(std::string condition_name, std::function<bool()> condition) {
	active_move_script.conditions[condition_name] = condition;
}