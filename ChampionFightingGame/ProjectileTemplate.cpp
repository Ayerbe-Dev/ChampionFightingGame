#include "ProjectileTemplate.h"
#include "ProjectileTemplateConstants.h"

ProjectileTemplate::ProjectileTemplate() {}

ProjectileTemplate::ProjectileTemplate(int id, Player* player) {
	this->id = id;
	this->player = player;
	this->projectile_kind = PROJECTILE_KIND_PROJECTILE_TEMPLATE;
	projectile_name = "projectile_template";
	resource_dir = "resource/projectile/projectile_template";
	projectile_int.resize(PROJECTILE_PROJECTILE_TEMPLATE_INT_MAX, 0);
	projectile_float.resize(PROJECTILE_PROJECTILE_TEMPLATE_FLOAT_MAX, 0.0);
	projectile_flag.resize(PROJECTILE_PROJECTILE_TEMPLATE_FLAG_MAX, false);

	loadProjectileTemplateStatusFunctions();
	super_init();
}