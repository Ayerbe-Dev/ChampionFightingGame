#pragma once
#include <queue>
#include <any>

//Declares a variable of the given type and extracts its value from the queue
#define UNWRAP(var_name, type) type var_name = std::any_cast<type>(args.get_arg())
//Same as the above, but for variables that were already declared
#define UNWRAP_NO_DECL(var_name) if (!args.args.empty()) { var_name = std::any_cast<decltype(var_name)>(args.get_arg()); }

struct ScriptArg {
	ScriptArg();
	ScriptArg(int num_args, std::queue<std::any> args);

	std::any get_arg();
	void pop();
	int num_args;

	std::queue<std::any> args;
};