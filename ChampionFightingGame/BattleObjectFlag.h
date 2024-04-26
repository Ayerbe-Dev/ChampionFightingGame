#pragma once
#include "CompileTimeCounter.h"
MAKE_COUNTER(battle_object_flag, 0);

const int BATTLE_OBJECT_FLAG_MAX = battle_object_flag.count();