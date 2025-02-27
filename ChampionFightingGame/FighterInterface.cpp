#include "Fighters.h"

Fighter* create_fighter(Player* player) {
	Fighter* fighter;
	switch (player->chara_kind) {
		case (CHARA_KIND_ROWAN):
		default:
		{
			fighter = new Rowan(player);
		}
		break;
		case (CHARA_KIND_ERIC):
		{
			fighter = new Eric(player);
		}
		break;
		case (CHARA_KIND_LEON):
		case (CHARA_KIND_CHAMELEON): //Leon and Chameleon are the same character, but the specifics on where to get resources are in the constructor
		{
			fighter = new Leon(player);
		}
		break;
		case (CHARA_KIND_DESIRAE):
		{
			fighter = new Desirae(player);
		}
		break;
		case (CHARA_KIND_NIGHTSABRE):
		{
			fighter = new Nightsabre(player);
		} break;
		case (CHARA_KIND_ATHENA):
		{
			fighter = new Athena(player);
		} break;
		case (CHARA_KIND_ROOK):
		{
			fighter = new Rook(player);
		} break;
		case (CHARA_KIND_DANTE):
		{
			fighter = new Dante(player);
		} break;
		case (CHARA_KIND_BRUNO):
		{
			fighter = new Bruno(player);
		} break;
		case (CHARA_KIND_TESSA):
		{
			fighter = new Tessa(player);
		} break;
		case (CHARA_KIND_DINGO):
		{
			fighter = new Dingo(player);
		} break;
		case (CHARA_KIND_WALLACE):
		{
			fighter = new Wallace(player);
		} break;
		case (CHARA_KIND_ATLAS):
		{
			fighter = new Atlas(player);
		} break;
		case (CHARA_KIND_JULIUS):
		{
			fighter = new Julius(player);
		} break;
		case (CHARA_KIND_RAMONA):
		{
			fighter = new Ramona(player);
		} break;
		case (CHARA_KIND_ZYAIR):
		{
			fighter = new Zyair(player);
		} break;
		case (CHARA_KIND_VESUVIUS):
		{
			fighter = new Vesuvius(player);
		} break;
	}
	ObjectManager::get_instance()->fighter[player->id] = fighter;
	return fighter;
}