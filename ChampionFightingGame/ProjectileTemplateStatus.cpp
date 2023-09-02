#include "ProjectileTemplate.h"
#include "ProjectileTemplateConstants.h"

void ProjectileTemplate::projectile_unique_main() {

}

void ProjectileTemplate::status_activate() {

}

void ProjectileTemplate::projectile_template_status_template() {

}

void ProjectileTemplate::projectile_template_enter_status_template() {

}

void ProjectileTemplate::projectile_template_exit_status_template() {

}

void ProjectileTemplate::load_projectile_unique_status_scripts() {
	status_script.resize(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_MAX, nullptr);
	enter_status_script.resize(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_MAX, nullptr);
	exit_status_script.resize(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_MAX, nullptr);

	ADD_PROJECTILE_STATUS(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE, &ProjectileTemplate::projectile_template_status_template);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE, &ProjectileTemplate::projectile_template_enter_status_template);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE, &ProjectileTemplate::projectile_template_exit_status_template);
}