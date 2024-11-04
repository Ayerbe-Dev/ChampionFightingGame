#pragma once
#include <list>
#include <vector>

class BaseTargetVar;

class TargetVarManager {
public:
	TargetVarManager(TargetVarManager& other) = delete;
	void operator=(const TargetVarManager& other) = delete;

	void process_targets();
	void register_target_var(BaseTargetVar* var);
	void unregister_target_var(BaseTargetVar* var);

	static TargetVarManager* get_instance();
	void destroy_instance();
private:
	TargetVarManager();
	static TargetVarManager* instance;

	std::list<BaseTargetVar*> targets;
};