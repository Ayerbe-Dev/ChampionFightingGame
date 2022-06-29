#include "ProjectileTemplate.h"
#include "ProjectileTemplateConstants.h"

void ProjectileTemplate::load_move_scripts() {
	script("default", [this]() {
		return;
	});
}