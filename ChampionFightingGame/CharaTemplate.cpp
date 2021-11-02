#include "CharaTemplate.h"
#include "Game.h"
#include "ProjectileTemplate.fwd.h"
#include "ProjectileTemplate.h"

CharaTemplate::CharaTemplate() {

}

CharaTemplate::CharaTemplate(SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor) {
	resource_dir = "resource/chara/template";
	superInit(id, renderer);
	load_params();
	loadCharaTemplateACMD();
	loadCharaTemplateStatusFunctions();
	set_current_move_script("default");
	this->chara_kind = CHARA_KIND_CHARA_TEMPLATE;
	this->base_texture = loadTexture("resource/chara/template/sprite/sprite.png", renderer);

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectile_objects[i] = new ProjectileInstance();
	}

	projectile_instances[0] = new IObject(OBJECT_TYPE_PROJECTILE, PROJECTILE_KIND_PROJECTILE_TEMPLATE, renderer, id, fighter_instance_accessor);
	delete (projectile_objects[0]);
	this->projectile_objects[0] = projectile_instances[0]->get_projectile();
	ProjectileTemplate* projectile_template_instance = (ProjectileTemplate*)projectile_objects[0];
	projectile_template_instance->chara_template = this;
}

void CharaTemplate::chara_id() {

}

void CharaTemplate::loadCharaTemplateStatusFunctions() {
	chara_template_status[CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE - CHARA_STATUS_MAX] = &CharaTemplate::chara_template_status_template;
	chara_template_enter_status[CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE - CHARA_STATUS_MAX] = &CharaTemplate::chara_template_enter_status_template;
	chara_template_exit_status[CHARA_CHARA_TEMPLATE_STATUS_TEMPLATE - CHARA_STATUS_MAX] = &CharaTemplate::chara_template_exit_status_template;
}

void CharaTemplate::loadCharaTemplateACMD() { //todo: Fill this in with all of the common empty scripts
	script("default", [this]() {
		return;
	});
	script("wait", [this]() {

	});
	script("walk_f", [this]() {

	});
	script("walk_b", [this]() {

	});
	script("dash_f", [this]() {

	});
	script("dash_b", [this]() {

	});
	script("crouch_d", [this]() {

	});
	script("crouch", [this]() {

	});
	script("crouch_u", [this]() {

	});
	script("jump_squat", [this]() {

	});
	script("jump", [this]() {

	});
	script("jump_f", [this]() {

	});
	script("jump_b", [this]() {

	});
	script("fall", [this]() {

	});
	script("stand_block", [this]() {

	});
	script("high_block", [this]() {

	});
	script("crouch_block", [this]() {

	});
	script("jump_block", [this]() {

	});
	script("parry_start", [this]() {

	});
	script("parry_start_air", [this]() {

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

	});
	script("hitstun_parry_air", [this]() {

	});
	script("stand_lp", [this]() {

	});
	script("stand_mp", [this]() {

	});
	script("stand_hp", [this]() {

	});
	script("stand_lk", [this]() {

	});
	script("stand_mk", [this]() {

	});
	script("stand_hk", [this]() {

	});
	script("crouch_lp", [this]() {

	});
	script("crouch_mp", [this]() {

	});
	script("crouch_hp", [this]() {

	});
	script("crouch_lk", [this]() {

	});
	script("crouch_mk", [this]() {

	});
	script("crouch_hk", [this]() {

	});
	script("jump_lp", [this]() {

	});
	script("jump_mp", [this]() {

	});
	script("jump_hp", [this]() {

	});
	script("jump_lk", [this]() {

	});
	script("jump_mk", [this]() {

	});
	script("jump_hk", [this]() {

	});
	script("grab", [this]() {

	});
	script("throw_f", [this]() {

	});
	script("throw_b", [this]() {

	});
	script("grab_air", [this]() {

	});
	script("throw_f_air", [this]() {

	});
	script("throw_b_air", [this]() {

	});
	script("stand_hitstun_l", [this]() {

	});
	script("stand_hitstun_m", [this]() {

	});
	script("stand_hitstun_h", [this]() {

	});
	script("crouch_hitstun_l", [this]() {

	});
	script("crouch_hitstun_m", [this]() {

	});
	script("crouch_hitstun_h", [this]() {

	});
	script("jump_hitstun", [this]() {

	});
	script("launch_start", [this]() {

	});
	script("launch", [this]() {

	});
	script("knockdown_start", [this]() {

	});
	script("knockdown_wait", [this]() {

	});
	script("wakeup", [this]() {

	});
	script("crumple", [this]() {

	});
	script("landing", [this]() {

	});
	script("landing_hitstun", [this]() {

	});
}

void CharaTemplate::chara_main() {}

void CharaTemplate::chara_status() {
	(this->*chara_template_status[status_kind - CHARA_STATUS_MAX])();
}

void CharaTemplate::chara_enter_status() {
	(this->*chara_template_enter_status[status_kind - CHARA_STATUS_MAX])();
}

void CharaTemplate::chara_exit_status() {
	(this->*chara_template_exit_status[status_kind - CHARA_STATUS_MAX])();
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