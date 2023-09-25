#include "BattleObject.h"

void BattleObject::SET_RATE(ScriptArg args) {
	if (args.args.front().type() != typeid(float)) {
		std::cout << args.args.front().type().name() << "\n";
	}
	UNWRAP(rate, float);
	set_rate(rate);
}

void BattleObject::SET_FRAME(ScriptArg args) {
	UNWRAP(frame, float);
	set_rate(frame);
}

void BattleObject::NEW_BLOCKBOX(ScriptArg args) {
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	new_blockbox(anchor, offset);
}

void BattleObject::NEW_HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(multihit, int);
	UNWRAP(damage, float);
	UNWRAP(chip_damage, float);
	UNWRAP(damage_scale, int);
	UNWRAP(meter_gain, float);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(hit_kind, HitKind);
	UNWRAP(attack_level, AttackLevel);
	UNWRAP(attack_height, AttackHeight);
	UNWRAP(hitlag, int);
	UNWRAP(blocklag, int);
	UNWRAP(hitstun, int);
	UNWRAP(blockstun, int);
	UNWRAP(hit_pushback, float);
	UNWRAP(block_pushback, float);
	UNWRAP(hit_status, HitStatus);
	UNWRAP(counterhit_status, HitStatus);
	UNWRAP(counterhit_type, CounterhitType);
	UNWRAP(juggle_start, int);
	UNWRAP(juggle_increase, int);
	UNWRAP(juggle_max, int);
	UNWRAP(clank_kind, ClankKind);
	UNWRAP(ko_kind, DamageKind);
	UNWRAP(continue_launch, bool);
	UNWRAP(disable_hitstun_parry, bool);
	UNWRAP(launch_init_y, float);
	UNWRAP(launch_gravity_y, float);
	UNWRAP(launch_max_fall_speed, float);
	UNWRAP(launch_speed_x, float);
	new_hitbox(id, multihit, damage, chip_damage, damage_scale, meter_gain,
		anchor, offset, hit_kind, attack_level, attack_height, hitlag, blocklag, hitstun,
		blockstun, hit_pushback, block_pushback, hit_status, counterhit_status, counterhit_type,
		juggle_start, juggle_increase, juggle_max, clank_kind, ko_kind, continue_launch,
		disable_hitstun_parry, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x);
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
	UNWRAP(hit_kind, HitKind);
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
	UNWRAP(armor, bool);
	UNWRAP(intangible_kind, IntangibleKind);

	new_hurtbox(id, anchor, offset, hurtbox_kind, armor, intangible_kind);
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

void BattleObject::PLAY_SE(ScriptArg args) {
	UNWRAP(se, std::string);
	play_se(se);
}

void BattleObject::PLAY_VC(ScriptArg args) {
	UNWRAP(vc, std::string);
	play_vc(vc);
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