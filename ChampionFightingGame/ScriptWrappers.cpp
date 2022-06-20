#include "BattleObject.h"

void BattleObject::SET_RATE(ScriptArg args) {
	UNWRAP(rate, double);
	set_rate(rate);
}

void BattleObject::SET_FRAME(ScriptArg args) {
	UNWRAP(frame, double);
	set_rate(frame);
}

void BattleObject::NEW_BLOCKBOX(ScriptArg args) {
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	new_blockbox(anchor, offset);
}