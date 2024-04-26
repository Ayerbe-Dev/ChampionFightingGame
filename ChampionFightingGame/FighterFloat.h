#pragma once
#include "BattleObjectFloat.h"
MAKE_COUNTER(fighter_float, BATTLE_OBJECT_FLOAT_MAX);

const int FIGHTER_FLOAT_HEALTH = fighter_float.count();
const int FIGHTER_FLOAT_PARTIAL_HEALTH = fighter_float.count();
const int FIGHTER_FLOAT_EX_METER = fighter_float.count();
const int FIGHTER_FLOAT_SUPER_METER = fighter_float.count();
const int FIGHTER_FLOAT_INIT_POS_JUMP_Y = fighter_float.count();
const int FIGHTER_FLOAT_CURRENT_GRAVITY = fighter_float.count();
const int FIGHTER_FLOAT_CURRENT_FALL_SPEED_MAX = fighter_float.count();
const int FIGHTER_FLOAT_PUSHBACK_PER_FRAME = fighter_float.count();
const int FIGHTER_FLOAT_GRAB_OFFSET_X = fighter_float.count();
const int FIGHTER_FLOAT_GRAB_OFFSET_Y = fighter_float.count();
const int FIGHTER_FLOAT_GRAB_OFFSET_Z = fighter_float.count();
const int FIGHTER_FLOAT_PREV_WORLD_RATE = fighter_float.count();
const int FIGHTER_FLOAT_DAMAGE_SCALE = fighter_float.count();
const int FIGHTER_FLOAT_PREV_DAMAGE_SCALE = fighter_float.count();
const int FIGHTER_FLOAT_DAMAGE_SCALE_UI = fighter_float.count();
const int FIGHTER_FLOAT_DAMAGE_UI_TRAINING = fighter_float.count();
const int FIGHTER_FLOAT_DAMAGE_SCALE_UI_TRAINING = fighter_float.count();
const int FIGHTER_FLOAT_COMBO_DAMAGE_UI_TRAINING = fighter_float.count();
const int FIGHTER_FLOAT_MAX = fighter_float.count();
