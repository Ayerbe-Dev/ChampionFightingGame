#pragma once
#include "AIMoveList.h"
#include "CircularBuffer.h"

struct AIInfo;

class Fighter;
class GameController;

class AI {
public:
	AI();
	AI(int id, int judgement, int precision, int execution, int reaction, int stubbornness);

	void init(int id, int judgement, int precision, int execution, int reaction, int stubbornness);

	void ai_main();
	void ai_neutral();

	void decision_main();
	void input_main();

	bool is_in_range(AIMoveList& move); //Check if you either are or will be in range to use this move
	bool fast_enough(AIMoveList& move); //Check if the move will come out faster than any of the opponent's moves within this range
	bool they_wont_press_here(AIMoveList& move); //Calculate odds of the opponent not using a move that can stuff this one
	bool is_safe(AIMoveList& move); //Check if opponent can punish this on block
	bool will_hit(AIMoveList& move); //Calculate odds of the opponent not blocking this

	//If a move has failed the other checks, sometimes use the move anyway if the judgement value is low enough
	bool fuck_it(AIMoveList& move);

	//Verify that the AI can even use this move
	bool can_use(AIMoveList& move);

	//Translates AI inputs into actual buttons, then turns them on
	void set_input(unsigned int input);

	int judgement; //More or less just the % odds of fuck_it() returning true
	int precision; //The actual range of a given move will be offset by any number within this range
	int execution; //Odds of dropping an input
	int reaction; //How far back through the opponent's log the AI has to read
	int stubbornness; //How difficult it is for the AI to change what move it wants to do

	Fighter* fighter;
	GameController* controller;
	int id;

	bool committed;
	AIMoveList* active_choice;
	int input_stage;

	std::vector<AIMoveList*> chosen_actions;
	std::vector<AIMoveList> move_list;
	std::vector<AIMoveList> opponent_move_list;

	CircularBuffer<AIInfo> ai_info;
private:
	void load_chara_info(std::string chara_kind, std::vector<AIMoveList>& target);
	void sort_ai_movelist(std::vector<AIMoveList>& target);
};