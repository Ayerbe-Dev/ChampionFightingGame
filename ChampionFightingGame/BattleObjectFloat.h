#pragma once
#include "CompileTimeCounter.h"
MAKE_COUNTER(battle_object_float, 0);

const int BATTLE_OBJECT_FLOAT_PRE_HITLAG_FRAME = battle_object_float.count();
const int BATTLE_OBJECT_FLOAT_X_SPEED = battle_object_float.count();
const int BATTLE_OBJECT_FLOAT_Y_SPEED = battle_object_float.count();
const int BATTLE_OBJECT_FLOAT_MAX = battle_object_float.count();