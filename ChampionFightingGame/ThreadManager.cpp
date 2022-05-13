#include "ThreadManager.h"
#include <iostream>

ThreadManager* ThreadManager::instance = nullptr;

ThreadManager::ThreadManager() {
	main_thread_id = std::this_thread::get_id();
	for (int i = 0; i < THREAD_KIND_MAX; i++) {
		active_thread[i] = false;
	}
}

ThreadManager* ThreadManager::get_instance() {
	if (instance == nullptr) {
		instance = new ThreadManager;
	}
	return instance;
}

void ThreadManager::add_thread(int id, std::thread thread) {
	if (active_thread[id]) {
		std::cout << "Error: Thread is already active at ID " << id << "\n";
	}
	id2id[thread.get_id()] = id;
	active_thread[id] = true;
	thread.detach();
}

void ThreadManager::lock_thread(int id) {
	if (id == -1) {
		id = id2id[std::this_thread::get_id()];
	}
	mutexes[id].lock();
}

void ThreadManager::unlock_thread(int id) {
	if (id == -1) {
		id = id2id[std::this_thread::get_id()];
	}
	mutexes[id].unlock();
}

void ThreadManager::kill_thread(int id) {
	lock_thread(id);
	active_thread[id] = false;
	unlock_thread(id);
}

bool ThreadManager::is_active() {
	std::thread::id id = std::this_thread::get_id();
	if (id == main_thread_id) {
		return true;
	}
	else if (id2id.find(id) == id2id.end()) {
		return false;
	}
	else {
		int id_int = id2id[id];
		lock_thread(id_int);
		//We don't unlock the mutex because if we were trying to avoid a data race in the main thread,
		//that thread was going to lock it as soon as this function finished returning, so it wouldn't
		//get a chance to actually run
		return active_thread[id_int];
	}
}