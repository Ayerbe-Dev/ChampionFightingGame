#include "BattleObject.h"

/// <summary>
/// Finds the first empty move_script slot, and assigns it a MoveScript that contains a name and a script to execute.
/// </summary>
/// <param name="name">: The anim_kind that the user should be in for this script to run.</param>
/// <param name="move_script">: The script that will run for the given anim_kind.</param>
void BattleObject::script(std::string name, std::function<void()> move_script) {
	move_script_table.add_script(name, move_script);
}

/// <summary>
/// Clears all move scripts.
/// </summary>
void BattleObject::wipe_scripts() {
	move_script_table.wipe_scripts();
}

/// <summary>
/// Sets the active move script to one corresponding to the given anim_name, or the default move script if no such script exists for the specified 
/// anim_name.
/// </summary>
/// <param name="anim_name">: The animation to find a move script for.</param>
void BattleObject::set_current_move_script(std::string anim_name) {
	active_move_script = move_script_table.get_script(anim_name);
	active_move_script.activate();
	active_move_script.execute(this, 0.0);
}

void BattleObject::execute_frame(float frame, std::function<void()> execute) {
	active_script_frame = ScriptFrame(frame);
	active_script_condition = nullptr;
	execute();
	active_move_script.frames.push(active_script_frame);
	last_execute_frame = frame;
}

void BattleObject::execute_wait(float frames, std::function<void()> execute) {
	active_script_frame = ScriptFrame(frames + last_execute_frame);
	active_script_condition = nullptr;
	execute();
	active_move_script.frames.push(active_script_frame);
	last_execute_frame += frames;
}

void BattleObject::push_condition(std::function<bool()> condition) {
	active_script_frame.conditions.push(condition);
	active_script_condition = &active_script_frame.conditions.back();
}