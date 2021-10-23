#include "ObjectInstance.h"

void ObjectInstance::set_current_move_script(string anim_name) {}

bool ObjectInstance::is_excute_frame(int excute_count, int frame)
{
	if (this->frame >= frame)
	{ //If we've reached the statement in question
		if (this->excute_count >= excute_count)
		{ //If we've already executed the statement
			return false;
		}
		else
		{ //If we are at the correct frame and haven't executed the statement yet
			last_excute_frame = frame;
			this->excute_count = excute_count;
			return true;
		}
	}
	else
	{ //If we still haven't reached the correct frame
		last_excute_frame = frame;
		return false;
	}
}

bool ObjectInstance::is_excute_wait(int excute_count, int frames) {
	if (frame >= last_excute_frame + frames) { //If it's been enough time since the last non-executed statement
		if (this->excute_count >= excute_count)
		{ //If we already executed the statement
			return false;
		}
		else
		{ //Success
			last_excute_frame = frame;
			this->excute_count = excute_count;
			return true;
		}
	}
	else
	{ //Still waiting
		return false;
	}
}