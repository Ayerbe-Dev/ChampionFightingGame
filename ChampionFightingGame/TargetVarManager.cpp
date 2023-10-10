#include "TargetVarManager.h"
#include "GameManager.h"
#include "TargetVar.h"

TargetVarManager::TargetVarManager() {

}

void TargetVarManager::process_targets() {
	if (!game_state_targets.empty()) {
		if (!unset_targets.empty()) {
			for (std::list<BaseTargetVar*>::iterator it = unset_targets.begin(),
				max = unset_targets.end(); it != max; it++) {
				game_state_targets.back().push_back(*it);
			}
			unset_targets.clear();
		}
		if (!game_state_targets.back().empty()) {
			for (std::list<BaseTargetVar*>::iterator it = game_state_targets.back().begin(),
				max = game_state_targets.back().end(); it != max; it++) {
				(*it)->process();
			}
		}
	}
}

void TargetVarManager::push_game_state_target_set() {
	this->game_state_targets.push_back(std::list<BaseTargetVar*>());
}

void TargetVarManager::pop_game_state_target_set() {
	game_state_targets.pop_back();
}

void TargetVarManager::register_target_var(BaseTargetVar* var) {
	unset_targets.push_back(var);
}

void TargetVarManager::unregister_target_var(BaseTargetVar* var) {
	if (!game_state_targets.empty()) {
		if (std::find(game_state_targets.back().begin(), game_state_targets.back().end(), var)
			!= game_state_targets.back().end()) {
			game_state_targets.back().remove(var);
		}
	}
	if (std::find(unset_targets.begin(), unset_targets.end(), var) != unset_targets.end()) {
		unset_targets.remove(var);
	}
}

TargetVarManager* TargetVarManager::instance = nullptr;
TargetVarManager* TargetVarManager::get_instance() {
	if (instance == nullptr) {
		instance = new TargetVarManager;
	}
	return instance;
}

void TargetVarManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}