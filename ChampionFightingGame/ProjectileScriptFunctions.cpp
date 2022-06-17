#include "Projectile.h"

void Projectile::push_function(void (Projectile::* function)(ScriptArg), int num_args, ...) {
	std::va_list va_list;
	va_start(va_list, num_args);
	ScriptArg args(num_args, va_list);
	BattleObject::push_function((void (BattleObject::*)(ScriptArg))(function), args);
}