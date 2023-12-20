#pragma once

enum TimerSetting {
	TIMER_SETTING_NORMAL,
	TIMER_SETTING_TRAINING,
	TIMER_SETTING_NONE,

	TIMER_SETTING_MAX
};

const int BATTLE_STATE_PRE_INTRO = 0;
const int BATTLE_STATE_INTRO = 1;
const int BATTLE_STATE_ROUND_START = 2;
const int BATTLE_STATE_BATTLE = 3;
const int BATTLE_STATE_KO = 4;
const int BATTLE_STATE_OUTRO = 5;

const int ROUND_WIN_DOUBLE_KO = 2;
const int ROUND_WIN_P2 = 1;
const int ROUND_WIN_P1 = 0;
const int ROUND_WIN_TIMEOUT = -1;

const int ROUND_ICON_NONE = 0;
const int ROUND_ICON_KO = 1;
const int ROUND_ICON_PERFECT = 2;
const int ROUND_ICON_EX_SUPER = 3;
const int ROUND_ICON_CHAMPION_SUPER = 4;
const int ROUND_ICON_TIMEOUT = 5;
const int ROUND_ICON_DOUBLE = 6;