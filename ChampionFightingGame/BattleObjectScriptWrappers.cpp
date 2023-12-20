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
	HitStatus hit_status = HIT_STATUS_CUSTOM;
	int custom_hit_status = 0;
	if (NEXT_TYPEID(typeid(int))) {
		UNWRAP_NO_DECL(custom_hit_status);
	}
	else {
		UNWRAP_NO_DECL(hit_status, HitStatus);
	}
	HitFlag hit_flags = HIT_FLAG_NONE;
	if (NEXT_TYPEID(typeid(HitFlag))) {
		UNWRAP_NO_DECL(hit_flags);
	}
	SpecialStatusCondition special_status_condition = SPECIAL_STATUS_CONDITION_NONE;
	HitStatus special_status = hit_status;
	int custom_special_status = custom_hit_status;
	HitFlag special_hit_flags = hit_flags;
	if (NEXT_TYPEID(typeid(SpecialStatusCondition))) {
		UNWRAP_NO_DECL(special_status_condition);
		special_status = HIT_STATUS_CUSTOM;
		custom_special_status = 0;
		special_hit_flags = HIT_FLAG_NONE;
		if (NEXT_TYPEID(typeid(int))) {
			UNWRAP_NO_DECL(custom_special_status);
		}
		else {
			UNWRAP_NO_DECL(special_status, HitStatus);
		}
		if (NEXT_TYPEID(typeid(HitFlag))) {
			UNWRAP_NO_DECL(special_hit_flags);
		}
	}
	int juggle_start = 0;
	int juggle_increase = 0;
	int juggle_max = 0;
	if ((collision_kind & COLLISION_KIND_AIR) || hit_status == HIT_STATUS_LAUNCH
		|| special_status == HIT_STATUS_LAUNCH || hit_flags & HIT_FLAG_FORCE_AERIAL 
		|| special_hit_flags & HIT_FLAG_FORCE_AERIAL) {
		UNWRAP_NO_DECL(juggle_start);
		if (collision_kind & COLLISION_KIND_AIR) {
			UNWRAP_NO_DECL(juggle_increase);
			UNWRAP_NO_DECL(juggle_max);
		}
	}
	UNWRAP(hit_height, HitHeight);
	UNWRAP(damage, float);
	float chip_damage = 0.0;
	if (collision_kind & COLLISION_KIND_GROUND) {
		UNWRAP_NO_DECL(chip_damage);
	}
	UNWRAP(damage_scale, int);
	UNWRAP(meter_gain, float);
	UNWRAP(hitlag, int);
	int blocklag = 0;
	int blockstun = 0;
	if (collision_kind & COLLISION_KIND_GROUND) {
		UNWRAP_NO_DECL(blocklag);
	}
	UNWRAP(hitstun, int);
	if (collision_kind & COLLISION_KIND_GROUND) {
		UNWRAP_NO_DECL(blockstun);
	}
	float pushback_ground_hit = 0.0;
	float pushback_ground_block = 0.0;
	float pushback_air_x = 0.0;
	float pushback_air_y = 0.0;
	int pushback_frames = 0;
	if (hit_status == HIT_STATUS_NORMAL || special_status == HIT_STATUS_NORMAL) {
		if (collision_kind & COLLISION_KIND_GROUND) {
			UNWRAP_NO_DECL(pushback_ground_hit);
			UNWRAP_NO_DECL(pushback_ground_block);
		}
		if (collision_kind & COLLISION_KIND_AIR || hit_flags & HIT_FLAG_FORCE_AERIAL
			|| special_hit_flags & HIT_FLAG_FORCE_AERIAL) {
			UNWRAP_NO_DECL(pushback_air_x);
			UNWRAP_NO_DECL(pushback_air_y);
		}
		UNWRAP_NO_DECL(pushback_frames);
	}
	else if (collision_kind & COLLISION_KIND_GROUND) {
		UNWRAP_NO_DECL(pushback_ground_block);
		UNWRAP_NO_DECL(pushback_frames);
	}
	bool has_launch_target_pos = false;
	glm::vec3 launch_target_pos = glm::vec3(0.0);
	float launch_init_y = 0.0;
	float launch_gravity = 0.0;
	float launch_max_fall_speed = 0.0;
	float launch_speed_x = 0.0;
	if (hit_status == HIT_STATUS_LAUNCH || special_status == HIT_STATUS_LAUNCH
		|| hit_status == HIT_STATUS_FLOAT || special_status == HIT_STATUS_FLOAT
		|| hit_flags & HIT_FLAG_CONTINUE_LAUNCH) { //We can't counterhit an already-launched opponent,
		//so HIT_FLAG_CONTINUE_LAUNCH should never be in the special hitflags
		if (NEXT_TYPEID(typeid(glm::vec3))) {
			has_launch_target_pos = true;
			UNWRAP_NO_DECL(launch_target_pos);
		}
		else {
			UNWRAP_NO_DECL(launch_init_y);
			UNWRAP_NO_DECL(launch_gravity);
			UNWRAP_NO_DECL(launch_max_fall_speed);
			UNWRAP_NO_DECL(launch_speed_x);
		}
	}
	UNWRAP(damage_kind, DamageKind);
	UNWRAP(hit_level, HitLevel);
	UNWRAP(hit_effect_id, int);
	UNWRAP(hit_sound_id, int);
	new_hitbox(id, multihit, anchor, offset, collision_kind, hit_status, custom_hit_status, hit_flags,
		special_status_condition, special_status, custom_special_status, special_hit_flags, 
		juggle_start, juggle_increase, juggle_max, hit_height, damage, chip_damage, damage_scale, 
		meter_gain, hitlag, blocklag, hitstun, blockstun, pushback_ground_hit, pushback_ground_block, 
		pushback_air_x, pushback_air_y, pushback_frames, launch_init_y, launch_gravity, 
		launch_max_fall_speed, launch_speed_x, launch_target_pos, has_launch_target_pos, damage_kind, 
		hit_level, hit_effect_id, hit_sound_id
	);
}

void BattleObject::CLEAR_HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	clear_hitbox(id);
}

void BattleObject::CLEAR_HITBOX_ALL(ScriptArg args) {
	clear_hitbox_all();
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
	if (NEXT_TYPEID(typeid(float))) {
		UNWRAP_NO_DECL(volume_mod);
	}
	play_sound(sound, volume_mod);
}

void BattleObject::PLAY_RESERVED_SOUND(ScriptArg args) {
	UNWRAP(sound, std::string);
	float volume_mod = 0.0;
	if (NEXT_TYPEID(typeid(float))) {
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