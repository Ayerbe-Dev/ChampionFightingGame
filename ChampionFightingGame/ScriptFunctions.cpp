#include "BattleObject.h"

/// <summary>
/// Checks if the user has reached the correct frame for the following block to run, and makes sure that the block has not already run.
/// </summary>
/// <param name="frame">: The frame the user must have reached in order to execute the block.</param>
/// <returns>Whether or not the contents of the block may be executed.</returns>
bool BattleObject::is_excute_frame(float frame) {
	bool ret = false;

	return ret;
}

/// <summary>
/// Checks if the user has waited enough frames for the following block to run, and makes sure that the block has not already run.
/// </summary>
/// <param name="frames">: How many frames it must have been since the last is_excute check in order to execute the block.</param>
/// <returns>Whether or not the contents of the block may be executed.</returns>
bool BattleObject::is_excute_wait(float frames) {
	bool ret = false;

	return ret;
}

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

void BattleObject::push_function(void (BattleObject::* function)(ScriptArg), int num_args, ...) {
	std::va_list va_list;
	va_start(va_list, num_args);
	ScriptArg args(num_args, va_list);
	push_function(function, args);
}

void BattleObject::push_function(void (BattleObject::* function)(ScriptArg), ScriptArg args) {
	active_script_frame.function_calls.push(std::bind(function, this, std::placeholders::_1));
	active_script_frame.function_args.push(args);
}