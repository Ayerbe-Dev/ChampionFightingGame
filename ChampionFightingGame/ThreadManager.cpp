#include "ThreadManager.h"
#include <iostream>
#include "utils.h"

ThreadObject::ThreadObject() {
	cv_start = false;
	cv_end = false;
	sync_var = false;
	execution_arg = nullptr;
	active = false;
}

void ThreadObject::execute() {
	while (active) {
		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock, [&] { return cv_start; });
		if (!active) {
			return;
		}
		cv_start = false;
		to_execute(execution_arg);
		cv_end = true;
		lock.unlock();
		cv.notify_all();
	}
}

void ThreadObject::init(std::function<void(void* execution_arg)> to_execute, void* execution_arg) {
	id = thread.get_id();
	this->to_execute = to_execute;
	this->execution_arg = execution_arg;
	active = true;
	thread = std::thread(&ThreadObject::execute, this);
	thread.detach();
}

ThreadManager::ThreadManager() {
	main_thread_id = std::this_thread::get_id();
}

void ThreadManager::add_thread(int id, std::function<void(void* execution_arg)> to_execute, void* execution_arg) {
	if (threads[id].active) {
		std::cout << "Error: Thread is already active at ID " << id << "\n";
	}
	threads[id].init(to_execute, execution_arg);
	id2id[threads[id].id] = id;
	threads[id].active = true;
}

//Note: This function technically has a use case, but ThreadObject::execute() never actually calls 
//lock or unlock, so calling this from the main thread is only useful if you're calling lock_thread
//manually within a function that got passed to the ThreadObject

//In other words, if all you need is to keep a thread synced with the main thread, don't use
//lock/unlock_thread(), use notify/wait_thread()
void ThreadManager::lock_thread(int id) {
	if (id == -1) {
		id = id2id[std::this_thread::get_id()];
	}
	while (!threads[id].mutex.try_lock());
}

void ThreadManager::unlock_thread(int id) {
	if (id == -1) {
		id = id2id[std::this_thread::get_id()];
	}
	threads[id].mutex.unlock();
}

bool ThreadManager::is_thread_unlocked(int id) {
	if (id == -1) {
		id = id2id[std::this_thread::get_id()];
	}
	std::unique_lock<std::mutex> lock(threads[id].mutex);
	if (lock.owns_lock()) {
		bool ret = lock.try_lock();
		if (ret) {
			lock.unlock();
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void ThreadManager::kill_thread(int id) {
	std::unique_lock<std::mutex> lock(threads[id].mutex);
	threads[id].active = false;
	threads[id].cv_start = true;
	threads[id].cv.notify_all();
	lock.unlock();
	id2id.erase(threads[id].id);
}

void ThreadManager::notify_thread(int id) {
	std::unique_lock<std::mutex> lock(threads[id].mutex);
	threads[id].cv_start = true;
	lock.unlock();
	threads[id].cv.notify_all();
}

void ThreadManager::wait_thread(int id) {
	std::unique_lock<std::mutex> lock(threads[id].mutex);
	bool& cv_end = threads[id].cv_end;
	threads[id].cv.wait(lock, [&] {return cv_end; });
	cv_end = false;
}

NO_INLINE void ThreadManager::sync_threads(int id1, int id2) {
	while (threads[id1].sync_var);
	threads[id1].sync_var = true;
	while (!threads[id2].sync_var);
	threads[id2].sync_var = false;
}

bool ThreadManager::is_active(int id) {
	return threads[id].active;
}

bool ThreadManager::is_active(std::thread::id& id) {
	try {
		int id_int = id2id.at(id);
		return threads[id_int].active;
	}
	catch (std::out_of_range()) { //Locking the thread while checking if it's active creates some wacky
		//syncing issues, so we're technically going to risk a data race. That being said, the only
		//risk of that happening is if we were to verify that the id was in the map, then have it get
		//removed from the map in another thread, resulting in us trying to access an index that no longer
		//exists. Instead, we just won't verify that the id is in the map at all until the actual call to
		//at(), and just catch it if it was removed at that point. Either way we'll know what to return.
		return false;
	}
}

bool ThreadManager::is_main_thread() {
	std::thread::id id = std::this_thread::get_id();
	return id == main_thread_id;
}

ThreadObject& ThreadManager::get_thread(int id) {
	if (id >= THREAD_KIND_MAX) {
		std::cout << "Thread ID " << id << " out of range, returning default thread.\n";
		return threads[0];
	}
	return threads[id];
}

ThreadManager* ThreadManager::instance = nullptr;
ThreadManager* ThreadManager::get_instance() {
	if (instance == nullptr) {
		instance = new ThreadManager;
	}
	return instance;
}

void ThreadManager::destroy_instance() {
	if (instance != nullptr) {
		delete instance;
	}
}