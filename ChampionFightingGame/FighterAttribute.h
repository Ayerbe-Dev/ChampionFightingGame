#pragma once

//FIGHTER CONTEXT:	The context that a Fighter's state is evaluated under: This affects which of the
//					opponent's hitboxes can hit this fighter, which actions we can transition into, etc.
//					In addition, some statuses can use different code/animations depending on the context,
//					such as air parries ignoring the attack height of a hitbox.
const int FIGHTER_CONTEXT_GROUND = 0;	//Fighter is on the ground
const int FIGHTER_CONTEXT_AIR = 1;		//Fighter is in the air
const int FIGHTER_CONTEXT_DOWN = 2;		//Fighter has been knocked down
const int FIGHTER_CONTEXT_MAX = 3;

//STATUS GROUP:	Groups of statuses which share common properties. A status can be part of multiple groups,
//				and there are macros for those.

const int STATUS_GROUP_NONE = 0;				/*No specific behavior.*/
const int STATUS_GROUP_NON_ACTION = 1;			/*Statuses in this group are not considered actions, so
												  they have no endlag and cannot cancel into themselves.*/
const int STATUS_GROUP_HITSTUN = 2;				/*Statuses in this group interact with the opponent's 
												  combo counter, damage scaling, etc.*/
const int STATUS_GROUP_NON_END_WITH_ANIM = 4;	/*Statuses in this group do not become actionable when
												  their animation ends.*/
const int STATUS_GROUP_NONSTANDARD_HITSTUN = 6; /*Macro for Launch, Float, Crumple and Knockdown*/
const int STATUS_GROUP_POST_PROCESS = 8;		/*Statuses in this group are processed after the opponent
												  has finished processing.*/
const int STATUS_GROUP_GRABBED = 14;			/*Macro for Grabbed*/

//DASH CANCEL KIND: Every character has different actions they can perform out of their dash. Rather
//					than make it fully character specific, we use different groups of cancel behaviors
//					and have each character fall into one of these groups.
const int DASH_CANCEL_KIND_NONE = 0;					/*Cannot be canceled*/
const int DASH_CANCEL_KIND_WHIFF_PUNISH_OFFENSIVE = 1;	/*Dash can cancel into Backdash, Backdash has 1
														  followup attack it can go into.*/
const int DASH_CANCEL_KIND_WHIFF_PUNISH_DEFENSIVE = 2;	/*Dash cannot be canceled, Backdash has 3
														  followup attacks it can go into.*/
const int DASH_CANCEL_KIND_INFINITE = 3;				/*Dash and Backdash can both cancel into 
														  eachother.*/
const int DASH_CANCEL_KIND_MAX = 4;

//JUMP KIND: The direction you're currently jumping in.
const int JUMP_KIND_F = 0;
const int JUMP_KIND_B = 1;
const int JUMP_KIND_N = 2;
const int JUMP_KIND_MAX = 3;

//HITSTUN HEIGHT:	The height that determines what hitstun animation you'll be placed into if you get
//					hit out of the current animation. Notably, your hitstun height is not affected by 
//					your stick direction while you aren't actionable.
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
const int WAKEUP_TYPE_FORWARD = 1;
const int WAKEUP_TYPE_BACK = 2;
const int WAKEUP_TYPE_SLOW = 3;
const int WAKEUP_TYPE_MAX = 4;

const int COUNTERHIT_VAL_NONE = 0;
const int COUNTERHIT_VAL_COUNTER = 1;
const int COUNTERHIT_VAL_PUNISH = 2;
const int COUNTERHIT_VAL_CRIT_STATUS = 3;