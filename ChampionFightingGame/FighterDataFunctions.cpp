#include "Fighter.h"
#include "Projectile.h"
#include "utils.h"

void Fighter::set_int(int target, int val) {
	object_int[target] = val;
}

void Fighter::inc_int(int target) {
	if (object_manager->is_allow_realtime_process(this)) {
		object_int[target]++;
	}
}

void Fighter::dec_int(int target) {
	if (object_manager->is_allow_realtime_process(this)) {
		object_int[target]--;
	}
}

void Fighter::set_float(int target, float val) {
	object_float[target] = val;
}

void Fighter::set_flag(int target, bool val) {
	object_flag[target] = val;
}

void Fighter::set_string(int target, std::string val) {
	object_string[target] = val;
}

bool Fighter::can_spend_ex(float ex) {
	return object_float[FIGHTER_FLOAT_EX_METER] >= ex && !object_flag[FIGHTER_FLAG_LOCK_EX];
}

void Fighter::spend_ex(float ex) {
	if (object_flag[FIGHTER_FLAG_LOCK_EX] || ex == 0.0f) return;
	object_float[FIGHTER_FLOAT_EX_METER] -= ex;
	object_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER] = 60;
}

void Fighter::gain_ex(float ex) {
	if (object_flag[FIGHTER_FLAG_LOCK_EX]) return;
	float max_ex = get_global_param_float(PARAM_FIGHTER, "ex_meter_size");
	float ex_diff = max_ex - object_float[FIGHTER_FLOAT_EX_METER];
	if (ex > ex_diff) {
		object_float[FIGHTER_FLOAT_SUPER_METER] = clampf(
			object_float[FIGHTER_FLOAT_SUPER_METER], 
			object_float[FIGHTER_FLOAT_SUPER_METER] + (ex - ex_diff), 
			get_global_param_float(PARAM_FIGHTER, "super_meter_size")
		);
		object_float[FIGHTER_FLOAT_EX_METER] = max_ex;
		return;
	}
	object_float[FIGHTER_FLOAT_EX_METER] = clampf(
		object_float[FIGHTER_FLOAT_EX_METER],
		object_float[FIGHTER_FLOAT_EX_METER] + ex,
		max_ex
	);
}