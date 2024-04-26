#include "Projectile.h"

void Projectile::set_int(int target, int val) {
	object_int[target] = val;
}

void Projectile::inc_int(int target) {
	if (object_manager->is_allow_realtime_process(this)) {
		object_int[target]++;
	}
}

void Projectile::dec_int(int target) {
	if (object_manager->is_allow_realtime_process(this)) {
		object_int[target]--;
	}
}

void Projectile::set_float(int target, float val) {
	object_float[target] = val;
}

void Projectile::set_flag(int target, bool val) {
	object_flag[target] = val;
}

void Projectile::set_string(int target, std::string val) {
	object_string[target] = val;
}