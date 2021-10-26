#include "CharaTemplate.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

CharaTemplate::CharaTemplate() {

}

CharaTemplate::CharaTemplate(SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor) {
	resource_dir = "resource/chara/template";
	superInit(id, renderer);
	load_unique_params();
	loadCharaTemplateACMD();
	loadCharaTemplateStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_CHARA_TEMPLATE;
	this->base_texture = loadTexture("resource/chara/template/sprite/sprite.png", renderer);

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectile_objects[i] = new ProjectileInstance();
	}

	IObject* projectile_template = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, renderer, id, fighter_instance_accessor);
	this->projectile_objects[0] = projectile_template->get_projectile();
	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectile_objects[0];
	projectile_template_instance->chara_template = this;
}

void CharaTemplate::chara_id() {

}

void CharaTemplate::loadCharaTemplateStatusFunctions() {
	pStatus[CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE] = &FighterInstance::chara_template_status_template;
	pEnter_status[CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE] = &FighterInstance::chara_template_enter_status_template;
	pExit_status[CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE] = &FighterInstance::chara_template_exit_status_template;
}

void CharaTemplate::loadCharaTemplateACMD() { //todo: Fill this in with all of the common empty scripts
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {

	});
	script("stand_lp", [this]() {

	});
	script("stand_mp", [this]() {

	});
}

bool CharaTemplate::specific_ground_status_act() {
	if (get_special_input(SPECIAL_KIND_236, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_EX;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}
	if (get_special_input(SPECIAL_KIND_236, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}

	if (get_special_input(SPECIAL_KIND_623, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_EX;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}
	if (get_special_input(SPECIAL_KIND_623, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}

	if (get_special_input(SPECIAL_KIND_CHARGE_DOWN, BUTTON_MACRO_P) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_EX;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}
	if (get_special_input(SPECIAL_KIND_CHARGE_DOWN, BUTTON_LP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_L;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}
	if (get_special_input(SPECIAL_KIND_CHARGE_DOWN, BUTTON_MP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_M;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}
	if (get_special_input(SPECIAL_KIND_CHARGE_DOWN, BUTTON_HP) != SPECIAL_INPUT_NONE) {
		chara_int[CHARA_INT_SPECIAL_LEVEL] = SPECIAL_LEVEL_H;
		return change_status(CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE);
	}

	//etc.

	return false;
}

bool CharaTemplate::specific_status_attack() {
	if (chara_flag[CHARA_FLAG_ATTACK_CONNECTED_DURING_STATUS]) {
		if (specific_ground_status_act()) {
			return true;
		}
	}
	return false;
}

void CharaTemplate::chara_template_status_template() {

}

void CharaTemplate::chara_template_enter_status_template() {

}

void CharaTemplate::chara_template_exit_status_template() {

}