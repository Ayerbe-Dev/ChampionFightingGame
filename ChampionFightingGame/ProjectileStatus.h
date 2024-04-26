#pragma once
#include "BattleObjectStatus.h"
MAKE_COUNTER(projectile_status, BATTLE_OBJECT_STATUS_MAX);

const int PROJECTILE_STATUS_MOVE = projectile_status.count();
const int PROJECTILE_STATUS_MAX = projectile_status.count();