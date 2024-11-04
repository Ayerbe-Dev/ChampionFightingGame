#include "TargetVarManager.h"
#include "TargetVar.h"

TargetVarManager::TargetVarManager() {

}

void TargetVarManager::process_targets() {
	for (std::list<BaseTargetVar*>::iterator it = targets.begin(),
		max = targets.end(); it != max; it++) {
		if (!(*it)->pause) {
			(*it)->process();
		}
	}
}

void TargetVarManager::register_target_var(BaseTargetVar* var) {
	targets.push_back(var);
}

void TargetVarManager::unregister_target_var(BaseTargetVar* var) {
	if (std::find(targets.begin(), targets.end(), var) != targets.end()) {
		targets.remove(var);
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