#pragma once
#include <source_location>
#include <type_traits>
#define MAKE_COUNTER(name, offset) constexpr CTCounter<offset - std::source_location::current().line() - 2> name;

template<int N>
class CTCounter {
public:
	static constexpr const int count(const std::source_location location = std::source_location::current()) {
		return location.line() + N;
	}
};