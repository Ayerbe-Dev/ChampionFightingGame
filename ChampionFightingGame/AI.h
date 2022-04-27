#pragma once
#include "AIMoveList.h"
#include "CircularBuffer.h"
#include <iostream>
#include <vector>

struct AIInfo;

class Fighter;

class AI {
public:
	AI();
	AI(int id, int judgement, int precision, int execution, int reaction);

	void init(int id, int judgement, int precision, int execution, int reaction);

	void ai_main();

	bool is_in_range(AIMoveList& move); //Check if you either are or will be in range to use this move
	bool fast_enough(AIMoveList& move); //Check if the move will come out faster than any of the opponent's moves within this range
	bool they_wont_press_here(AIMoveList& move); //Calculate odds of the opponent not using a move that can stuff this one
	bool is_safe(AIMoveList& move); //Check if opponent can punish this on block
	bool will_hit(AIMoveList& move); //Calculate odds of the opponent not blocking this
	bool fuck_it(AIMoveList& move); //Randomly decide whether or not to go for a move despite it failing 
	//everything except the range check
	bool fuck_it_close_enough(AIMoveList& move); //Randomly decide whether or not to go for a move despite it
	//failing everything including the range check
	bool can_use(AIMoveList& move); //Check whether or not the user has charge for this move if needed, 
	//and has meter for this move if needed

	int judgement; //More or less just the % odds of fuck_it() returning true
	int precision; //The actual range of a given move will be offset by any number within this range
	int execution;
	int reaction;

	Fighter* fighter;
	int id;

	std::vector<AIMoveList> move_list;
	std::vector<AIMoveList> opponent_move_list;

	CircularBuffer<AIInfo> ai_info;
private:
	void load_chara_info(std::string chara_kind, std::vector<AIMoveList>& target);
	void sort_ai_movelist(std::vector<AIMoveList>& target);
};