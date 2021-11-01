#pragma once
#include "FighterInstance.fwd.h"
#include "FighterInstance.h"
#include "ObjectInstance.fwd.h"
#include "ObjectInstance.h"
#include "Psychic.fwd.h"
#include "utils.h"

class Psychic : public FighterInstance {
public:
	virtual void chara_id();
	Psychic();
	Psychic(SDL_Renderer* renderer, int id, FighterInstanceAccessor* fighter_instance_accessor);
	void loadPsychicStatusFunctions();
	void loadPsychicACMD();

	int psychic_int[CHARA_PSYCHIC_INT_MAX];
	float psychic_float[CHARA_PSYCHIC_FLOAT_MAX];
	bool psychic_flag[CHARA_PSYCHIC_FLAG_MAX];

	void chara_main() override;

	bool specific_ground_status_act() override;
	bool specific_air_status_act() override;

	bool specific_status_attack() override;
};