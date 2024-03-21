#include "Projectile.h"
#include "Fighter.h"
#include "utils.h"

bool Projectile::is_valid_incoming_fighter_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Fighter* attacker) {
	if (attacker->multihit_connected[hitbox->multihit]
	|| !(hitbox->collision_kind & COLLISION_KIND_PROJECTILE)) {
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
	|| !(hitbox->collision_kind & COLLISION_KIND_PROJECTILE)
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
	unique_process_incoming_fighter_hitbox_collision_hit(hitbox, attacker);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
	attacker->process_outgoing_projectile_hitbox_collision_hit(hitbox, this);
}

void Projectile::process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker) {
	projectile_int[PROJECTILE_INT_HEALTH]--;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	unique_process_incoming_projectile_hitbox_collision_hit(hitbox, attacker);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
	attacker->process_outgoing_projectile_hitbox_collision_hit(hitbox, this);
}

void Projectile::process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	owner->fighter_int[FIGHTER_INT_COMBO_COUNT]++;
	owner->fighter_flag[FIGHTER_FLAG_PROJECTILE_HIT_DURING_STATUS] = true;
	unique_process_outgoing_fighter_hitbox_collision_hit(hitbox, defender);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
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
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	owner->gain_ex(hitbox->hit_result.meter_gain * 0.5);
	unique_process_outgoing_fighter_hitbox_collision_blocked(hitbox, defender);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
}

void Projectile::process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 16;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = 16;
	owner->gain_ex(hitbox->hit_result.meter_gain * 0.5);
	unique_process_outgoing_fighter_hitbox_collision_parried(hitbox, defender);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
}

void Projectile::process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = 16;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = 16;
	owner->gain_ex(hitbox->hit_result.meter_gain * 0.5);
	unique_process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, defender);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
}

void Projectile::process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag / 2;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	owner->gain_ex(hitbox->hit_result.meter_gain * 0.3);
	unique_process_outgoing_fighter_hitbox_collision_armored(hitbox, defender);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
}

void Projectile::process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_right_of_way_armored(hitbox, defender);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
}

void Projectile::process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.blocklag / 2;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	unique_process_outgoing_fighter_hitbox_collision_invincibility(hitbox, defender);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
}

void Projectile::process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker) {
	attacker->update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	post_collision_status = projectile_int[PROJECTILE_INT_COUNTER_DEFENDER_STATUS];
	unique_process_incoming_fighter_hitbox_collision_counter(hitbox, attacker);
	attacker->process_outgoing_projectile_hitbox_collision_counter(hitbox, this);
}

void Projectile::process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker) {
	attacker->update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	post_collision_status = projectile_int[PROJECTILE_INT_COUNTER_DEFENDER_STATUS];
	unique_process_incoming_projectile_hitbox_collision_counter(hitbox, attacker);
	attacker->process_outgoing_projectile_hitbox_collision_counter(hitbox, this);
}

void Projectile::process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	if (owner->post_collision_status == FIGHTER_STATUS_NONE) {
		owner->post_collision_status = defender->fighter_int[FIGHTER_INT_COUNTER_ATTACKER_STATUS];
	}
	unique_process_outgoing_fighter_hitbox_collision_counter(hitbox, defender);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
}

void Projectile::process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender) {
	update_hitbox_connect(hitbox->id);
	projectile_int[PROJECTILE_INT_HEALTH]--;
	projectile_int[PROJECTILE_INT_HITLAG_FRAMES] = hitbox->hit_result.hitlag;
	projectile_int[PROJECTILE_INT_INIT_HITLAG_FRAMES] = projectile_int[PROJECTILE_INT_HITLAG_FRAMES];
	if (owner->post_collision_status == FIGHTER_STATUS_NONE) {
		owner->post_collision_status = defender->projectile_int[PROJECTILE_INT_COUNTER_ATTACKER_STATUS];
	}
	unique_process_outgoing_projectile_hitbox_collision_counter(hitbox, defender);
	if (projectile_int[PROJECTILE_INT_HEALTH] <= 0) {
		deactivate();
	}
}

void Projectile::process_outgoing_fighter_grabbox_collision(Grabbox* grabbox, Fighter* defender) {
	unique_process_outgoing_fighter_grabbox_collision(grabbox, defender);
}