#include "BattleObject.h"

void BattleObject::SET_RATE(ScriptArg args) {
	UNWRAP(rate, float);
	set_rate(rate);
}

void BattleObject::SET_FRAME(ScriptArg args) {
	UNWRAP(frame, float);
	set_frame(frame);
}

void BattleObject::NEW_BLOCKBOX(ScriptArg args) {
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	new_blockbox(anchor, offset);
}

void BattleObject::NEW_HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(multihit, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(collision_kind, CollisionKind);
	UNWRAP(damage, float);
	UNWRAP(chip_damage, float);
	UNWRAP(damage_scale, int);
	UNWRAP(meter_gain, float);
	UNWRAP(hitlag, int);
	UNWRAP(hitstun, int);
	int blocklag = 0;
	int blockstun = 0;
	if (collision_kind & COLLISION_KIND_GROUND) {
		UNWRAP_NO_DECL(blocklag);
	}
	if (collision_kind & COLLISION_KIND_GROUND) {
		UNWRAP_NO_DECL(blockstun);
	}
	HitStatus hit_status = HIT_STATUS_CUSTOM;
	int custom_hit_status = 0;
	if (WRAPPED_TYPE(int)) {
		UNWRAP_NO_DECL(custom_hit_status);
	}
	else {
		UNWRAP_NO_DECL(hit_status, HitStatus);
	}
	UNWRAP(hit_result, HitResult);
	UNWRAP(hit_flags, HitFlag);
	UNWRAP(critical_condition, CriticalCondition);
	HitStatus critical_status = hit_status;
	int custom_critical_status = custom_hit_status;
	HitResult critical_hit_result = hit_result;
	critical_hit_result.hitstun("stand_hitstun_critical", "crouch_hitstun_critical");
	HitFlag critical_hit_flags = hit_flags;
	if (critical_condition != CRITICAL_CONDITION_NONE) {
		critical_status = HIT_STATUS_CUSTOM;
		custom_critical_status = 0;
		if (WRAPPED_TYPE(int)) {
			UNWRAP_NO_DECL(custom_critical_status);
		}
		else {
			UNWRAP_NO_DECL(critical_status);
		}
		UNWRAP_NO_DECL(critical_hit_result);
		UNWRAP_NO_DECL(critical_hit_flags);
	}
	else if (hit_status == HIT_STATUS_NORMAL) {
		critical_hit_result.ground(5.0, 0.0).frames(15);
	}
	int juggle_start = 0;
	int juggle_increase = 0;
	int juggle_max = 0;
	if ((collision_kind & COLLISION_KIND_AIR) || hit_status == HIT_STATUS_LAUNCH
		|| critical_status == HIT_STATUS_LAUNCH || hit_flags & HIT_FLAG_FORCE_AERIAL 
		|| critical_hit_flags & HIT_FLAG_FORCE_AERIAL) {
		UNWRAP_NO_DECL(juggle_start);
		if (collision_kind & COLLISION_KIND_AIR) {
			UNWRAP_NO_DECL(juggle_increase);
			UNWRAP_NO_DECL(juggle_max);
		}
	}
	UNWRAP(hit_height, HitHeight);
	UNWRAP(damage_kind, DamageKind);
	UNWRAP(hit_effect, std::string);
	UNWRAP(hit_sound, std::string);;
	new_hitbox(id, multihit, anchor, offset, collision_kind, damage, chip_damage, damage_scale, 
		meter_gain, hitlag, hitstun, blocklag, blockstun, hit_status, custom_hit_status,
		hit_result, hit_flags, critical_condition, critical_status, custom_critical_status,
		critical_hit_result, critical_hit_flags, juggle_start, juggle_increase, juggle_max, 
		hit_height, damage_kind, hit_effect, hit_sound
	);
}

void BattleObject::CLEAR_HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	clear_hitbox(id);
}

void BattleObject::CLEAR_HITBOX_ALL(ScriptArg args) {
	clear_hitbox_all();
}

void BattleObject::SET_DEFINITE_HITBOX(ScriptArg args) {
	UNWRAP(target, Fighter*);
	UNWRAP(hit_status, int);
	UNWRAP(hit_flags, HitFlag);
	UNWRAP(juggle_start, int);
	UNWRAP(juggle_increase, int);
	UNWRAP(damage, float);
	UNWRAP(damage_scale, int);
	UNWRAP(meter_gain, float);
	UNWRAP(hitlag, int);
	UNWRAP(hitstun, int);
	UNWRAP(hit_result, HitResult);
	UNWRAP(damage_kind, DamageKind);
	UNWRAP(hit_effect, std::string);
	UNWRAP(hit_sound, std::string);
	set_definite_hitbox(target, hit_status, hit_flags, juggle_start, juggle_increase, damage,
		damage_scale, meter_gain, hitlag, hitstun, hit_result, damage_kind, hit_effect,
		hit_sound
	);
}

void BattleObject::NEW_GRABBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(grabbox_kind, GrabboxKind);
	UNWRAP(hit_kind, CollisionKind);
	UNWRAP(attacker_status_if_hit, int);
	UNWRAP(defender_status_if_hit, int);
	new_grabbox(id, anchor, offset, grabbox_kind, hit_kind, attacker_status_if_hit, defender_status_if_hit);
}

void BattleObject::CLEAR_GRABBOX(ScriptArg args) {
	UNWRAP(id, int);
	clear_grabbox(id);
}

void BattleObject::CLEAR_GRABBOX_ALL(ScriptArg args) {
	clear_grabbox_all();
}


void BattleObject::NEW_HURTBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(hurtbox_kind, HurtboxKind);
	UNWRAP(armor_hits, int);
	UNWRAP(intangible_kind, IntangibleKind);

	new_hurtbox(id, anchor, offset, hurtbox_kind, armor_hits, intangible_kind);
}

void BattleObject::CLEAR_HURTBOX(ScriptArg args) {
	UNWRAP(id, int);
	clear_hurtbox(id);
}

void BattleObject::CLEAR_HURTBOX_ALL(ScriptArg args) {
	clear_hurtbox_all();
}

void BattleObject::SET_HURTBOX_INTANGIBLE_KIND(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(intangible_kind, IntangibleKind);
	set_hurtbox_intangible_kind(id, intangible_kind);
}

void BattleObject::NEW_PUSHBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	new_pushbox(id, anchor, offset);
}

void BattleObject::CLEAR_PUSHBOX(ScriptArg args) {
	UNWRAP(id, int);
	clear_pushbox(id);
}

void BattleObject::CLEAR_PUSHBOX_ALL(ScriptArg args) {
	clear_pushbox_all();
}

void BattleObject::PLAY_SOUND(ScriptArg args) {
	UNWRAP(sound, std::string);
	float volume_mod = 0.0;
	if (WRAPPED_TYPE(float)) {
		UNWRAP_NO_DECL(volume_mod);
	}
	play_sound(sound, volume_mod);
}

void BattleObject::PLAY_RESERVED_SOUND(ScriptArg args) {
	UNWRAP(sound, std::string);
	float volume_mod = 0.0;
	if (WRAPPED_TYPE(float)) {
		UNWRAP_NO_DECL(volume_mod);
	}
	play_reserved_sound(sound, volume_mod);
}

void BattleObject::NEW_EFFECT(ScriptArg args) {
	int overload = 3;
	std::any bone_container;
	std::any offset_container;
	UNWRAP(name, std::string);
	UNWRAP(pos, glm::vec3);
	UNWRAP(rot, glm::vec3);
	UNWRAP(scale, glm::vec3);
	UNWRAP(rgba, glm::vec4);
	if (args.args.front().type() != typeid(glm::vec3)) {
		if (args.args.front().type() == typeid(int)) {
			overload = 1;

		}
		else if (args.args.front().type() == typeid(std::string)) {
			overload = 2;
		}
		bone_container = args.get_arg();
		offset_container = args.get_arg();
	}
	else {
		overload = 0;
	}
	UNWRAP(pos_frame, glm::vec3);
	UNWRAP(rot_frame, glm::vec3);
	UNWRAP(scale_frame, glm::vec3);
	UNWRAP(rgba_frame, glm::vec4);
	int* interp_var = nullptr;
	UNWRAP_NO_DECL(interp_var);
	float rate = 1.0;
	UNWRAP_NO_DECL(rate);
	float frame = 0.0;
	UNWRAP_NO_DECL(frame);
	switch (overload) {
		case (0): {
			new_effect(name, pos, rot, scale, rgba, pos_frame, rot_frame, scale_frame, rgba_frame, interp_var, rate, frame);
		} break;
		case (1): {
			int bone_id = std::any_cast<int>(bone_container);
			glm::vec3 bone_offset = std::any_cast<glm::vec3>(offset_container);
			new_effect(name, pos, rot, scale, rgba, bone_id, bone_offset, pos_frame, rot_frame, scale_frame, rgba_frame, interp_var, rate, frame);
		} break;
		case (2): {
			std::string bone_name = std::any_cast<std::string>(bone_container);
			glm::vec3 bone_offset = std::any_cast<glm::vec3>(offset_container);
			new_effect(name, pos, rot, scale, rgba, bone_name, bone_offset, pos_frame, rot_frame, scale_frame, rgba_frame, interp_var, rate, frame);
		} break;
		default: {
			GameManager::get_instance()->add_crash_log("ERROR: Arg 6 of NEW_EFFECT in script " 
				+ active_move_script.name + " was neither int, string, nor glm::vec3. How you managed "
				+ "to get here without crashing is beyond me.");
		} break;
	}
}

void BattleObject::NEW_EFFECT_NO_FOLLOW(ScriptArg args) {
	UNWRAP(name, std::string);
	UNWRAP(pos, glm::vec3);
	UNWRAP(rot, glm::vec3);
	UNWRAP(scale, glm::vec3);
	UNWRAP(rgba, glm::vec4);
	UNWRAP(pos_frame, glm::vec3);
	UNWRAP(rot_frame, glm::vec3);
	UNWRAP(scale_frame, glm::vec3);
	UNWRAP(rgba_frame, glm::vec4);
	int* interp_var = nullptr;
	UNWRAP_NO_DECL(interp_var);
	float rate = 1.0;
	UNWRAP_NO_DECL(rate);
	float frame = 0.0;
	UNWRAP_NO_DECL(frame);
	new_effect_no_follow(name, pos, rot, scale, rgba, pos_frame, rot_frame, scale_frame, rgba_frame, interp_var, rate, frame);
}

void BattleObject::CLEAR_EFFECT(ScriptArg args) {
	UNWRAP(name, std::string);
	int instance_id = 0;
	UNWRAP_NO_DECL(instance_id);
	clear_effect(name, instance_id);
}

void BattleObject::CLEAR_EFFECT_ALL(ScriptArg args) {
	clear_effect_all();
}

void BattleObject::PRINT_MSG_FROM_SCRIPT(ScriptArg args) {
	UNWRAP(message, std::string);
	std::cout << message;
}