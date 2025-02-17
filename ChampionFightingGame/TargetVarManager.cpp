#include "TargetVarManager.h"
#include "TargetVar.h"
#include <iostream>

TargetVarManager::TargetVarManager() {
	removal_idx = 0;
	dir = 0;
}

void TargetVarManager::process_targets() {
	for (int i = 0; i < targets.size(); i++) {
		if (!targets[i]->pause) {
			targets[i]->process();
		}
	}
}

//Originally this class was implemented using a linked list. However, every time a SceneElement
//adds a new object using add_element(s), at least 4 different constructors and destructors will
//run. As a result, during the creation of a single object, all of its TargetVars will be added to
//and removed from the back of the list multiple times.

void TargetVarManager::register_target_var(BaseTargetVar* var) {
	//Each addition to the list took O(1) time, but it also required a separate heap allocation, which 
	//wasn't ideal given how often elements were removed. This fix was pretty simple: Just change it 
	//from a list to a vector, and now adding, removing and then adding a target var only uses 1 
	//allocation, if any.

	targets.push_back(var);
	removal_idx++;
}

void TargetVarManager::unregister_target_var(BaseTargetVar* var) {
	//The removal process, on the other hand, was a lot more complicated of a fix to optimize.
	//std::list::remove() is O(n), it starts from the front, and since destructors for member variables
	//run in reverse order, this meant that the removal process for a single renderable object took 
	//O((n+k)n) time, where k is the size of the list not including the renderable object.

	//This is what we in the business like to call "slow as fuck."

	//If we removed an element from somewhere other than the back of the list, we'll store the index 
	//we found it at, because for renderable objects, chances are the next thing we're about to remove
	//is next to it.
	if (removal_idx != targets.size()) {
		//Because of how we remove elements from our vector, it's possible that if we remove an entire
		//object from somewhere other than the back of the array, the layout of objects in front of it
		//will be mirrored. Thus, we want to check the elements on both sides of the one we last 
		//removed. That being said, once we've found a direction to check in, we'll want to keep going
		//in that direction until it stops working. Once that's happened, that's our signal to use the
		//standard reverse iteration.
		if (dir >= 0 && removal_idx != targets.size() - 1 && targets[removal_idx + 1] == var) {
			removal_idx++;
			dir = 1;
			targets[removal_idx] = targets.back();
			targets.pop_back();
			return;
		}
		if (dir <= 0 && removal_idx != 0 && targets[removal_idx - 1] == var) {
			removal_idx--;
			dir = -1;
			targets[removal_idx] = targets.back();
			targets.pop_back();
			return;
		}
		removal_idx = targets.size();
		dir = 0;
	}

	for (int i = targets.size() - 1; i >= 0; i--) {
		if (targets[i] == var) {
			removal_idx = i;
			targets[i] = targets.back();
			targets.pop_back();
			return;
		}
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