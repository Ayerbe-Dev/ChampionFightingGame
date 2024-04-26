#pragma once
#include "CompileTimeCounter.h"
MAKE_COUNTER(battle_object_string, 0);

const int BATTLE_OBJECT_STRING_MAX = battle_object_string.count();