#include "CharaTemplate.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

CharaTemplate::CharaTemplate() {

}

CharaTemplate::CharaTemplate(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/chara_template";
	chara_name = "chara_template";
	if (!crash_to_debug) {
		load_params();
	}
	loadCharaTemplateACMD();
	loadCharaTemplateStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_CHARA_TEMPLATE;
	this->base_texture = loadTexture("resource/chara/template/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->chara_template = this;
}

void CharaTemplate::chara_id() {

}

void CharaTemplate::chara_status() {
	(this->*chara_template_status[status_kind - FIGHTER_STATUS_MAX])();
}

void CharaTemplate::chara_enter_status() {
	(this->*chara_template_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void CharaTemplate::chara_exit_status() {
	(this->*chara_template_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}