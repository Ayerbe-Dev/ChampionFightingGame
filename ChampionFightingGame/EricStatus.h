#pragma once
#include "FighterStatus.h"
MAKE_COUNTER(eric_status, FIGHTER_STATUS_MAX);

const int CHARA_ERIC_STATUS_SPECIAL_FIREBALL = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_UPPERCUT = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL = eric_status.count();
const int CHARA_ERIC_STATUS_MAX = eric_status.count();