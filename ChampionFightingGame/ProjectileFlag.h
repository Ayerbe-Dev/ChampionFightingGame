#pragma once
#include "BattleObjectFlag.h"
MAKE_COUNTER(projectile_flag, BATTLE_OBJECT_FLAG_MAX);

const int PROJECTILE_FLAG_ATTACK_HIT = projectile_flag.count();
const int PROJECTILE_FLAG_DESPAWN_ON_OOB = projectile_flag.count();
const int PROJECTILE_FLAG_MAX = projectile_flag.count();