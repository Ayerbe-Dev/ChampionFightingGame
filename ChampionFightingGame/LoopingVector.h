#pragma once
#include <vector>
#include <stdexcept>

//i am so sorry for what you are about to witness

template <typename T>
class LoopingVector {
public:
	LoopingVector() {

	}

	LoopingVector(std::vector<T> data) {
		this->data = data;
	}

	T& operator[](int idx) {
		if (size() == 0) {
			throw std::range_error("Yeah ok even I can't salvage this one");
		}
		while (idx < 0) {
			idx += size();
		}
		while (idx >= size()) {
			idx -= size();
		}
		return data[idx];
	}

	const T& operator[](int idx) const {
		if (size() == 0) {
			throw std::range_error("Yeah ok even I can't salvage this one");
		}
		while (idx < 0) {
			idx += size();
		}
		while (idx >= size()) {
			idx -= size();
		}
		return data[idx];
	}

	void push_back(T val) {
		data.push_back(val);
	}

	void pop_back(T val) {
		data.pop_back();
	}

	std::size_t size() {
		return data.size();
	}
private:
	std::vector<T> data;
};