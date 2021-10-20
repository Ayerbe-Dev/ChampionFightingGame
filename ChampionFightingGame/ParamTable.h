using namespace std;
#include <iostream>
#include <sstream>
#include "utils.h"
#pragma once

struct StatsTable {
	float walk_f_speed;
	float walk_b_speed;
	int dash_f_accel_frame;
	float dash_f_speed;
	int dash_f_maintain_speed_frame;
	int dash_b_accel_frame;
	float dash_b_speed;
	int dash_b_maintain_speed_frame;
	int dash_cancel_kind;
	int dash_f_cancel_frame;
	int dash_b_cancel_frame;
	float jump_y_init_speed;
	float jump_y_init_speed_s;
	float jump_x_speed;
	float gravity;
	float max_fall_speed;
	int empty_landing_lag;
	int lp_landing_lag;
	int mp_landing_lag;
	int hp_landing_lag;
	int lk_landing_lag;
	int mk_landing_lag;
	int hk_landing_lag;
	int health;
	bool has_airdash;
	float meter_gain_on_parry;
};