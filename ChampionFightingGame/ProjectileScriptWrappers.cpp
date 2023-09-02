#include "Projectile.h"

void Projectile::SET_INT(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, int);

	set_int(target, val);
}

void Projectile::SET_FLOAT(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, float);

	set_float(target, val);
}

void Projectile::SET_FLAG(ScriptArg args) {
	UNWRAP(target, int);
	UNWRAP(val, bool);

	set_flag(target, val);
}

void Projectile::ADD_POS(ScriptArg args) {
	UNWRAP(pos, glm::vec3);
	add_pos(pos);
}

void Projectile::SET_POS(ScriptArg args) {
	UNWRAP(pos, glm::vec3);
	set_pos(pos);
}

void Projectile::CHANGE_STATUS(ScriptArg args) {
	UNWRAP(new_status_kind, int);
	bool call_end_status = true;
	UNWRAP_NO_DECL(call_end_status);
	bool require_different_status = true;
	UNWRAP_NO_DECL(require_different_status);
	change_status(new_status_kind, call_end_status, require_different_status);
}