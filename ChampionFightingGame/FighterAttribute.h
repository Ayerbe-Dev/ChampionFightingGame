#pragma once

const int FIGHTER_SITUATION_GROUND = 0;
const int FIGHTER_SITUATION_AIR = 1;
const int FIGHTER_SITUATION_DOWN = 2;
const int FIGHTER_SITUATION_MAX = 3;

const int STATUS_GROUP_NONE = 0;
const int STATUS_GROUP_NON_ACTION = 1; //Statuses in this group cannot transition into themselves.
const int STATUS_GROUP_HITSTUN = 2; //Statuses in this group interact with the opponent's combo counter.
const int STATUS_GROUP_NON_END_WITH_ANIM = 4; //Statuses in this group do not become actionable when 
//their animation ends.
const int STATUS_GROUP_NONSTANDARD_HITSTUN = 6; //Launch, Crumple, Knockdown, etc.
const int STATUS_GROUP_STATUS_AFTER_OPPONENT = 8; //Statuses in this group aren't processed until after
//the opponent has finished processing.
const int STATUS_GROUP_GRABBED = 14; //Grabbed

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
const int HITSTUN_HEIGHT_MAX = 2;

const int PARRY_HEIGHT_HIGH = 0;
const int PARRY_HEIGHT_MID = 1;
const int PARRY_HEIGHT_LOW = 2;
const int PARRY_HEIGHT_ALL = 3;
const int PARRY_HEIGHT_MAX = 4;

const int SPECIAL_LEVEL_L = 0;
const int SPECIAL_LEVEL_M = 1;
const int SPECIAL_LEVEL_H = 2;
const int SPECIAL_LEVEL_EX = 3;
const int SPECIAL_LEVEL_N = 4;
const int SPECIAL_LEVEL_MAX = 5;

const int WAKEUP_TYPE_DEFAULT = 0;
const int WAKEUP_TYPE_FAST = 1;
const int WAKEUP_TYPE_BACK = 2;
const int WAKEUP_TYPE_MAX = 3;

const int UI_TEXT_TYPE_NONE = 0;
const int UI_TEXT_TYPE_COUNTER = 1;
const int UI_TEXT_TYPE_PUNISH = 2;
const int UI_TEXT_TYPE_CRITICAL = 3;
const int UI_TEXT_TYPE_ARMOR_BREAK = 4;
const int UI_TEXT_TYPE_REVERSAL = 5;

const int COUNTERHIT_VAL_NONE = 0;
const int COUNTERHIT_VAL_COUNTER = 1;
const int COUNTERHIT_VAL_PUNISH = 2;
const int COUNTERHIT_VAL_CRIT_STATUS = 3;