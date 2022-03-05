#include "BattleObject.h"

/// <summary>
/// Checks if the user has reached the correct frame for the following block to run, and makes sure that the block has not already run.
/// </summary>
/// <param name="frame">: The frame the user must have reached in order to execute the block.</param>
/// <returns>Whether or not the contents of the block may be executed.</returns>
bool BattleObject::is_excute_frame(float frame) {
	bool ret = false;
	attempted_excutes++;
	if (this->frame >= frame) {
		if (excute_count < attempted_excutes) {
			excute_count++;
			ret = true;
		}
	}

	last_excute_frame = frame;

	return ret;
}

/// <summary>
/// Checks if the user has waited enough frames for the following block to run, and makes sure that the block has not already run.
/// </summary>
/// <param name="frames">: How many frames it must have been since the last is_excute check in order to execute the block.</param>
/// <returns>Whether or not the contents of the block may be executed.</returns>
bool BattleObject::is_excute_wait(float frames) {
	bool ret = false;
	attempted_excutes++;
	if (frame >= last_excute_frame + frames) {
		if (excute_count < attempted_excutes) {
			excute_count++;
			ret = true;
		}
	}
	last_excute_frame += frames;

	return ret;
}

/// <summary>
/// Finds the first empty move_script slot, and assigns it a MoveScript that contains a name and a script to execute.
/// </summary>
/// <param name="name">: The anim_kind that the user should be in for this script to run.</param>
/// <param name="move_script">: The script that will run for the given anim_kind.</param>
void BattleObject::script(string name, function<void()> move_script) {
	for (int i = 0; i < MOVE_SCRIPT_MAX; i++) {
		if (move_scripts[i].id == -1) {
			move_scripts[i] = MoveScript(name, move_script, i);
			return;
		}
	}
}

/// <summary>
/// Clears all move scripts.
/// </summary>
void BattleObject::wipe_scripts() {
	for (int i = 0; i < MOVE_SCRIPT_MAX; i++) {
		move_scripts[i] = MoveScript();
	}
}

/// <summary>
/// Sets the active move script to one corresponding to the given anim_name, or the default move script if no such script exists for the specified 
/// anim_name.
/// </summary>
/// <param name="anim_name">: The animation to find a move script for.</param>
void BattleObject::set_current_move_script(string anim_name) {
	for (int i = 0; i < 256; i++) {
		if (move_scripts[i].name == anim_name) {
			move_script = move_scripts[i];
			return;
		}
	}
	move_script = move_scripts[0];
}