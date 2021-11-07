#include "FighterAccessor.h"

FighterAccessor::FighterAccessor() {}

FighterAccessor::FighterAccessor(Fighter* p1, Fighter* p2) {
	this->fighter[0] = p1;
	this->fighter[1] = p2;
}