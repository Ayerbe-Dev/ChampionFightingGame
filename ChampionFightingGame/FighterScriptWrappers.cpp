#include "Fighter.h"

void Fighter::SET_INT(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, int);

	set_int(target, val);
}

void Fighter::SET_FLOAT(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, float);

	set_float(target, val);
}

void Fighter::SET_FLAG(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, bool);

	set_flag(target, val);
}

void Fighter::NEW_HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(multihit, int);
	UNWRAP(damage, float);
	UNWRAP(chip_damage, float);
	UNWRAP(damage_scale, int);
	UNWRAP(meter_gain, float);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(situation_hit, SituationHit);
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
	UNWRAP(ko_kind, KoKind);
	UNWRAP(continue_launch, bool);
	UNWRAP(disable_hitstun_parry, bool);
	UNWRAP(launch_init_y, float);
	UNWRAP(launch_gravity_y, float);
	UNWRAP(launch_max_fall_speed, float);
	UNWRAP(launch_speed_x, float);
	bool use_player_pos = true;
	UNWRAP_NO_DECL(use_player_pos);
	new_hitbox(id, multihit, damage, chip_damage, damage_scale, meter_gain,
		anchor, offset, situation_hit, attack_level, attack_height, hitlag, blocklag, hitstun,
		blockstun, hit_pushback, block_pushback, hit_status, counterhit_status, counterhit_type,
		juggle_start, juggle_increase, juggle_max, clank_kind, ko_kind, continue_launch,
		disable_hitstun_parry, launch_init_y, launch_gravity_y, launch_max_fall_speed, launch_speed_x,
		use_player_pos);
}

void Fighter::CLEAR_HITBOX(ScriptArg args) {
	UNWRAP(id, int);
	clear_hitbox(id);
}

void Fighter::CLEAR_HITBOX_ALL(ScriptArg args) {
	clear_hitbox_all();
}

void Fighter::NEW_GRABBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);
	UNWRAP(grabbox_kind, GrabboxKind);
	UNWRAP(situation_hit, SituationHit);
	UNWRAP(attacker_status_if_hit, int);
	UNWRAP(defender_status_if_hit, int);
	bool use_player_pos = true;
	UNWRAP_NO_DECL(use_player_pos);
	new_grabbox(id, anchor, offset, grabbox_kind, situation_hit, attacker_status_if_hit, defender_status_if_hit, use_player_pos);
}

void Fighter::CLEAR_GRABBOX(ScriptArg args) {
	UNWRAP(id, int);
	clear_grabbox(id);
}

void Fighter::CLEAR_GRABBOX_ALL(ScriptArg args) {
	clear_grabbox_all();
}


void Fighter::NEW_HURTBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);

	HurtboxKind hurtbox_kind = HURTBOX_KIND_NORMAL;
	bool armor = false;
	IntangibleKind intangible_kind = INTANGIBLE_KIND_NONE;

	UNWRAP_NO_DECL(hurtbox_kind);
	UNWRAP_NO_DECL(armor);
	UNWRAP_NO_DECL(intangible_kind);

	new_hurtbox(id, anchor, offset, hurtbox_kind, armor, intangible_kind);
}

void Fighter::CLEAR_HURTBOX(ScriptArg args) {
	UNWRAP(id, int);
	clear_hurtbox(id);
}

void Fighter::CLEAR_HURTBOX_ALL(ScriptArg args) {
	clear_hurtbox_all();
}

void Fighter::GRAB_OPPONENT(ScriptArg args) {
	UNWRAP(attacker_bone_name, std::string);
	UNWRAP(defender_bone_name, std::string);
	UNWRAP(offset, glm::vec2);
	UNWRAP(frames, int);
	grab_opponent(attacker_bone_name, defender_bone_name, offset, frames);
}

void Fighter::THROW_OPPONENT(ScriptArg args) {
	UNWRAP(damage, float);
	float x_speed = 0.0;
	UNWRAP_NO_DECL(x_speed);
	float y_speed = 0.0;
	UNWRAP_NO_DECL(y_speed);
	throw_opponent(damage, x_speed, y_speed);
}

void Fighter::SET_JOSTLE_OFFSET(ScriptArg args) {
	UNWRAP(offset, float);
	set_jostle_offset(offset);
}

void Fighter::ADD_POS(ScriptArg args) {
	if (args.args.front().type() == typeid(float)) {
		UNWRAP(pos_x, float);
		UNWRAP(pos_y, float);
		add_pos(pos_x, pos_y);
	}
	else {
		UNWRAP(pos, glm::vec3);
		add_pos(pos);
	}
}

void Fighter::SET_POS(ScriptArg args) {
	if (args.args.front().type() == typeid(float)) {
		UNWRAP(pos_x, float);
		UNWRAP(pos_y, float);
		set_pos(pos_x, pos_y);
	}
	else {
		UNWRAP(pos, glm::vec3);
		set_pos(pos);
	}
}

void Fighter::REENTER_LAST_ANIM(ScriptArg args) {
	reenter_last_anim();
}

void Fighter::START_CINEMATIC_SEQUENCE(ScriptArg args) {
	UNWRAP(anim_kind, std::string);
	UNWRAP(anim_rate, float);
	UNWRAP(anim_frame, float);
	UNWRAP(world_brightness, float);
	UNWRAP(dim_self, bool);
	UNWRAP(world_rate, float);
	start_cinematic_sequence(anim_kind, anim_rate, anim_frame, world_brightness, dim_self, world_rate);
}

void Fighter::RESET_WORLD_RATE(ScriptArg args) {
	reset_world_rate();
}

void Fighter::ENABLE_CANCEL(ScriptArg args) {
	UNWRAP(cat, int);
	UNWRAP(kind, int);
	enable_cancel(cat, kind);
}

void Fighter::DISABLE_CANCEL(ScriptArg args) {
	UNWRAP(cat, int);
	UNWRAP(kind, int);
	disable_cancel(cat, kind);
}

void Fighter::DISABLE_ALL_CANCELS(ScriptArg args) {
	disable_all_cancels();
}

void Fighter::CHANGE_STATUS(ScriptArg args) {
	UNWRAP(new_status_kind, int);
	bool call_end_status = true;
	UNWRAP_NO_DECL(call_end_status);
	bool require_different_status = true;
	UNWRAP_NO_DECL(require_different_status);
	change_status(new_status_kind, call_end_status, require_different_status);
}

void Fighter::ACTIVATE_PROJECTILE(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(pos, glm::vec3);
	activate_projectile(id, pos);
}

void Fighter::DEACTIVATE_PROJECTILE(ScriptArg args) {
	UNWRAP(id, int);
	deactivate_projectile(id);
}

void Fighter::SET_PROJECTILE_INT(ScriptArg args) {
	UNWRAP(projectile, int);
	UNWRAP(target, int);
	UNWRAP(val, int);

	set_projectile_int(projectile, target, val);
}

void Fighter::SET_PROJECTILE_FLOAT(ScriptArg args) {
	UNWRAP(projectile, int);
	UNWRAP(target, int);
	UNWRAP(val, float);

	set_projectile_int(projectile, target, val);
}

void Fighter::SET_PROJECTILE_FLAG(ScriptArg args) {
	UNWRAP(projectile, int);
	UNWRAP(target, int);
	UNWRAP(val, bool);

	set_projectile_int(projectile, target, val);
}

void Fighter::ADD_PROJECTILE_POS(ScriptArg args) {
	UNWRAP(projectile, int);
	if (args.args.front().type() == typeid(float)) {
		UNWRAP(pos_x, float);
		UNWRAP(pos_y, float);
		add_projectile_pos(projectile, pos_x, pos_y);
	}
	else {
		UNWRAP(pos, glm::vec3);
		add_projectile_pos(projectile, pos);
	}
}

void Fighter::SET_PROJECTILE_POS(ScriptArg args) {
	UNWRAP(projectile, int);
	if (args.args.front().type() == typeid(float)) {
		UNWRAP(pos_x, float);
		UNWRAP(pos_y, float);
		set_projectile_pos(projectile, pos_x, pos_y);
	}
	else {
		UNWRAP(pos, glm::vec3);
		set_projectile_pos(projectile, pos);
	}
}

void Fighter::CHANGE_PROJECTILE_STATUS(ScriptArg args) {
	UNWRAP(projectile, int);
	UNWRAP(new_status_kind, int);
	bool call_end_status = true;
	UNWRAP_NO_DECL(call_end_status);
	bool require_different_status = true;
	UNWRAP_NO_DECL(require_different_status);
	change_projectile_status(projectile, new_status_kind, call_end_status, require_different_status);
}

void Fighter::CHANGE_OPPONENT_STATUS(ScriptArg args) {
	UNWRAP(status_kind, int);
	change_opponent_status(status_kind);
}

void Fighter::CHANGE_OPPONENT_ANIM(ScriptArg args) {
	UNWRAP(anim_kind, std::string);
	float rate = 1.0;
	UNWRAP_NO_DECL(rate);
	float frame = 0.0;
	UNWRAP_NO_DECL(frame);
	change_opponent_anim(anim_kind, rate, frame);
}