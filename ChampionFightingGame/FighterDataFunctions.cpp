#include "Fighter.h"
#include "Projectile.h"

void Fighter::set_int(int target, int val) {
	fighter_int[target] = val;
}

void Fighter::inc_int(int target) {
	if (battle_object_manager->allow_dec_var(id)) {
		fighter_int[target]++;
	}
}

void Fighter::dec_int(int target) {
	if (battle_object_manager->allow_dec_var(id)) {
		fighter_int[target]--;
	}
}

void Fighter::set_float(int target, float val) {
	fighter_float[target] = val;
}

void Fighter::set_flag(int target, bool val) {
	fighter_flag[target] = val;
}