#include "Fighter.h"
#include "Projectile.h"
#include "utils.h"

void Fighter::set_int(int target, int val) {
	fighter_int[target] = val;
}

void Fighter::inc_int(int target) {
	if (object_manager->is_allow_realtime_process(this)) {
		fighter_int[target]++;
	}
}

void Fighter::dec_int(int target) {
	if (object_manager->is_allow_realtime_process(this)) {
		fighter_int[target]--;
	}
}

void Fighter::set_float(int target, float val) {
	fighter_float[target] = val;
}

void Fighter::set_flag(int target, bool val) {
	fighter_flag[target] = val;
}

void Fighter::set_string(int target, std::string val) {
	fighter_string[target] = val;
}

bool Fighter::can_spend_ex(float ex) {
	return fighter_float[FIGHTER_FLOAT_EX_METER] >= ex && !fighter_flag[FIGHTER_FLAG_LOCK_EX];
}

void Fighter::spend_ex(float ex) {
	if (fighter_flag[FIGHTER_FLAG_LOCK_EX] || ex == 0.0f) return;
	fighter_float[FIGHTER_FLOAT_EX_METER] -= ex;
	fighter_int[FIGHTER_INT_TRAINING_EX_RECOVERY_TIMER] = 60;
}

void Fighter::gain_ex(float ex) {
	if (fighter_flag[FIGHTER_FLAG_LOCK_EX]) return;
	float max_ex = get_global_param_float(PARAM_FIGHTER, "ex_meter_size");
	float ex_diff = max_ex - fighter_float[FIGHTER_FLOAT_EX_METER];
	if (ex > ex_diff) {
		fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(
			fighter_float[FIGHTER_FLOAT_SUPER_METER], 
			fighter_float[FIGHTER_FLOAT_SUPER_METER] + (ex - ex_diff), 
			get_global_param_float(PARAM_FIGHTER, "super_meter_size")
		);
		fighter_float[FIGHTER_FLOAT_EX_METER] = max_ex;
		return;
	}
	fighter_float[FIGHTER_FLOAT_EX_METER] = clampf(
		fighter_float[FIGHTER_FLOAT_EX_METER],
		fighter_float[FIGHTER_FLOAT_EX_METER] + ex,
		max_ex
	);
}