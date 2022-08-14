#include "FighterInterface.h"
#include "Fighters.h"

Fighter* create_fighter(int chara_kind, int id, Player* player) {
	FighterInterface fighter_interface(chara_kind, id, player);
	Fighter* ret = fighter_interface.get_fighter();
	return ret;
}

FighterInterface::FighterInterface() {

}

FighterInterface::FighterInterface(int chara_kind, int id, Player* player) {
	switch (chara_kind) {
		case (CHARA_KIND_ROY):
		default:
		{
			fighter = new Roy(id, player);
		}
		break;
		case (CHARA_KIND_ERIC):
		{
			fighter = new Eric(id, player);
		}
		break;
		case (CHARA_KIND_LEON):
		case (CHARA_KIND_CHAMELEON): //Leon and Chameleon are the same character, but the specifics on where to get resources are in the constructor
		{
			fighter = new Leon(id, player);
		}
		break;
		case (CHARA_KIND_ANGELICA):
		{
			fighter = new Angelica(id, player);
		}
		break;
		case (CHARA_KIND_NIGHTSABER):
		{
			fighter = new Nightsaber(id, player);
		} break;
		case (CHARA_KIND_SULLY):
		{
			fighter = new Sully(id, player);
		} break;
		case (CHARA_KIND_ROOK):
		{
			fighter = new Rook(id, player);
		} break;
		case (CHARA_KIND_DANTE):
		{
			fighter = new Dante(id, player);
		} break;
		case (CHARA_KIND_BRUNO):
		{
			fighter = new Bruno(id, player);
		} break;
		case (CHARA_KIND_TESSA):
		{
			fighter = new Tessa(id, player);
		} break;
		case (CHARA_KIND_ALEJANDRO):
		{
			fighter = new Alejandro(id, player);
		} break;
		case (CHARA_KIND_NORMAN):
		{
			fighter = new Norman(id, player);
		} break;
		case (CHARA_KIND_ATLAS):
		{
			fighter = new Atlas(id, player);
		} break;
		case (CHARA_KIND_JULIUS):
		{
			fighter = new Julius(id, player);
		} break;
		case (CHARA_KIND_RAMONA):
		{
			fighter = new Ramona(id, player);
		} break;
		case (CHARA_KIND_ZYAIR):
		{
			fighter = new Zyair(id, player);
		} break;
		case (CHARA_KIND_VESUVIUS):
		{
			fighter = new Vesuvius(id, player);
		} break;
	}
	BattleObjectManager::get_instance()->fighter[id] = fighter;
}

FighterInterface::~FighterInterface() {}

Fighter* FighterInterface::get_fighter() {
	return fighter;
}