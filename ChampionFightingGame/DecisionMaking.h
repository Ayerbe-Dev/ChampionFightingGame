#pragma once
#include <iostream>
#include <vector>
#include "MoveList.h"


class DecisionMaking {
public:
	DecisionMaking();

	bool is_in_range(MoveList& move); //Check if you either are or will be in range to use this move
	bool fast_enough(MoveList& move); //Check if the move will come out faster than any of the opponent's moves within this range
	bool they_wont_press_here(MoveList& move); //Calculate odds of the opponent not using a move that can stuff this one
	bool is_safe(MoveList& move); //Check if opponent can punish this on block
	bool will_hit(MoveList& move); //Calculate odds of the opponent not blocking this
	bool fuck_it(MoveList& move); //Randomly decide whether or not to go for a move despite it failing 
	//everything except the range check
	bool fuck_it_close_enough(MoveList& move); //Randomly decide whether or not to go for a move despite it
	//failing everything including the range check

	bool can_use(); //Check whether or not the user has charge for this move if needed, and has meter for
	//this move if needed

	void load_chara_info(std::string chara_kind, std::string opponent_chara_kind);

	std::vector<MoveList> move_list;
	int judgement; //More or less just the % odds of fuck_it() returning true
	int precision; //The actual range of a given move will be offset by any number within this range
};