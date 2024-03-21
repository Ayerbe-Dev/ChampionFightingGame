#include "TimeFuncs.h"
#include <chrono>

thread_local std::chrono::steady_clock::time_point g_chron = std::chrono::steady_clock::now();
void wait_ms(double ms_duration, bool process_time) {
	std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
	std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<double, std::nano>> future_time = std::chrono::steady_clock::now() + std::chrono::duration<double, std::milli>(ms_duration);

	//reduce the future time to account for processing time
	if (process_time) {
		future_time -= (current_time - g_chron);
	}
	while (current_time < future_time) {
		current_time = std::chrono::steady_clock::now();
	}
	if (process_time) {
		g_chron = std::chrono::steady_clock::now();
	}
};

double get_ms_since_last_frame() {
	std::chrono::duration<double, std::milli> ms = std::chrono::steady_clock::now() - g_chron;
	return ms.count();
}