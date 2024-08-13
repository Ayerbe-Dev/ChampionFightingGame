#pragma once
#include <queue>
#include <any>
#include <functional>
#include <string>

//Declares a variable or function of the given type and extracts its value from the queue
#define UNWRAP(var_name, type) type var_name = std::any_cast<type>(args.get_arg())
#define UNWRAP_FUN(var_name, obj_type, fun_type) fun_type(obj_type::*var_name)() = std::any_cast<fun_type(obj_type::*)()>(args.get_arg())
#define UNWRAP_FUN_CONST(var_name, obj_type, fun_type) fun_type(obj_type::*var_name)() const = std::any_cast<fun_type(obj_type::*)() const>(args.get_arg())
//Same as the above, but for variables that were already declared
#define UNWRAP_NO_DECL(var_name) if (!args.args.empty()) { var_name = std::any_cast<decltype(var_name)>(args.get_arg()); }
//Get the typeid for the next arg
#define WRAPPED_TYPE(type_id) (!args.args.empty()) && args.args.front().type() == typeid(type_id)

struct ScriptArg {
	ScriptArg();
	ScriptArg(int num_args, std::queue<std::any> args);

	std::any get_arg();
	void push_arg(std::any arg);
	void pop();
	int num_args;

	std::queue<std::any> args;
};

const unsigned int SCRIPT_CONDITION_KIND_FALSE = 0;
const unsigned int SCRIPT_CONDITION_KIND_TRUE = 1;
const unsigned int SCRIPT_CONDITION_KIND_NONE = 2;

template<typename T>
class ScriptFunc {
public:
	ScriptFunc() {
		this->condition_name = "";
		this->condition_kind = SCRIPT_CONDITION_KIND_NONE;
	}

	ScriptFunc(void(T::* func)(ScriptArg), ScriptArg args) {
		this->condition_name = "";
		this->condition_kind = SCRIPT_CONDITION_KIND_NONE;
		this->function_call = func;
		this->function_args = args;
	}

	ScriptFunc(std::string condition_name, unsigned int condition_kind,
		void(T::* func)(ScriptArg), ScriptArg args) {
		this->condition_name = condition_name;
		this->condition_kind = condition_kind;
		this->function_call = func;
		this->function_args = args;
	}

	void execute(T* object) {
		std::mem_fn(function_call)(object, function_args);
	}

	void(T::* function_call)(ScriptArg);
	ScriptArg function_args;
	unsigned int condition_kind;
	std::string condition_name;
};