#include "AI.h"
#include "Fighter.h"

AI::AI() {

}

AI::AI(int id, int judgement, int precision, int execution, int reaction) {
	init(id, judgement, precision, execution, reaction);
}

void AI::init(int id, int judgement, int precision, int execution, int reaction) {
	this->id = id;

	BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();
	load_chara_info(battle_object_manager->fighter[id]->chara_name, move_list);
	load_chara_info(battle_object_manager->fighter[!id]->chara_name, opponent_move_list);

	fighter = battle_object_manager->fighter[id]; //The AI only gets direct access to its own character, not
	//the opponent

	decisions.init(judgement, precision, &opponent_move_list);
	inputs.init(execution, reaction);
}

void AI::load_chara_info(std::string chara_kind, std::vector<AIMoveList>& target) {

}