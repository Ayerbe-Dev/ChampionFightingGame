#include "Fighter.h"

void Fighter::push_function(void (Fighter::* function)(ScriptArg), int num_args, ...) {
	std::va_list va_list;
	va_start(va_list, num_args);
	ScriptArg args(num_args, va_list);
	BattleObject::push_function((void (BattleObject::*)(ScriptArg))(function), args);
}