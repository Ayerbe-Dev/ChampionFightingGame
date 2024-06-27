#pragma once
#include "FighterStatus.h"
MAKE_COUNTER(eric_status, FIGHTER_STATUS_MAX);

const int CHARA_ERIC_STATUS_SPECIAL_FIREBALL_START = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_FIREBALL_PUNCH = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_FIREBALL_KICK = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_SLIDE = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_SLIDE_FOLLOWUP = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED_START = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_SLIDE_ENHANCED = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_START = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_UPPERCUT = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_FALL = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_UPPERCUT_LAND = eric_status.count();
const int CHARA_ERIC_STATUS_SPECIAL_INSTALL = eric_status.count();
const int CHARA_ERIC_STATUS_CHAMPION_SUPER_START = eric_status.count();
const int CHARA_ERIC_STATUS_CHAMPION_SUPER = eric_status.count();
const int CHARA_ERIC_STATUS_MAX = eric_status.count();