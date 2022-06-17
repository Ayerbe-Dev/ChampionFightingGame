#include "BattleObject.h"

void BattleObject::SET_RATE(ScriptArg args) {
	UNWRAP(rate, float);
	set_rate(rate);
}

void BattleObject::SET_FRAME(ScriptArg args) {
	UNWRAP(frame, float);
	set_rate(frame);
}