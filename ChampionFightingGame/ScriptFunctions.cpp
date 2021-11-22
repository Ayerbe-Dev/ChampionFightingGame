#include "Object.h"

bool Object::is_excute_frame(int frame) {
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

bool Object::is_excute_wait(int frames) {
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

void Object::script(string name, function<void()> move_script) {
	for (int i = 0; i < MOVE_SCRIPT_MAX; i++) {
		if (move_scripts[i].id == -1) {
			move_scripts[i] = MoveScript(name, move_script, i);
			break;
		}
	}
}

void Object::set_current_move_script(string anim_name) {
	for (int i = 0; i < 256; i++) {
		if (move_scripts[i].name == anim_name) {
			move_script = move_scripts[i].move_script;
			break;
		}
		else {
			move_script = move_scripts[0].move_script;
		}
	}
}