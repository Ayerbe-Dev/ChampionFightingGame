#pragma once
#include "BattleObjectInt.h"
MAKE_COUNTER(projectile_int, BATTLE_OBJECT_INT_MAX);

const int PROJECTILE_INT_HEALTH = projectile_int.count();
const int PROJECTILE_INT_ACTIVE_TIME = projectile_int.count();
const int PROJECTILE_INT_ELAPSED_FRAMES = projectile_int.count();
const int PROJECTILE_INT_ATTACK_LEVEL = projectile_int.count();
const int PROJECTILE_INT_OWNER_ENDLAG = projectile_int.count();
const int PROJECTILE_INT_COUNTER_ATTACKER_STATUS = projectile_int.count();
const int PROJECTILE_INT_COUNTER_DEFENDER_STATUS = projectile_int.count();
const int PROJECTILE_INT_MAX = projectile_int.count();