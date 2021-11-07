#pragma once
#include "utils.h"
#include "GameCoordinate.h"
#include "FighterAccessor.fwd.h"
#include "Fighter.fwd.h"

/*
	This class allows one fighter to access the attributes of the other without needing a pointer to the other fighter.
	This is mostly so we dont confuse ourselves with members. Some common information that is shared between players will also be here, such as
	which fighter has priority in terms of rendering.
*/
class FighterAccessor {
public:
	Fighter* fighter[2];
	int render_priority = 0;
	int render_priority_no_req = 0;

	FighterAccessor();
	FighterAccessor(Fighter* p1, Fighter* p2);
};