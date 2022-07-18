#pragma once
#include "ScriptArg.h"
#include <functional>

template<typename T>
class ScriptFunc {
public:
	ScriptFunc() {

	}

	ScriptFunc(void(T::* func)(ScriptArg), ScriptArg args) {
		this->function_call = func;
		this->function_args = args;
	}

	void execute(T* object) {
		std::mem_fn(function_call)(object, function_args);
	}

	void(T::* function_call)(ScriptArg);
	ScriptArg function_args;
};