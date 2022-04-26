#pragma once
#include "AIDecision.h"
#include "AIInput.h"
#include "AIMoveList.h"
#include <vector>

class Fighter;

class AI {
public:
	AI();
	AI(int id, int judgement, int precision, int execution, int reaction);

	void init(int id, int judgement, int precision, int execution, int reaction);
	
	void load_chara_info(std::string chara_kind, std::vector<AIMoveList>& target);

	Fighter* fighter;
	int id;

	AIDecision decisions;
	AIInput inputs;

	std::vector<AIMoveList> move_list;
	std::vector<AIMoveList> opponent_move_list;
};