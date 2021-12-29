#include "Angelica.h"
#include "Battle.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

Angelica::Angelica() {

}

Angelica::Angelica(int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	this->player_info = player_info;
	resource_dir = "resource/chara/angelica";
	chara_name = "angelica";
	if (!crash_to_debug) {
		load_params();
	}
	loadCharaMoveScripts();
	loadAngelicaStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_CHARA_TEMPLATE;
	this->base_texture = loadSDLTexture("resource/chara/angelica/sprite/sprite.png");

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i] = new Projectile();
	}

//	projectile_objects[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, id, player_info, fighter_accessor);
//	delete (projectiles[0]);
//	this->projectiles[0] = projectile_objects[0]->get_projectile();
//	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectiles[0];
//	projectile_template_instance->angelica = this;
}

void Angelica::chara_id() {

}

void Angelica::chara_status() {
	(this->*angelica_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Angelica::chara_enter_status() {
	(this->*angelica_enter_status[status_kind - FIGHTER_STATUS_MAX])();
}

void Angelica::chara_exit_status() {
	(this->*angelica_exit_status[status_kind - FIGHTER_STATUS_MAX])();
}