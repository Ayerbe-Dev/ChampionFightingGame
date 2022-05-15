#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <map>
#include <functional>

enum {
	THREAD_KIND_PLAYER_1,
	THREAD_KIND_PLAYER_2,
	THREAD_KIND_EFFECT,

	THREAD_KIND_MAX
};

class ThreadObject {
public:
	ThreadObject();

	std::thread thread;
	std::thread::id id;
	std::mutex mutex;
	std::condition_variable cv;
	bool cv_start;
	bool cv_end;
	bool active;
	
	std::function<void(void*)> to_execute;
	void* execution_arg;

	void execute();
	void init(std::function<void(void* execution_arg)> to_execute, void* execution_arg);
};

class ThreadManager {
public:
	ThreadManager(ThreadManager& other) = delete;
	void operator=(const ThreadManager& other) = delete;

	static ThreadManager* get_instance();

	void add_thread(int id, std::function<void(void* execution_arg)> to_execute, void* execution_arg);
	void lock_thread(int id = -1);
	void unlock_thread(int id = -1);
	bool is_thread_unlocked(int id = -1);
	void notify_thread(int id);
	void wait_thread(int id);
	void kill_thread(int id);
	bool is_active(std::thread::id &id);
	bool is_main_thread();
private:
	ThreadManager();
	static ThreadManager* instance;

	std::thread::id main_thread_id;

	ThreadObject threads[THREAD_KIND_MAX];

	std::map<std::thread::id, int> id2id;
};