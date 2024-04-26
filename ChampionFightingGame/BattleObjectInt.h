#pragma once
#include "CompileTimeCounter.h"
MAKE_COUNTER(battle_object_int, 0);

const int BATTLE_OBJECT_INT_HITLAG_FRAMES = battle_object_int.count();
const int BATTLE_OBJECT_INT_INIT_HITLAG_FRAMES = battle_object_int.count();
const int BATTLE_OBJECT_INT_MAX = battle_object_int.count();