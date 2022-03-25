#include "ProjectileTemplate.h"
#include "ProjectileTemplateConstants.h"

ProjectileTemplate::ProjectileTemplate() {}

ProjectileTemplate::ProjectileTemplate(int id, PlayerInfo* player_info) {
	this->player_info = player_info;
	this->projectile_kind = PROJECTILE_KIND_PROJECTILE_TEMPLATE;
	projectile_name = "projectile_template";
	resource_dir = "resource/projectile/projectile_template";
	projectile_int.resize(PROJECTILE_PROJECTILE_TEMPLATE_INT_MAX, 0);
	projectile_float.resize(PROJECTILE_PROJECTILE_TEMPLATE_FLOAT_MAX, 0.0);
	projectile_flag.resize(PROJECTILE_PROJECTILE_TEMPLATE_FLAG_MAX, false);
	load_params();
	load_move_scripts();
	loadProjectileTemplateStatusFunctions();
	superInit();
}

void ProjectileTemplate::loadProjectileTemplateStatusFunctions() {
	status_script.resize(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_MAX, nullptr);
	enter_status_script.resize(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_MAX, nullptr);
	exit_status_script.resize(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_MAX, nullptr);

	ADD_PROJECTILE_STATUS(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE, &ProjectileTemplate::projectile_template_status_template);
	ADD_PROJECTILE_ENTRY_STATUS(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE, &ProjectileTemplate::projectile_template_enter_status_template);
	ADD_PROJECTILE_EXIT_STATUS(PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE, &ProjectileTemplate::projectile_template_exit_status_template);
}

void ProjectileTemplate::load_move_scripts() {
	script("default", [this]() {
		return;
	});
}

void ProjectileTemplate::projectile_unique_main() {

}
void ProjectileTemplate::status_default() {
	
}

void ProjectileTemplate::status_hit() {
	
}

void ProjectileTemplate::enter_status_hit() {
	
}

void ProjectileTemplate::projectile_template_status_template() {

}

void ProjectileTemplate::projectile_template_enter_status_template() {

}

void ProjectileTemplate::projectile_template_exit_status_template() {

}