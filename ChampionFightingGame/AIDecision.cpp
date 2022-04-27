#include "AI.h"
#include <random>

bool AI::is_in_range(AIMoveList& move) {
	return true;
}

bool AI::fast_enough(AIMoveList& move) {
	return true;
}

bool AI::they_wont_press_here(AIMoveList& move) {
	return true;
}

bool AI::is_safe(AIMoveList& move) {
	return true;
}

bool AI::will_hit(AIMoveList& move) {
	return true;
}

bool AI::fuck_it(AIMoveList& move) {
	return fuck_it_close_enough(move) && is_in_range(move);
}

bool AI::fuck_it_close_enough(AIMoveList& move) {
	int impulse = std::rand() % 10;
	return impulse > judgement;
}

bool AI::can_use(AIMoveList& move) {
	return true;
}