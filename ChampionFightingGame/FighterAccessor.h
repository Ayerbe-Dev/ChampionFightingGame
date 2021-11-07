#pragma once
#include "utils.h"
#include "GameCoordinate.h"
#include "FighterAccessor.fwd.h"
#include "Fighter.fwd.h"

/*
	This class allows one fighter to access the attributes of the other without needing a pointer to the other fighter.
	This is mostly so we dont confuse ourselves with members.
*/
class FighterAccessor {
public:
	Fighter* fighter[2]; 
	/*
	this is PROBABLY a bad idea but if it turns out to be too memory intensive, we can just change this so it only accesses the needed info
	rn the only thing we need it for is position lmao
	*/

	FighterAccessor();
	FighterAccessor(Fighter* p1, Fighter* p2);
};