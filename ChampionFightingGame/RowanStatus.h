#pragma once
#include "FighterStatus.h"
MAKE_COUNTER(rowan_status, FIGHTER_STATUS_MAX);

const int CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_START = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_PUNCH = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_FIREBALL_KICK = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_SLIDE = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_SLIDE_FOLLOWUP = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED_START = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_SLIDE_ENHANCED = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_START = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_FALL = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_UPPERCUT_LAND = rowan_status.count();
const int CHARA_ROWAN_STATUS_SPECIAL_INSTALL = rowan_status.count();
const int CHARA_ROWAN_STATUS_CHAMPION_SUPER_START = rowan_status.count();
const int CHARA_ROWAN_STATUS_CHAMPION_SUPER = rowan_status.count();
const int CHARA_ROWAN_STATUS_MAX = rowan_status.count();