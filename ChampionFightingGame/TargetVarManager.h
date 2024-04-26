#pragma once
#include <list>
#include <vector>

class BaseTargetVar;

class TargetVarManager {
public:
	TargetVarManager(TargetVarManager& other) = delete;
	void operator=(const TargetVarManager& other) = delete;

	void process_targets();
	void push_game_state_target_set();
	void pop_game_state_target_set();

	void register_target_var(BaseTargetVar* var, bool persistent);
	void unregister_target_var(BaseTargetVar* var, bool persistent);

	static TargetVarManager* get_instance();
	void destroy_instance();
private:
	TargetVarManager();
	static TargetVarManager* instance;

	std::vector<std::list<BaseTargetVar*>> game_state_targets;
	std::list<BaseTargetVar*> unset_targets;
	std::list<BaseTargetVar*> persistent_targets;
};