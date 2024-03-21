#include "BattleObject.h"

void BattleObject::set_definite_hitbox(Fighter* target, unsigned int hit_status,
	HitFlag hit_flags, int juggle_start, int juggle_increase, float damage, int damage_scale,
	float meter_gain, int hitlag, int hitstun, std::string hit_anim, HitMove hit_move, 
	DamageKind damage_kind, std::string hit_effect, std::string hit_sound) {
	definite_hitbox.set_properties(this, target, hit_status, hit_flags, juggle_start, juggle_increase,
		damage, damage_scale, meter_gain, hitlag, hitstun, hit_anim, hit_move, damage_kind, hit_effect,
		hit_sound
	);
	definite_hitbox.activate();
}