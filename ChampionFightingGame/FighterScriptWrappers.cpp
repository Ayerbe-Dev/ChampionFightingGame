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

void Fighter::GRAB_OPPONENT(ScriptArg args) {
	UNWRAP(attacker_bone_name, std::string);
	UNWRAP(defender_bone_name, std::string);
	UNWRAP(offset, glm::vec2);
	UNWRAP(frames, int);
	grab_opponent(attacker_bone_name, defender_bone_name, offset, frames);
}

void Fighter::THROW_OPPONENT(ScriptArg args) {
	UNWRAP(damage, float);
	UNWRAP(x_speed, float);
	UNWRAP(y_speed, float);
	UNWRAP(gravity, float);
	UNWRAP(max_fall_speed, float);
	throw_opponent(damage, x_speed, y_speed, gravity, max_fall_speed);
}

void Fighter::ADD_POS(ScriptArg args) {
	UNWRAP(pos, glm::vec3);
	add_pos(pos);
}

void Fighter::SET_POS(ScriptArg args) {
	UNWRAP(pos, glm::vec3);
	set_pos(pos);
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
	UNWRAP(pos, glm::vec3);
	add_projectile_pos(projectile, pos);
}

void Fighter::SET_PROJECTILE_POS(ScriptArg args) {
	UNWRAP(projectile, int);
	UNWRAP(pos, glm::vec3);
	set_projectile_pos(projectile, pos);
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