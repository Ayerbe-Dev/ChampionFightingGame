using namespace std;
#include <iostream>
#include <sstream>
#include "utils.h"
#pragma once

struct StatsTable {
	f32 walk_f_speed;
	f32 walk_b_speed;
	i32 dash_f_accel_frame;
	f32 dash_f_speed;
	i32 dash_f_maintain_speed_frame;
	i32 dash_b_accel_frame;
	f32 dash_b_speed;
	i32 dash_b_maintain_speed_frame;
	i32 dash_cancel_kind;
	i32 dash_f_cancel_frame;
	i32 dash_b_cancel_frame;
	f32 jump_y_init_speed;
	f32 jump_y_init_speed_s;
	f32 jump_x_speed;
	f32 gravity;
	f32 max_fall_speed;
	i32 empty_landing_lag;
	i32 lp_landing_lag;
	i32 mp_landing_lag;
	i32 hp_landing_lag;
	i32 lk_landing_lag;
	i32 mk_landing_lag;
	i32 hk_landing_lag;
	i32 health;
	bool has_airdash;
	f32 meter_gain_on_parry;
};