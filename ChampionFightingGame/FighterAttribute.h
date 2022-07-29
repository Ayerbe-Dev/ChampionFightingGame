#pragma once

const int FIGHTER_SITUATION_GROUND = 0;
const int FIGHTER_SITUATION_AIR = 1;
const int FIGHTER_SITUATION_DOWN = 2;
const int FIGHTER_SITUATION_MAX = 3;

const int STATUS_GROUP_NORMAL = 0;
const int STATUS_GROUP_HITSTUN = 1;
const int STATUS_GROUP_CROUCH = 2;
const int STATUS_GROUP_ATTACK = 3;
const int STATUS_GROUP_MAX = 4;

const int DASH_CANCEL_KIND_FOLLOWUP = 0;
const int DASH_CANCEL_KIND_INDEFINITE = 1;
const int DASH_CANCEL_KIND_MAX = 2;

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
const int ATTACK_KIND_SPECIAL_CHARGE_DOWN = 18;
const int ATTACK_KIND_SPECIAL_CHARGE_BACK = 19;
const int ATTACK_KIND_SUPER_236236 = 20;
const int ATTACK_KIND_SUPER_214214 = 21;
const int ATTACK_KIND_NONE = 22;
const int ATTACK_KIND_MAX = 23;

const int SPECIAL_LEVEL_L = 0;
const int SPECIAL_LEVEL_M = 1;
const int SPECIAL_LEVEL_H = 2;
const int SPECIAL_LEVEL_EX = 3;
const int SPECIAL_LEVEL_MAX = 4;

const int SPECIAL_INPUT_NONE = 0;
const int SPECIAL_INPUT_JUST = 1;
const int SPECIAL_INPUT_NORMAL = 2;
const int SPECIAL_INPUT_MAX = 3;

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
const int CANCEL_KIND_236P = 12;
const int CANCEL_KIND_236K = 13;
const int CANCEL_KIND_214P = 14;
const int CANCEL_KIND_214K = 15;
const int CANCEL_KIND_623P = 16;
const int CANCEL_KIND_623K = 17;
const int CANCEL_KIND_41236P = 18;
const int CANCEL_KIND_41236K = 19;
const int CANCEL_KIND_63214P = 20;
const int CANCEL_KIND_63214K = 21;
const int CANCEL_KIND_CHARGE_DOWN_P = 22;
const int CANCEL_KIND_CHARGE_DOWN_K = 23;
const int CANCEL_KIND_CHARGE_BACK_P = 24;
const int CANCEL_KIND_CHARGE_BACK_K = 25;
const int CANCEL_KIND_236236P = 26;
const int CANCEL_KIND_236236K = 27;
const int CANCEL_KIND_214214P = 28;
const int CANCEL_KIND_214214K = 29;
const int CANCEL_KIND_DASH_F = 30;
const int CANCEL_KIND_DASH_B = 31;
const int CANCEL_KIND_MAX = 32;

const int CANCEL_CAT_HIT = 0;
const int CANCEL_CAT_BLOCK = 1;
const int CANCEL_CAT_WHIFF = 2;
const int CANCEL_CAT_MAX = 3;
const int CANCEL_CAT_FILLER = 4; //Used as part of a stupid safety check, dw about it
const int CANCEL_CAT_CONTACT = 5;
const int CANCEL_CAT_ALL = 6;