#pragma once
#include "FighterInstance.fwd.h"
#include "FighterInstance.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "CharaTemplate.fwd.h"
#include "utils.h"

class CharaTemplate : public FighterInstance {
public:
	virtual void chara_id();
	CharaTemplate();
	CharaTemplate(SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor);
	void loadCharaTemplateStatusFunctions();
	void loadCharaTemplateACMD();

	int chara_template_int[CHARA_CHARA_TEMPLATE_INT_MAX];
	float chara_template_float[CHARA_CHARA_TEMPLATE_FLOAT_MAX];
	bool chara_template_flag[CHARA_CHARA_TEMPLATE_FLAG_MAX];

	bool specific_ground_status_act() override;
	bool specific_status_attack() override;

	void chara_template_status_template() override;
	void chara_template_enter_status_template() override;
	void chara_template_exit_status_template() override;
};