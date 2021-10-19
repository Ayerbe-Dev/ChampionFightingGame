#include "FighterInstanceAccessor.h"

FighterInstanceAccessor::FighterInstanceAccessor() {}

FighterInstanceAccessor::FighterInstanceAccessor(FighterInstance* p1, FighterInstance* p2) {
	this->fighter_instance[0] = p1;
	this->fighter_instance[1] = p2;
}