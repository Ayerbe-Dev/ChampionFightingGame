#include "AIInput.h"

AIInput::AIInput() {

}

AIInput::AIInput(int execution, int reaction) {
	init(execution, reaction);
}

void AIInput::init(int execution, int reaction) {
	this->execution = execution;
	this->reaction = reaction;
}