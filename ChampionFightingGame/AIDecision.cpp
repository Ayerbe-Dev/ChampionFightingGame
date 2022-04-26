#include "AIDecision.h"
#include "AIMoveList.h"
#include <random>

AIDecision::AIDecision() {

}

AIDecision::AIDecision(int judgement, int precision, std::vector<AIMoveList>* opponent_move_list) {
	init(judgement, precision, nullptr);
}

void AIDecision::init(int judgement, int precision, std::vector<AIMoveList>* opponent_move_list) {
	this->judgement = judgement;
	this->precision = 10 - precision;
	this->opponent_move_list = opponent_move_list;
}

bool AIDecision::is_in_range(AIMoveList& move) {
	
}

bool AIDecision::fast_enough(AIMoveList& move) {

}

bool AIDecision::they_wont_press_here(AIMoveList& move) {

}

bool AIDecision::is_safe(AIMoveList& move) {

}

bool AIDecision::will_hit(AIMoveList& move) {

}

bool AIDecision::fuck_it(AIMoveList& move) {
	return fuck_it_close_enough(move) && is_in_range(move);
}

bool AIDecision::fuck_it_close_enough(AIMoveList& move) {
	int impulse = std::rand() % 10;
	return impulse > judgement;
}

bool AIDecision::can_use(AIMoveList& move) {

}