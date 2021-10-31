#include "Eric.h"
#include "utils.h"
#include "Game.h"
#include "EricFireball.fwd.h"
#include "EricFireball.h"

EricScript::EricScript() {};

EricScript::EricScript(string name, function<void()> move_script, int id) {
	this->name = name;
	this->eric_script = move_script;
	this->id = id;
}

Eric::Eric() {
	
}

Eric::Eric(SDL_Renderer *renderer, int id, FighterInstanceAccessor *fighter_instance_accessor) {
	resource_dir = "resource/chara/eric";
	load_unique_params();
	loadEricACMD();
	loadEricStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_ERIC;
	this->base_texture = loadTexture("resource/chara/eric/sprite/sprite.png", renderer);

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectile_objects[i] = new ProjectileInstance();
	}

	projectile_instances[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_ERIC_FIREBALL, renderer, id, fighter_instance_accessor);
	delete (projectile_objects[0]);
	this->projectile_objects[0] = projectile_instances[0]->get_projectile();
	EricFireball* eric_fireball_instance = (EricFireball*)projectile_objects[0];
	eric_fireball_instance->eric = this;
}

void Eric::chara_id() {
/*
▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░▒▒░░▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░▒▒░░▒▒▒▒░░▒▒░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░▒▒▒▒▒▒░░▒▒░░░░░░▒▒░░▒▒░░▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▒▒░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▒▒▒▒░░▒▒▒▒▒▒░░░░▒▒▒▒▓▓░░▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░▒▒▒▒░░▒▒▒▒░░░░▒▒▒▒░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒░░░░░░░░░░░░▓▓▓▓▓▓░░▒▒░░░░░░░░▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░░░▒▒▒▒▒▒░░▒▒░░▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒
░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒░░░░░░▒▒        ░░        ░░░░░░░░░░░░░░▒▒░░░░▒▒░░▒▒░░▒▒░░░░░░▒▒░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░▒▒░░▒▒▒▒░░▒▒░░░░░░▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒░░░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒▓▓▒▒░░  ░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░▒▒░░░░▒▒▒▒░░▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒
░░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒░░░░▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░▒▒░░░░░░░░▒▒░░░░░░░░░░▒▒▒▒░░
▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓░░▒▒░░░░▒▒░░░░░░▒▒░░░░░░▒▒░░░░░░░░░░░░▒▒▒▒▓▓▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒░░  ░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░▒▒▒▒
░░░░░░▒▒▓▓▓▓▓▓▓▓▒▒░░░░▒▒▒▒▒▒▒▒░░░░▒▒░░░░░░▒▒▒▒░░░░▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▓▓▒▒▓▓▒▒▒▒░░░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░▒▒▒▒▒▒░░░░░░░░░░░░▒▒▒▒▒▒░░▒▒▒▒▒▒
▒▒░░▒▒▓▓▓▓▓▓▓▓▒▒▒▒░░░░▒▒░░▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒░░▒▒▓▓▓▓▓▓▒▒▓▓▒▒▒▒▓▓▓▓▒▒▒▒░░▒▒▒▒▓▓▒▒▒▒░░░░    ░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░▒▒░░░░░░▒▒░░░░░░▒▒▒▒▒▒▒▒
▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▓▓▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒░░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░▒▒▒▒░░░░▒▒
▒▒▒▒▓▓▓▓▓▓▓▓▓▓░░░░▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▓▓▒▒▒▒░░░░░░░░▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒░░▒▒▒▒
▒▒▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒░░░░▒▒▒▒▒▒▒▒░░▒▒▒▒░░░░░░░░▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒░░░░░░░░░░░░▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒
▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒░░▒▒▒▒░░▒▒▒▒▒▒▒▒░░░░▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▒▒▒▒▒▒░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒░░▒▒░░▒▒▒▒▒▒░░░░░░░░▒▒░░░░░░  ░░░░░░░░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒░░░░▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██░░  ░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░▒▒░░░░░░░░▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██░░      ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒░░░░▒▒░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░  ░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒░░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▓▓▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░    ░░░░▒▒░░░░░░▒▒▒▒▒▒▒▒░░░░░░░░▒▒░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒░░░░▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒  ░░░░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░    ░░░░░░░░░░▒▒▒▒▒▒▒▒░░  ░░░░▒▒░░░░▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒░░░░░░▒▒░░▒▒░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░    ░░░░░░░░▒▒▒▒▒▒▒▒▒▒░░░░░░░░▒▒░░▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░    ░░░░░░░░▒▒▒▒▒▒▓▓▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒▓▓▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒░░  ░░░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░    ░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░      ▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░      ░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒░░    ░░  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░    ░░░░░░▒▒▒▒▒▒▓▓▒▒▒▒▓▓░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░    ░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░  ░░░░░░░░▒▒▒▒▒▒▒▒▓▓░░▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒░░░░▒▒▒▒▒▒▒▒░░▒▒▓▓▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒░░░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓██░░  ░░  ░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓░░    ░░░░░░▒▒▒▒▒▒▒▒▒▒░░░░░░░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒░░░░░░░░░░░░▒▒▓▓▒▒▒▒▓▓▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░▒▒░░▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒░░      ░░░░▒▒▒▒▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒░░░░▒▒▒▒░░██▒▒▒▒▒▒▒▒▓▓▒▒░░▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓░░  ░░░░▒▒░░▒▒▒▒░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▓▓▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒▒▒▓▓▓▓▒▒▒▒▓▓▒▒▓▓▒▒▓▓▓▓▓▓▒▒▒▒░░░░  ░░░░▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓
▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒    ░░  ░░░░░░░░░░░░  ░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▓▓▓▓▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▒▒▒▒░░▒▒▒▒░░░░    ░░░░▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒░░░░    ░░    ░░░░  ░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒░░▒▒▒▒░░░░░░░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒▒▒              ░░  ░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒░░░░░░░░  ░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓░░          ░░░░░░  ░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒░░░░  ░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒░░░░░░▒▒▓▓▓▓▒▒    ░░░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░    ░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓██▓▓▓▓▓▓▓▓████▒▒░░  ░░░░▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░  ░░▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓████████████░░░░  ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒░░▒▒░░░░    ░░▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓████████████░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░      ▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒░░░░░░▒▒░░▒▒▒▒▒▒▒▒░░
▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░  ░░░░▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒░░░░▒▒░░▒▒░░░░▒▒▒▒▒▒
▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒░░▒▒
▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓░░░░    ░░░░▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒░░▒▒░░░░░░  ░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░▒▒░░▒▒▒▒▒▒░░░░▒▒░░░░▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒░░░░▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░      ░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒
▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓░░░░  ░░░░░░░░░░  ░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░      ░░▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒░░░░        ░░░░░░      ░░  ░░░░░░░░  ░░░░░░░░░░▒▒░░░░░░▒▒░░▒▒▒▒░░▒▒░░░░░░          ░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒
▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓░░░░░░░░░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░    ░░░░░░░░░░░░░░▒▒░░░░░░          ░░▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓
▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓████████▓▓██▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░▒▒░░░░░░░░░░░░░░░░░░░░          ░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓████████████████████████████████████████████████▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░  ░░░░░░▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓
▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓████████████████████████████████████████████████████████████████▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓████████████████████████████████████████████████████████████████████████████████▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓████████████████████████████████████████████████████████████████████████████▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓
▒▒▓▓▓▓▓▓▓▓▓▓██▒▒▒▒▒▒░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒
▒▒▓▓▓▓▓▓▓▓▓▓▓▓██▒▒▒▒░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒
▓▓▓▓▓▓▓▓▓▓▓▓▓▓██▓▓▒▒░░░░░░░░░░░░░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▓▓████▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▓▓▒▒▒▒▒▒
*/
}

void Eric::loadEricStatusFunctions() {

}

void Eric::loadEricACMD() {
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 3;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("walk_f", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("walk_b", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("dash_f", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("dash_b", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("crouch_d", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("crouch", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("crouch_u", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("jump_squat", [this]() {

	});
	script("jump", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -25, 20 }, GameCoordinate{ 20, 120 });
			new_hurtbox(1, GameCoordinate{ -35, 75 }, GameCoordinate{ 35, 115 });
		}
	});
	script("jump_f", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 40 }, GameCoordinate{ 35, 105 });
		}
	});
	script("jump_b", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 40 }, GameCoordinate{ 35, 105 });
		}
	});
	script("fall", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 120 });
			new_hurtbox(1, GameCoordinate{ -35, 75 }, GameCoordinate{ 35, 115 });
		}
	});
	script("stand_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("high_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("crouch_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("jump_block", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("parry_start", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 4;
		}
	});
	script("parry_start_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 4;
		}
	});
	script("parry_mid", [this]() {

	});
	script("parry_high", [this]() {

	});
	script("parry_low", [this]() {

	});
	script("parry_air", [this]() {

	});
	script("hitstun_parry", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 5;
		}
	});
	script("hitstun_parry_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 5;
		}
	});
	script("stand_lp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
			new_hitbox(0, 0, 30, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 0.0, 0.0, 0.0, 0.0, false);
		}
		if (is_excute_frame(4)) {
			new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ 35,70 }, GameCoordinate{ 90, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
		}
	});
	script("stand_mp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
			new_hitbox(0, 0, 40, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 0.0, 0.0, 0.0, 0.0, false);
		}
		if (is_excute_frame(8)) {
			new_hitbox(1, 0, 40, 5, 1.2, 1, GameCoordinate{ 35,70 }, GameCoordinate{ 90, 90 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
		}
	});
	script("stand_hp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
			new_hitbox(0, 0, 50, 5, 1.2, 1, GameCoordinate{ 5,70 }, GameCoordinate{ 130, 90 }, HITBOX_KIND_BLOCK, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 0.0, 0.0, 0.0, 0.0, true);
		}
		if (is_excute_frame(10)) {
			new_hitbox(1, 0, 50, 5, 1.2, 1, GameCoordinate{ 25,70 }, GameCoordinate{ 90, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, true);
		}
		if (is_excute_wait(2)) {
			new_hitbox(2, 0, 60, 10, 1.2, 1, GameCoordinate{ 80,70 }, GameCoordinate{ 100, 100 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 20, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_CRUMPLE, HIT_STATUS_CRUMPLE, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, true);
		}
		if (is_excute_wait(3)) {
			clear_hitbox_all();
		}
	});
	script("stand_lk", [this]() {

	});
	script("stand_mk", [this]() {

	});
	script("stand_hk", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 0, 90 });
		}
		if (is_excute_frame(2)) {
			new_hurtbox(1, GameCoordinate{ -15, 100 }, GameCoordinate{ 25, 60 });
		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 0, 50, 10, 1.2, 1, GameCoordinate{ 25,60 }, GameCoordinate{ 60, 120 }, HITBOX_KIND_NORMAL, 20, 40, 10, SITUATION_HIT_GROUND_AIR, 4, 15, 12, 70, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 3, 3, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 20, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_AERIAL, 20.0, 0.0, 2.0, 1.0, true);
		}
		if (is_excute_frame(8)) {
			clear_hitbox_all();
		}
		if (is_excute_frame(12)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("crouch_lp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -38, 0 }, GameCoordinate{ 45, 71 });
		}
		if (is_excute_frame(4)) {
			new_hurtbox(1, GameCoordinate{ 21, 43 }, GameCoordinate{ 68, 64 });
			new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ 42,43 }, GameCoordinate{ 76, 64 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
		}
		if (is_excute_wait(2)) {
			clear_hitbox_all();
		}
		if (is_excute_wait(2)) {
			clear_hurtbox(1);
		}
	});
	script("crouch_mp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -38, 0 }, GameCoordinate{ 45, 71 });
		}
		if (is_excute_frame(4)) {
			new_hurtbox(1, GameCoordinate{ 21, 43 }, GameCoordinate{ 68, 64 });
			new_hitbox(1, 0, 40, 5, 1.2, 1, GameCoordinate{ 42,43 }, GameCoordinate{ 76, 64 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 6, 6, 4, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, false);
		}
		if (is_excute_wait(2)) {
			clear_hitbox_all();
		}
		if (is_excute_wait(2)) {
			clear_hurtbox(1);
		}
	});
	script("crouch_hp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -45, 0 }, GameCoordinate{ 45, 25 });
			new_hurtbox(1, GameCoordinate{ -30, 20 }, GameCoordinate{ 25, 73 });
		}
		if (is_excute_frame(6)) {
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 40, 55 });
		}
		if (is_excute_wait(2)) {
			new_hitbox(1, 0, 50, 5, 1.2, 1, GameCoordinate{ 25,55 }, GameCoordinate{ -10, 140 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 15, 12, 7, false, ATTACK_HEIGHT_MID, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_NORMAL, HIT_STATUS_LAUNCH, COUNTERHIT_TYPE_AERIAL, 14.0, 0.0, 0.0, 1.0, true);
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
		}
	});
	script("crouch_lk", [this]() {

	});
	script("crouch_mk", [this]() {

	});
	script("crouch_hk", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
		}
		if (is_excute_frame(3)) {

		}
		if (is_excute_frame(6)) {
			new_hitbox(1, 0, 50, 5, 1.2, 1, GameCoordinate{ -5,35 }, GameCoordinate{ 90, 0 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 20, 15, 13, 10, false, ATTACK_HEIGHT_LOW, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 4, HIT_STATUS_KNOCKDOWN, HIT_STATUS_KNOCKDOWN, COUNTERHIT_TYPE_NORMAL, 10.0, 0.0, 0.0, 1.0, true);
		}

	});
	script("jump_lp", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -45, 0 }, GameCoordinate{ 0, 50 });
			new_hurtbox(1, GameCoordinate{ -15, 10 }, GameCoordinate{ 15, 100 });
			new_hurtbox(2, GameCoordinate{ 10, 40 }, GameCoordinate{ 35, 100 });
		}
		if (is_excute_frame(2)) {
			new_hurtbox(3, GameCoordinate{ 5, 55 }, GameCoordinate{ 40, 95 });
		}
		if (is_excute_wait(1)) {
			max_ticks = 2;
			new_hurtbox(1, GameCoordinate{ -15, 10 }, GameCoordinate{ 15, 75 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 75 });
			new_hurtbox(3, GameCoordinate{ 5, 25 }, GameCoordinate{ 45, 75 });
			new_hitbox(1, 0, 30, 5, 1.2, 1, GameCoordinate{ 20,20 }, GameCoordinate{ 50, 60 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 8, 8, 6, 4, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_LIGHT, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 3, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 4.0, false);
		}
	});
	script("jump_mp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -55, 15 }, GameCoordinate{ 0, 100 });
			new_hurtbox(1, GameCoordinate{ -10, 25 }, GameCoordinate{ 27, 80 });
			new_hurtbox(2, GameCoordinate{ -65, 70 }, GameCoordinate{ 5, 100 });
		}
		if (is_excute_wait(4)) {
			new_hurtbox(1, GameCoordinate{ -10, 25 }, GameCoordinate{ 23, 50 });
			new_hurtbox(2, GameCoordinate{ -45, 70 }, GameCoordinate{ 5, 100 });
		}
		if (is_excute_wait(2)) {
			new_hurtbox(1, GameCoordinate{ 0, 50 }, GameCoordinate{ 40, 80 });
			new_hurtbox(3, GameCoordinate{ 15, 45 }, GameCoordinate{ 55, 70 });
			new_hitbox(1, 0, 30, 15, 1.2, 1, GameCoordinate{ 45,35 }, GameCoordinate{ 75, 55 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 12, 8, 8, 6, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_MEDIUM, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 3, 5, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 4.0, false);
		}
		if (is_excute_wait(2)) {
			clear_hitbox_all();
		}
		if (is_excute_frame(13)) {
			clear_hurtbox(3);
		}
	});
	script("jump_hp", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -40, 25 }, GameCoordinate{ 40, 65 });
			new_hurtbox(1, GameCoordinate{ -30, 30 }, GameCoordinate{ 30, 110 });
		}
		if (is_excute_frame(6)) {
			new_hurtbox(1, GameCoordinate{ -10, 30 }, GameCoordinate{ 30, 110 });
			new_hurtbox(2, GameCoordinate{ -25, 75 }, GameCoordinate{ 75, 95 });
			new_hitbox(1, 0, 60, 15, 1.2, 1, GameCoordinate{ 60,75 }, GameCoordinate{ 95, 95 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 10, 8, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 7, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 10.0, false);
			new_hitbox(2, 0, 50, 15, 1.2, 1, GameCoordinate{ 20,75 }, GameCoordinate{ 95, 95 }, HITBOX_KIND_NORMAL, 15, 30, 10, SITUATION_HIT_GROUND_AIR, 16, 10, 10, 8, false, ATTACK_HEIGHT_HIGH, ATTACK_LEVEL_HEAVY, 10, 10, CLANK_KIND_NORMAL, chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS], 1, 7, HIT_STATUS_NORMAL, HIT_STATUS_NORMAL, COUNTERHIT_TYPE_NONE, 7.0, 0.0, 0.0, 10.0, false);
		}
		if (is_excute_wait(5)) {
			clear_hitbox(1);
		}
		if (is_excute_wait(4)) {
			clear_hitbox_all();
			clear_hurtbox(2);
		}
	});
	script("jump_lk", [this]() {

	});
	script("jump_mk", [this]() {

	});
	script("jump_hk", [this]() {

	});
	script("grab", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
		if (is_excute_frame(3)) {
			new_grabbox(0, GameCoordinate{ 15, 55 }, GameCoordinate{ 70, 100 }, GRABBOX_KIND_NORMAL, SITUATION_HIT_GROUND_AIR, CHARA_STATUS_THROW, CHARA_STATUS_GRABBED);
		}
	});
	script("throw_f", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = -60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 5);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(13)) {
			damage_opponent(30.0, 1, 15.0, 8.0);
			change_opponent_status(CHARA_STATUS_THROWN);
		}
	});
	script("throw_b", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = 60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 20);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(15)) {
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = -60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 7);
		}
		if (is_excute_frame(20)) {
			damage_opponent(30.0, -1, -18.0, 8.0);
			change_opponent_status(CHARA_STATUS_THROWN);
		}
	});
	script("grab_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
		if (is_excute_frame(3)) {
			new_grabbox(0, GameCoordinate{ 15, 55 }, GameCoordinate{ 70, 100 }, GRABBOX_KIND_NORMAL, SITUATION_HIT_GROUND_AIR, CHARA_STATUS_THROW_AIR, CHARA_STATUS_GRABBED);
		}
	});
	script("throw_f_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = -60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 5);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(13)) {
			damage_opponent(30.0, 1, 15.0, 8.0);
			change_opponent_status(CHARA_STATUS_THROWN);
		}
	});
	script("throw_b_air", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = 60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 20);
			change_opponent_anim("stand_hitstun_m", 2);
			set_opponent_thrown_ticks();
		}
		if (is_excute_frame(15)) {
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X] = -60;
			chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] = 0;
			set_opponent_offset(GameCoordinate{ chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_X], chara_float[CHARA_FLOAT_MANUAL_POS_OFFSET_Y] }, 7);
		}
		if (is_excute_frame(20)) {
			damage_opponent(30.0, -1, -18.0, 8.0);
			change_opponent_status(CHARA_STATUS_THROWN);
		}
	});
	script("stand_hitstun_l", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("stand_hitstun_m", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("stand_hitstun_h", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("crouch_hitstun_l", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("crouch_hitstun_m", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("crouch_hitstun_h", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("jump_hitstun", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("launch_start", [this]() {

	});
	script("launch", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 70 });
		}
	});
	script("knockdown_start", [this]() {

	});
	script("knockdown_wait", [this]() {

	});
	script("wakeup", [this]() {
		if (is_excute_frame(0)) {
			max_ticks = 2;
		}
	});
	script("crumple", [this]() {

	});
	script("landing", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
	script("landing_hitstun", [this]() {
		if (is_excute_frame(0)) {
			new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
			new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
			new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
		}
	});
}

bool Eric::specific_ground_status_act() {
	return false;
}

bool Eric::specific_air_status_act() {
	return false;
}

bool Eric::specific_status_attack() {
	if (chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (situation_kind == CHARA_SITUATION_GROUND && specific_ground_status_act()) {
			return true;
		}
		else if (situation_kind == CHARA_SITUATION_AIR && specific_air_status_act()) {
			return true;
		}
		else if (chara_int[CHARA_INT_HITLAG_FRAMES] == 0) {
			if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_LP && check_button_input(BUTTON_MP)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_MP;
				if (situation_kind == CHARA_SITUATION_GROUND) {
					return change_status(CHARA_STATUS_ATTACK, true, false);
				}
				else {
					return change_status(CHARA_STATUS_ATTACK_AIR, true, false);
				}
			}
			if (chara_int[CHARA_INT_ATTACK_KIND] == ATTACK_KIND_MP && check_button_input(BUTTON_HP)) {
				chara_int[CHARA_INT_ATTACK_KIND] = ATTACK_KIND_HP;
				if (situation_kind == CHARA_SITUATION_GROUND) {
					return change_status(CHARA_STATUS_ATTACK, true, false);
				}
				else {
					return change_status(CHARA_STATUS_ATTACK_AIR, true, false);
				}
			}
		}
	}
	return false;
}

void Eric::enter_status_dash() {
	change_anim("dash_f", 2);
	chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = true;
	new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
	new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
	new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
}

void Eric::exit_status_dash() {
	chara_flag[CHARA_FLAG_DASH_CANCEL] = false;
	chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = false;
}

void Eric::enter_status_dashb() {
	change_anim("dash_b", 2);
	chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = true;
	new_hurtbox(0, GameCoordinate{ -35, 0 }, GameCoordinate{ 37, 35 });
	new_hurtbox(1, GameCoordinate{ -25, 0 }, GameCoordinate{ 20, 110 });
	new_hurtbox(2, GameCoordinate{ -15, 55 }, GameCoordinate{ 35, 95 });
}

void Eric::exit_status_dashb() {
	chara_flag[CHARA_FLAG_DASH_CANCEL] = false;
	chara_flag[CHARA_FLAG_ALLOW_GROUND_CROSSUP] = false;
}