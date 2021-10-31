#include "ProjectileTemplate.h"
#include "Game.h"

ProjectileTemplate::ProjectileTemplate(SDL_Renderer * renderer, int id, FighterInstanceAccessor * fighter_instance_accessor) {
	resource_dir = "resource/projectile/template";
	this->projectile_kind = PROJECTILE_KIND_PROJECTILE_TEMPLATE;
	load_params();
	loadProjectileTemplateACMD();
	loadProjectileTemplateStatusFunctions();
	this->base_texture = loadTexture("resource/projectile/template/sprite/sprite.png", renderer);
	this->fighter_instance_accessor = fighter_instance_accessor;
	superInit(renderer);
}

void ProjectileTemplate::loadProjectileTemplateStatusFunctions() {
	pStatus[PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE] = &ProjectileInstance::projectile_template_status_template;
	pEnter_status[PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE] = &ProjectileInstance::projectile_template_enter_status_template;
	pExit_status[PROJECTILE_PROJECTILE_TEMPLATE_STATUS_TEMPLATE] = &ProjectileInstance::projectile_template_exit_status_template;
}

void ProjectileTemplate::loadProjectileTemplateACMD() {
	script("default", [this]() {
		return;
	});
}

void ProjectileTemplate::tickOnceProjectileUnique() {

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