#include "Fighter.h"

void Fighter::NEW_HURTBOX(ScriptArg args) {
	UNWRAP(id, int);
	UNWRAP(anchor, glm::vec2);
	UNWRAP(offset, glm::vec2);

	int hurtbox_kind = HURTBOX_KIND_NORMAL;
	bool armor = false;
	int intangible_kind = INTANGIBLE_KIND_NONE;

	if (args.num_args > 3) {
		UNWRAP_NO_DECL(hurtbox_kind, int);
		if (args.num_args > 4) {
			UNWRAP_NO_DECL(armor, bool);
			if (args.num_args > 5) {
				UNWRAP_NO_DECL(intangible_kind, int);
			}
		}
	}
	new_hurtbox(id, anchor, offset, hurtbox_kind, armor, intangible_kind);
}