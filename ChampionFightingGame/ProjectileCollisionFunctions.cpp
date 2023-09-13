#include "Projectile.h"
#include "Fighter.h"
#include "utils.h"

bool Projectile::is_valid_incoming_fighter_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Fighter* attacker) {
	if (attacker->multihit_connected[hitbox->multihit]
	|| !(hitbox->hit_kind & HIT_KIND_PROJECTILE)
	|| hitbox->attack_level < projectile_int[PROJECTILE_INT_ATTACK_LEVEL]) {
		return false;
	}

	if (hurtbox->hurtbox_kind == HURTBOX_KIND_COUNTER) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_COUNTER;
		return true;
	}
	
	incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
	return true;
}

bool Projectile::is_valid_incoming_projectile_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Projectile* attacker) {
	if (attacker->multihit_connected[hitbox->multihit]
	|| attacker->projectile_int[PROJECTILE_INT_HITLAG_FRAMES]
	|| !(hitbox->hit_kind & HIT_KIND_PROJECTILE)
	|| attacker->projectile_int[PROJECTILE_INT_ATTACK_LEVEL] < projectile_int[PROJECTILE_INT_ATTACK_LEVEL]) {
		return false;
	}


	if (hurtbox->hurtbox_kind == HURTBOX_KIND_COUNTER) {
		incoming_collision_kind = INCOMING_COLLISION_KIND_COUNTER;
		return true;
	}
	incoming_collision_kind = INCOMING_COLLISION_KIND_HIT;
	return true;
}

void Projectile::process_incoming_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* attacker) {
	projectile_int[PROJECTILE_INT_HEALTH]--;
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		post_collision_status = PROJECTILE_STATUS_DEACTIVATE;
	}
	unique_process_incoming_fighter_hitbox_collision_hit(hitbox, attacker);
	attacker->process_outgoing_projectile_hitbox_collision_hit(hitbox, this);
}

void Projectile::process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker) {
	projectile_int[PROJECTILE_INT_HEALTH]--;
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		post_collision_status = PROJECTILE_STATUS_DEACTIVATE;
	}
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	unique_process_incoming_projectile_hitbox_collision_hit(hitbox, attacker);
	attacker->process_outgoing_projectile_hitbox_collision_hit(hitbox, this);
}

void Projectile::process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		post_collision_status = PROJECTILE_STATUS_DEACTIVATE;
	}
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = hitbox->hitlag;
	owner->fighter_int[FIGHTER_INT_COMBO_COUNT]++;
	owner->fighter_flag[FIGHTER_FLAG_PROJECTILE_CONNECTED_DURING_STATUS] = true;
	unique_process_outgoing_fighter_hitbox_collision_hit(hitbox, defender);
}

void Projectile::process_outgoing_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* defender) {
	if (projectile_int[PROJECTILE_INT_ATTACK_LEVEL] == defender->projectile_int[PROJECTILE_INT_ATTACK_LEVEL]) {
		update_hitbox_connect(hitbox->id);
	}
	unique_process_outgoing_projectile_hitbox_collision_hit(hitbox, defender);
}

void Projectile::process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		post_collision_status = PROJECTILE_STATUS_DEACTIVATE;
	}
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	owner->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, owner->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitbox->meter_gain * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	unique_process_outgoing_fighter_hitbox_collision_blocked(hitbox, defender);
}

void Projectile::process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		post_collision_status = PROJECTILE_STATUS_DEACTIVATE;
	}
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 16;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = 16;
	owner->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, owner->fighter_float[FIGHTER_FLOAT_SUPER_METER] + get_local_param_float("meter_gain_on_parry") * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	unique_process_outgoing_fighter_hitbox_collision_parried(hitbox, defender);
}

void Projectile::process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 16;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = 16;
	owner->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0, owner->fighter_float[FIGHTER_FLOAT_SUPER_METER] + get_local_param_float("meter_gain_on_parry") * 0.5, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	unique_process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, defender);
}

void Projectile::process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	owner->fighter_float[FIGHTER_FLOAT_SUPER_METER] = clampf(0.0, owner->fighter_float[FIGHTER_FLOAT_SUPER_METER] + hitbox->meter_gain * 0.3, get_param_int(PARAM_FIGHTER, "ex_meter_size"));
	unique_process_outgoing_fighter_hitbox_collision_armored(hitbox, defender);
}

void Projectile::process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_right_of_way_armored(hitbox, defender);
}

void Projectile::process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag / 2;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_invincibility(hitbox, defender);
}

void Projectile::process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker) {
	attacker->update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	unique_process_incoming_fighter_hitbox_collision_counter(hitbox, attacker);
	attacker->process_outgoing_projectile_hitbox_collision_counter(hitbox, this);
}

void Projectile::process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker) {
	attacker->update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	unique_process_incoming_projectile_hitbox_collision_counter(hitbox, attacker);
	attacker->process_outgoing_projectile_hitbox_collision_counter(hitbox, this);
}

void Projectile::process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_counter(hitbox, defender);
}

void Projectile::process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->blocklag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	unique_process_outgoing_projectile_hitbox_collision_counter(hitbox, defender);
}

void Projectile::process_outgoing_fighter_grabbox_collision(Grabbox* grabbox, Fighter* defender) {
	unique_process_outgoing_fighter_grabbox_collision(grabbox, defender);
}