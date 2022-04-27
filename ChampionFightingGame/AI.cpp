#include "AI.h"
#include "AIManager.h"
#include "Fighter.h"
#include <fstream>

AI::AI() {
	judgement = 8;
	precision = 10;
	execution = 10;
	reaction = 12;
	ai_info.resize(2);
}

AI::AI(int id, int judgement, int precision, int execution, int reaction) {
	init(id, judgement, precision, execution, reaction);
}

void AI::init(int id, int judgement, int precision, int execution, int reaction) {
	this->id = id;

	BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();
	load_chara_info(battle_object_manager->fighter[id]->chara_name, move_list);
	load_chara_info(battle_object_manager->fighter[!id]->chara_name, opponent_move_list);

	fighter = battle_object_manager->fighter[id];

	this->judgement = judgement;
	this->precision = 10 - precision;
	this->execution = execution;
	this->reaction = reaction;
}

void AI::ai_main() {
	AIManager* ai_manager = AIManager::get_instance();
	ai_manager->ai_mutex.lock();
	ai_info.insert(ai_manager->ai_info[!id].newest(reaction)); //Get the most recent actions from the
	//opponent that we're allowed to see
	ai_manager->ai_mutex.unlock();

	//Run through all of the functions in AIDecision.cpp to figure out what action we should take

	//Do some stuff in AIInput.cpp to simulate actually making the inputs
}

void AI::load_chara_info(std::string chara_kind, std::vector<AIMoveList>& target) {
	std::ifstream stream;
	stream.open("resource/chara/" + chara_kind + "/ai/movelist.yml");
	if (stream.fail()) {
		std::cerr << "Failed to open AI movelist: " << "resource/chara/" << chara_kind << "/ai/movelist.yml\n";
		stream.close();
		return;
	}

	sort_ai_movelist(target);
	stream.close();
}

void AI::sort_ai_movelist(std::vector<AIMoveList>& target) {

}