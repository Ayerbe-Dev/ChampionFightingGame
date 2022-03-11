#include "FighterInterface.h"
#include "Fighters.h"

Fighter* create_fighter(int chara_kind, int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	FighterInterface fighter_interface(chara_kind, id, player_info, fighter_accessor);
	Fighter* ret = fighter_interface.get_fighter();
	return ret;
}

FighterInterface::FighterInterface() {}

FighterInterface::FighterInterface(int chara_kind, int id, PlayerInfo* player_info, FighterAccessor* fighter_accessor) {
	switch (chara_kind) {
		case (CHARA_KIND_ROY):
		default:
		{
			fighter = new Roy(id, player_info, fighter_accessor);
		}
		break;
		case (CHARA_KIND_ERIC):
		{
			fighter = new Eric(id, player_info, fighter_accessor);
		}
		break;
		case (CHARA_KIND_LEON):
		case (CHARA_KIND_CHAMELEON): //Leon and Chameleon are the same character, but the specifics on where to get resources are in the constructor
		{
			fighter = new Leon(id, player_info, fighter_accessor);
		}
		break;
		case (CHARA_KIND_ANGELICA):
		{
			fighter = new Angelica(id, player_info, fighter_accessor);
		}
		break;
		case (CHARA_KIND_NIGHTSABER):
		{
			fighter = new Nightsaber(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_SULLY):
		{
			fighter = new Sully(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_PRIEST):
		{
			fighter = new Priest(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_AZIEL):
		{
			fighter = new Aziel(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_BRUNO):
		{
			fighter = new Bruno(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_TESSA):
		{
			fighter = new Tessa(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_ALEJANDRO):
		{
			fighter = new Alejandro(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_NORMAN):
		{
			fighter = new Norman(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_ATLAS):
		{
			fighter = new Atlas(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_JULIUS):
		{
			fighter = new Julius(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_RAMONA):
		{
			fighter = new Ramona(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_ZYAIR):
		{
			fighter = new Zyair(id, player_info, fighter_accessor);
		} break;
		case (CHARA_KIND_VESUVIUS):
		{
			fighter = new Vesuvius(id, player_info, fighter_accessor);
		} break;
	}
	fighter->player_info = player_info;
	fighter_accessor->fighter[id] = fighter;
	fighter->fighter_accessor = fighter_accessor;
}

FighterInterface::~FighterInterface() {}

Fighter* FighterInterface::get_fighter() {
	return fighter;
}