#include "BattleObject.h"
#include "Fighter.h"
#include "Projectile.h"

void BattleObject::check_incoming_hitbox_collisions(std::vector<Hitbox*> hitboxes) {
	for (size_t i = 0; i < 10; i++) {
		if (!hurtboxes[i].active) continue;
		for (size_t i2 = 0, max2 = hitboxes.size(); i2 < max2; i2++) {
			if (is_valid_incoming_hitbox_collision(&hurtboxes[i], hitboxes[i2])) {
				connected_hitbox = hitboxes[i2];
				return;
			}
		}
	}
}

bool BattleObject::is_valid_incoming_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox) {
	if (!is_collide(hurtbox->rect, hitbox->rect)) return false;
	switch (hitbox->object->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			return is_valid_incoming_fighter_hitbox_collision(hurtbox, hitbox, (Fighter*)hitbox->object);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			return is_valid_incoming_projectile_hitbox_collision(hurtbox, hitbox, (Projectile*)hitbox->object);
		} break;
		default: {
			return false;
		} break;
	}
}

bool BattleObject::is_valid_incoming_fighter_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Fighter* attacker) {
	return false;
}

bool BattleObject::is_valid_incoming_projectile_hitbox_collision(Hurtbox* hurtbox, Hitbox* hitbox, Projectile* attacker) {
	return false;
}

void BattleObject::process_incoming_hitbox_collision(Hitbox* hitbox, BattleObject* attacker) {
	switch (incoming_collision_kind) {
	case INCOMING_COLLISION_KIND_HIT: {
		process_incoming_hitbox_collision_hit(hitbox, attacker);
	} break;
	case INCOMING_COLLISION_KIND_BLOCKED: {
		process_incoming_hitbox_collision_blocked(hitbox, attacker);
	} break;
	case INCOMING_COLLISION_KIND_PARRIED: {
		process_incoming_hitbox_collision_parried(hitbox, attacker);
	} break;
	case INCOMING_COLLISION_KIND_HITSTUN_PARRIED: {
		process_incoming_hitbox_collision_hitstun_parried(hitbox, attacker);
	} break;
	case INCOMING_COLLISION_KIND_ARMORED: {
		process_incoming_hitbox_collision_armored(hitbox, attacker);
	} break;
	case INCOMING_COLLISION_KIND_RIGHT_OF_WAY_ARMORED: {
		process_incoming_hitbox_collision_right_of_way_armored(hitbox, attacker);
	} break;
	case INCOMING_COLLISION_KIND_INVINCIBILITY: {
		process_incoming_hitbox_collision_invincibility(hitbox, attacker);
	} break;
	case INCOMING_COLLISION_KIND_COUNTER: {
		process_incoming_hitbox_collision_counter(hitbox, attacker);
	} break;
	default:
	case INCOMING_COLLISION_KIND_MAX: {

	} break;

	}
}

void BattleObject::process_incoming_hitbox_collision_hit(Hitbox* hitbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_hitbox_collision_hit(hitbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_hitbox_collision_hit(hitbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_incoming_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* attacker) {

}

void BattleObject::process_incoming_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* attacker) {

}

void BattleObject::process_outgoing_hitbox_collision_hit(Hitbox* hitbox, BattleObject* defender) {
	switch (defender->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_outgoing_fighter_hitbox_collision_hit(hitbox, (Fighter*)defender);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_outgoing_projectile_hitbox_collision_hit(hitbox, (Projectile*)defender);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_outgoing_fighter_hitbox_collision_hit(Hitbox* hitbox, Fighter* defender) {

}

void BattleObject::process_outgoing_projectile_hitbox_collision_hit(Hitbox* hitbox, Projectile* defender) {

}

void BattleObject::process_incoming_hitbox_collision_blocked(Hitbox* hitbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_hitbox_collision_blocked(hitbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_hitbox_collision_blocked(hitbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_incoming_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* attacker) {

}

void BattleObject::process_incoming_projectile_hitbox_collision_blocked(Hitbox* hitbox, Projectile* attacker) {

}

void BattleObject::process_outgoing_hitbox_collision_blocked(Hitbox* hitbox, BattleObject* defender) {
	switch (defender->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_outgoing_fighter_hitbox_collision_blocked(hitbox, (Fighter*)defender);
		} break;
		default:
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
		} break;
	}
}

void BattleObject::process_outgoing_fighter_hitbox_collision_blocked(Hitbox* hitbox, Fighter* defender) {

}

void BattleObject::process_incoming_hitbox_collision_parried(Hitbox* hitbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_hitbox_collision_parried(hitbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_hitbox_collision_parried(hitbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_incoming_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* attacker) {

}

void BattleObject::process_incoming_projectile_hitbox_collision_parried(Hitbox* hitbox, Projectile* attacker) {

}

void BattleObject::process_outgoing_hitbox_collision_parried(Hitbox* hitbox, BattleObject* defender) {
	switch (defender->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_outgoing_fighter_hitbox_collision_parried(hitbox, (Fighter*)defender);
		} break;
		default:
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
		} break;
	}
}

void BattleObject::process_outgoing_fighter_hitbox_collision_parried(Hitbox* hitbox, Fighter* defender) {

}

void BattleObject::process_incoming_hitbox_collision_hitstun_parried(Hitbox* hitbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_hitbox_collision_hitstun_parried(hitbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_hitbox_collision_hitstun_parried(hitbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_incoming_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* attacker) {

}

void BattleObject::process_incoming_projectile_hitbox_collision_hitstun_parried(Hitbox* hitbox, Projectile* attacker) {

}

void BattleObject::process_outgoing_hitbox_collision_hitstun_parried(Hitbox* hitbox, BattleObject* defender) {
	switch (defender->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_outgoing_fighter_hitbox_collision_hitstun_parried(hitbox, (Fighter*)defender);
		} break;
		default:
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
		} break;
	}
}

void BattleObject::process_outgoing_fighter_hitbox_collision_hitstun_parried(Hitbox* hitbox, Fighter* defender) {

}

void BattleObject::process_incoming_hitbox_collision_armored(Hitbox* hitbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_hitbox_collision_armored(hitbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_hitbox_collision_armored(hitbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_incoming_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* attacker) {

}

void BattleObject::process_incoming_projectile_hitbox_collision_armored(Hitbox* hitbox, Projectile* attacker) {

}

void BattleObject::process_outgoing_hitbox_collision_armored(Hitbox* hitbox, BattleObject* defender) {
	switch (defender->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_outgoing_fighter_hitbox_collision_armored(hitbox, (Fighter*)defender);
		} break;
		default:
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
		} break;
	}
}

void BattleObject::process_outgoing_fighter_hitbox_collision_armored(Hitbox* hitbox, Fighter* defender) {

}

void BattleObject::process_incoming_hitbox_collision_right_of_way_armored(Hitbox* hitbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_hitbox_collision_right_of_way_armored(hitbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_hitbox_collision_right_of_way_armored(hitbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_incoming_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* attacker) {

}

void BattleObject::process_incoming_projectile_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Projectile* attacker) {

}

void BattleObject::process_outgoing_hitbox_collision_right_of_way_armored(Hitbox* hitbox, BattleObject* defender) {
	switch (defender->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_outgoing_fighter_hitbox_collision_right_of_way_armored(hitbox, (Fighter*)defender);
		} break;
		default:
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
		} break;
	}
}

void BattleObject::process_outgoing_fighter_hitbox_collision_right_of_way_armored(Hitbox* hitbox, Fighter* defender) {

}

void BattleObject::process_incoming_hitbox_collision_invincibility(Hitbox* hitbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_hitbox_collision_invincibility(hitbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_hitbox_collision_invincibility(hitbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_incoming_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* attacker) {

}

void BattleObject::process_incoming_projectile_hitbox_collision_invincibility(Hitbox* hitbox, Projectile* attacker) {

}

void BattleObject::process_outgoing_hitbox_collision_invincibility(Hitbox* hitbox, BattleObject* defender) {
	switch (defender->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_outgoing_fighter_hitbox_collision_invincibility(hitbox, (Fighter*)defender);
		} break;
		default:
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
		} break;
	}
}

void BattleObject::process_outgoing_fighter_hitbox_collision_invincibility(Hitbox* hitbox, Fighter* defender) {

}

void BattleObject::process_incoming_hitbox_collision_counter(Hitbox* hitbox, BattleObject* attacker) {
	switch (attacker->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_incoming_fighter_hitbox_collision_counter(hitbox, (Fighter*)attacker);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_incoming_projectile_hitbox_collision_counter(hitbox, (Projectile*)attacker);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_incoming_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* attacker) {

}

void BattleObject::process_incoming_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* attacker) {

}

void BattleObject::process_outgoing_hitbox_collision_counter(Hitbox* hitbox, BattleObject* defender) {
	switch (defender->object_type) {
		case BATTLE_OBJECT_TYPE_FIGHTER: {
			process_outgoing_fighter_hitbox_collision_counter(hitbox, (Fighter*)defender);
		} break;
		case BATTLE_OBJECT_TYPE_PROJECTILE: {
			process_outgoing_projectile_hitbox_collision_counter(hitbox, (Projectile*)defender);
		} break;
		default: {
		} break;
	}
}

void BattleObject::process_outgoing_fighter_hitbox_collision_counter(Hitbox* hitbox, Fighter* defender) {

}

void BattleObject::process_outgoing_projectile_hitbox_collision_counter(Hitbox* hitbox, Projectile* defender) {

}