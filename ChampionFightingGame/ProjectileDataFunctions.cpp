#include "Projectile.h"

void Projectile::set_int(int target, int val) {
	projectile_int[target] = val;
}

void Projectile::inc_int(int target) {
	if (battle_object_manager->allow_dec_var(id)) {
		projectile_int[target]++;
	}
}

void Projectile::dec_int(int target) {
	if (battle_object_manager->allow_dec_var(id)) {
		projectile_int[target]--;
	}
}

void Projectile::set_float(int target, float val) {
	projectile_float[target] = val;
}

void Projectile::set_flag(int target, bool val) {
	projectile_flag[target] = val;
}