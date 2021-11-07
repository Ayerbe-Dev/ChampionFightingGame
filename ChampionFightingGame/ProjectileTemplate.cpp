#include "ProjectileTemplate.h"
#include "Game.h"

ProjectileTemplate::ProjectileTemplate(int id, PlayerInfo* player_info, FighterAccessor * fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/projectile/template";
	this->projectile_kind = PROJECTILE_KIND_PROJECTILE_TEMPLATE;
	load_params();
	loadProjectileTemplateACMD();
	loadProjectileTemplateStatusFunctions();
	this->base_texture = loadTexture("resource/projectile/template/sprite/sprite.png");
	this->fighter_accessor = fighter_accessor;
	superInit();
}

void ProjectileTemplate::loadProjectileTemplateStatusFunctions() {
	projectile_template_status[PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE - PROJECTILE_STATUS_MAX] = &ProjectileTemplate::projectile_template_status_template;
	projectile_template_enter_status[PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE - PROJECTILE_STATUS_MAX] = &ProjectileTemplate::projectile_template_enter_status_template;
	projectile_template_exit_status[PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE - PROJECTILE_STATUS_MAX] = &ProjectileTemplate::projectile_template_exit_status_template;
}

void ProjectileTemplate::loadProjectileTemplateACMD() {
	script("default", [this]() {
		return;
	});
}

void ProjectileTemplate::tickOnceProjectileUnique() {

}

void ProjectileTemplate::projectile_unique_status() {
	(this->*projectile_template_status[status_kind - PROJECTILE_STATUS_MAX])();
}

void ProjectileTemplate::projectile_unique_enter_status() {
	(this->*projectile_template_enter_status[status_kind - PROJECTILE_STATUS_MAX])();
}

void ProjectileTemplate::projectile_unique_exit_status() {
	(this->*projectile_template_exit_status[status_kind - PROJECTILE_STATUS_MAX])();
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