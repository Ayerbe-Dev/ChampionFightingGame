#pragma once
#include <queue>
#include <any>
#include <iostream>
#include "GameManager.h"

template<typename ...T>
std::queue<std::any> extract_variadic_to_queue_impl(std::vector<int>* error_vec, const T&... args) {
	std::vector<std::any> vec = { args... };
	std::queue<std::any> ret;
	for (int i = 0, max = vec.size(); i < max; i++) {
		if (vec[i].type() == typeid(double)) {
			ret.push((float)(std::any_cast<double>(vec[i])));
		}
		else if (vec[i].type() == typeid(const char*)) {
			ret.push(std::string(std::any_cast<const char*>(vec[i])));
		}
		else {
			ret.push(vec[i]);
			std::string type_name = vec[i].type().name();
			if (type_name.starts_with("enum <unnamed-enum-")) {
				error_vec->push_back(i);
			}
		}
	}
	return ret;
}

template<typename ...T>
std::queue<std::any> extract_variadic_to_queue(std::vector<int>* error_vec, T... args) {
	return extract_variadic_to_queue_impl(error_vec, std::any(args)...);
}