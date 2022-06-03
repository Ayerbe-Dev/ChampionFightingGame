#include "AI.h"
#include "AIManager.h"
#include "Fighter.h"
#include <fstream>

AI::AI() {
	id = -1;
	fighter = nullptr;
	controller = nullptr;
	judgement = 8;
	precision = 10;
	execution = 10;
	reaction = 12;
	ai_info.resize(2);
	committed = false;
	input_stage = 0;
	active_choice = nullptr;
}

AI::AI(int id, int judgement, int precision, int execution, int reaction, int stubbornness) {
	init(id, judgement, precision, execution, reaction, stubbornness);
}

void AI::init(int id, int judgement, int precision, int execution, int reaction, int stubbornness) {
	this->id = id;

	BattleObjectManager* battle_object_manager = BattleObjectManager::get_instance();
	load_chara_info(battle_object_manager->fighter[id]->chara_name, move_list);
	load_chara_info(battle_object_manager->fighter[!id]->chara_name, opponent_move_list);

	fighter = battle_object_manager->fighter[id];
	controller = &fighter->player->controller;

	this->judgement = judgement;
	this->precision = 10 - precision;
	this->execution = execution;
	this->reaction = reaction;
	this->stubbornness = stubbornness;
	committed = false;
	input_stage = 0;
	active_choice = nullptr;
}

void AI::ai_main() {
	AIManager* ai_manager = AIManager::get_instance();
	ai_manager->ai_mutex.lock();
	ai_info.insert(ai_manager->ai_info[!id].newest(reaction)); //Get the most recent actions from the
	//opponent that we're allowed to see
	ai_manager->ai_mutex.unlock();

	if (fighter->get_status_group() == STATUS_GROUP_HITSTUN) {
	
	}
	else {
		ai_neutral();
	}
}

void AI::ai_neutral() {
	if (!committed) {
		decision_main();
	}

	input_main();
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