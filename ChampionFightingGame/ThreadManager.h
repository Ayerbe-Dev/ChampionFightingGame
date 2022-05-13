#pragma once
#include <thread>
#include <mutex>
#include <map>

enum {
	THREAD_KIND_PLAYER_1,
	THREAD_KIND_PLAYER_2,
	THREAD_KIND_EFFECT,

	THREAD_KIND_MAX
};

//On paper, literally nothing about this class is safe. In practice, everything about this is safe

class ThreadManager {
public:
	ThreadManager(ThreadManager& other) = delete;
	void operator=(const ThreadManager& other) = delete;

	static ThreadManager* get_instance();

	void add_thread(int id, std::thread thread);
	void lock_thread(int id = -1);
	void unlock_thread(int id = -1);
	void kill_thread(int id);
	bool is_active();
private:
	ThreadManager();
	static ThreadManager* instance;

	std::thread::id main_thread_id;

	std::thread threads[THREAD_KIND_MAX];
	std::mutex mutexes[THREAD_KIND_MAX];
	bool active_thread[THREAD_KIND_MAX];

	std::map<std::thread::id, int> id2id;
};