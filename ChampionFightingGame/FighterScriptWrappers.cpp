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

void Fighter::SET_STRING(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, std::string);

	set_string(target, val);
}

void Fighter::ADD_POS(ScriptArg args) {
	UNWRAP(pos, glm::vec3);
	add_pos_validate(pos);
}

void Fighter::SET_POS(ScriptArg args) {
	UNWRAP(pos, glm::vec3);
	set_pos_validate(pos);
}

void Fighter::CHANGE_ANIM(ScriptArg args) {
	UNWRAP(anim_kind, std::string);
	float rate = 1.0f;
	UNWRAP_NO_DECL(rate);
	float frame = 0.0f;
	UNWRAP_NO_DECL(frame);
	change_anim(anim_kind, rate, frame);
}

void Fighter::CHANGE_SCRIPT(ScriptArg args) {
	UNWRAP(script_name, std::string);
	change_script(script_name);
}

void Fighter::PLAY_CAMERA_ANIM(ScriptArg args) {
	UNWRAP(anim_kind, std::string);
	UNWRAP(anim_rate, float);
	UNWRAP(anim_frame, float);
	play_camera_anim(anim_kind, anim_rate, anim_frame);
}

void Fighter::STOP_CAMERA_ANIM(ScriptArg args) {
	stop_camera_anim();
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

void Fighter::STOP_CINEMATIC_SEQUENCE(ScriptArg args) {
	stop_cinematic_sequence();
}

void Fighter::RESET_WORLD_RATE(ScriptArg args) {
	reset_world_rate();
}

void Fighter::ENABLE_CANCEL(ScriptArg args) {
	UNWRAP(move, std::string);
	UNWRAP(kind, CancelKind);
	enable_cancel(move, kind);
}

void Fighter::DISABLE_CANCEL(ScriptArg args) {
	UNWRAP(move, std::string);
	UNWRAP(kind, CancelKind);
	disable_cancel(move, kind);
}

void Fighter::DISABLE_ALL_CANCELS(ScriptArg args) {
	disable_all_cancels();
}

void Fighter::CHANGE_STATUS(ScriptArg args) {
	UNWRAP(new_status_kind, int);
	bool call_end_status = true;
	UNWRAP_NO_DECL(call_end_status);
	change_status(new_status_kind, call_end_status);
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

	set_projectile_float(projectile, target, val);
}

void Fighter::SET_PROJECTILE_FLAG(ScriptArg args) {
	UNWRAP(projectile, int);
	UNWRAP(target, int);
	UNWRAP(val, bool);

	set_projectile_flag(projectile, target, val);
}

void Fighter::SET_PROJECTILE_STRING(ScriptArg args) {
	UNWRAP(projectile, int);
	UNWRAP(target, int);
	UNWRAP(val, std::string);

	set_projectile_string(projectile, target, val);
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
	change_projectile_status(projectile, new_status_kind, call_end_status);
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

void Fighter::GRAB_OPPONENT(ScriptArg args) {
	UNWRAP(attacker_bone_name, std::string);
	UNWRAP(defender_bone_name, std::string);
	UNWRAP(offset, glm::vec3);
	grab_opponent(attacker_bone_name, defender_bone_name, offset);
}