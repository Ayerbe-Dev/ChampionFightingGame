#pragma once

const int FIGHTER_SITUATION_GROUND = 0;
const int FIGHTER_SITUATION_AIR = 1;
const int FIGHTER_SITUATION_DOWN = 2;
const int FIGHTER_SITUATION_MAX = 3;

const int STATUS_GROUP_NORMAL = 0;
const int STATUS_GROUP_HITSTUN = 1;
const int STATUS_GROUP_CROUCH = 2;
const int STATUS_GROUP_ATTACK = 3;
const int STATUS_GROUP_LANDING = 4;
const int STATUS_GROUP_MAX = 5;

const int DASH_CANCEL_KIND_NONE = 0;
const int DASH_CANCEL_KIND_WHIFF_PUNISH_OFFENSIVE = 1;
const int DASH_CANCEL_KIND_WHIFF_PUNISH_DEFENSIVE = 2;
const int DASH_CANCEL_KIND_INFINITE = 3;
const int DASH_CANCEL_KIND_MAX = 4;

const int JUMP_KIND_F = 0;
const int JUMP_KIND_B = 1;
const int JUMP_KIND_N = 2;
const int JUMP_KIND_MAX = 3;

const int HITSTUN_HEIGHT_STAND = 0;
const int HITSTUN_HEIGHT_CROUCH = 1;
const int HITSTUN_HEIGHT_NONE = 2;
const int HITSTUN_HEIGHT_MAX = 3;

const int PARRY_HEIGHT_HIGH = 0;
const int PARRY_HEIGHT_MID = 1;
const int PARRY_HEIGHT_LOW = 2;
const int PARRY_HEIGHT_ALL = 3;
const int PARRY_HEIGHT_MAX = 4;

const int ATTACK_KIND_LP = 0;
const int ATTACK_KIND_MP = 1;
const int ATTACK_KIND_HP = 2;
const int ATTACK_KIND_LK = 3;
const int ATTACK_KIND_MK = 4;
const int ATTACK_KIND_HK = 5;
const int ATTACK_KIND_CLP = 6;
const int ATTACK_KIND_CMP = 7;
const int ATTACK_KIND_CHP = 8;
const int ATTACK_KIND_CLK = 9;
const int ATTACK_KIND_CMK = 10;
const int ATTACK_KIND_CHK = 11;
const int ATTACK_KIND_OTHER = 12;
const int ATTACK_KIND_SPECIAL_236 = 13;
const int ATTACK_KIND_SPECIAL_214 = 14;
const int ATTACK_KIND_SPECIAL_623 = 15;
const int ATTACK_KIND_SPECIAL_41236 = 16; 
const int ATTACK_KIND_SPECIAL_63214 = 17;
const int ATTACK_KIND_SPECIAL_632 = 18;
const int ATTACK_KIND_SPECIAL_22 = 19;
const int ATTACK_KIND_SPECIAL_28 = 20;
const int ATTACK_KIND_SPECIAL_46 = 21;
const int ATTACK_KIND_SUPER_236236 = 22;
const int ATTACK_KIND_SUPER_214214 = 23;
const int ATTACK_KIND_SUPER_4646 = 24;
const int ATTACK_KIND_NONE = 25;
const int ATTACK_KIND_MAX = 26;

const int SPECIAL_LEVEL_L = 0;
const int SPECIAL_LEVEL_M = 1;
const int SPECIAL_LEVEL_H = 2;
const int SPECIAL_LEVEL_EX = 3;
const int SPECIAL_LEVEL_MAX = 4;

const int WAKEUP_TYPE_DEFAULT = 0;
const int WAKEUP_TYPE_FAST = 1;
const int WAKEUP_TYPE_BACK = 2;
const int WAKEUP_TYPE_MAX = 3;

const int CANCEL_KIND_LP = 0;
const int CANCEL_KIND_MP = 1;
const int CANCEL_KIND_HP = 2;
const int CANCEL_KIND_LK = 3;
const int CANCEL_KIND_MK = 4;
const int CANCEL_KIND_HK = 5;
const int CANCEL_KIND_CLP = 6;
const int CANCEL_KIND_CMP = 7;
const int CANCEL_KIND_CHP = 8;
const int CANCEL_KIND_CLK = 9;
const int CANCEL_KIND_CMK = 10;
const int CANCEL_KIND_CHK = 11;
const int CANCEL_KIND_OTHER = 12;
const int CANCEL_KIND_236P = 13;
const int CANCEL_KIND_236K = 14;
const int CANCEL_KIND_214P = 15;
const int CANCEL_KIND_214K = 16;
const int CANCEL_KIND_623P = 17;
const int CANCEL_KIND_623K = 18;
const int CANCEL_KIND_41236P = 19;
const int CANCEL_KIND_41236K = 20;
const int CANCEL_KIND_63214P = 21;
const int CANCEL_KIND_63214K = 22;
const int CANCEL_KIND_632P = 23;
const int CANCEL_KIND_632K = 24;
const int CANCEL_KIND_22P = 25;
const int CANCEL_KIND_22K = 26;
const int CANCEL_KIND_28P = 27;
const int CANCEL_KIND_28K = 28;
const int CANCEL_KIND_46P = 29;
const int CANCEL_KIND_46K = 30;
const int CANCEL_KIND_236236P = 31;
const int CANCEL_KIND_236236K = 32;
const int CANCEL_KIND_214214P = 33;
const int CANCEL_KIND_214214K = 34;
const int CANCEL_KIND_4646P = 35;
const int CANCEL_KIND_4646K = 36;
const int CANCEL_KIND_DASH_F = 37;
const int CANCEL_KIND_DASH_B = 38;
const int CANCEL_KIND_JUMP = 39;
const int CANCEL_KIND_GRAB = 40;
const int CANCEL_KIND_PARRY_START = 41;
const int CANCEL_KIND_ADVANCE = 42;
const int CANCEL_KIND_MAX = 43;

const int CANCEL_CAT_HIT = 0;
const int CANCEL_CAT_BLOCK = 1;
const int CANCEL_CAT_WHIFF = 2;
const int CANCEL_CAT_MAX = 3;
const int CANCEL_CAT_FILLER = 4; //Used as part of a stupid safety check, dw about it
const int CANCEL_CAT_CONTACT = 5;
const int CANCEL_CAT_ALL = 6;

const int UI_TEXT_TYPE_NONE = 0;
const int UI_TEXT_TYPE_COUNTER = 1;
const int UI_TEXT_TYPE_COUNTER_PUNISH = 2;
const int UI_TEXT_TYPE_COUNTER_HITSTUN_PARRY = 3;
const int UI_TEXT_TYPE_COUNTER_JUMP = 4;
const int UI_TEXT_TYPE_ARMOR_BREAK = 5;
const int UI_TEXT_TYPE_REVERSAL = 6;

const int COUNTERHIT_VAL_NONE = 0;
const int COUNTERHIT_VAL_COUNTER = 1;
const int COUNTERHIT_VAL_PUNISH = 2;
const int COUNTERHIT_VAL_UNIQUE_STATUS = 3;