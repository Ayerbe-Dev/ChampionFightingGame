#pragma once
#include "CompileTimeCounter.h"
MAKE_COUNTER(battle_object_status, 0);

const int BATTLE_OBJECT_STATUS_NONE = battle_object_status.count();
const int BATTLE_OBJECT_STATUS_MAX = battle_object_status.count();