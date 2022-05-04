#pragma once
#include <algorithm>

//Disclaimer: This class is NOT designed to have a changing number of elements.

template <typename T>
class CircularBuffer {
public:
	CircularBuffer() {
		data = new T[1];
		num_elements = 1;
		curr_index = 0;
	}

	CircularBuffer(int size) {
		data = new T[size];
		num_elements = size;
		curr_index = 0;
	}

	~CircularBuffer() {
		delete[] data;
	}

	void resize(int size) {
		T* new_data = new T[size];
		int smallest_size = std::min(size, num_elements);
		for (int i = 0; i < smallest_size; i++) {
			new_data[i] = data[i];
		}
		num_elements = size;
		delete[] data;
		data = new_data;
		if (curr_index >= size) {
			curr_index = size - 1;
		}
	}

	int size() const {
		return num_elements;
	}

	void insert(T elem) {
		data[curr_index] = elem;
		curr_index = next_index(curr_index);
	}

	int next_index(int index) {
		if (index + 1 >= num_elements) {
			return 0;
		}
		else {
			return index + 1;
		}
	}

	int prev_index(int index) {
		if (index == 0) {
			return num_elements - 1;
		}
		else {
			return index - 1;
		}
	}

	T& oldest(int offset = 0) {
		int index = curr_index;
		for (int i = 0; i < offset; i++) {
			index = next_index(curr_index);
		}
		return data[index];
	}

	T& newest(int offset = 0) {
		int index = prev_index(curr_index);
		for (int i = 0; i < offset; i++) {
			index = prev_index(curr_index);
		}
		return data[index];
	}

private:
	int curr_index;
	int num_elements;
	T* data;
};