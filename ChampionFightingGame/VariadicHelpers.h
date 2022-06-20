#pragma once
#include <queue>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/algorithm.hpp>
#include <any>
#include <iostream>
#include <glm/glm.hpp>

template<typename ...T>
std::queue<std::any> extract_variadic_to_queue_impl(const T&... args) {
	std::vector<std::any> vec = { args... };
	std::queue<std::any> ret;
	for (int i = 0, max = vec.size(); i < max; i++) {
		ret.push(vec[i]);
	}
	return ret;
}

template<typename ...T>
std::queue<std::any> extract_variadic_to_queue(T... args) {
	return extract_variadic_to_queue_impl(std::any(args)...);
}