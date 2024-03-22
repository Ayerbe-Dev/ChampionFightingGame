#include "ScriptFunc.h"

ScriptArg::ScriptArg() {
	this->num_args = 0;
}

ScriptArg::ScriptArg(int num_args, std::queue<std::any> args) {
	this->num_args = num_args;
	this->args = args;
}

void ScriptArg::push_arg(std::any arg) {
	num_args++;
	args.push(arg);
}

void ScriptArg::pop() {
	args.pop();
}

std::any ScriptArg::get_arg() {
	std::any ret = args.front();
	pop();
	return ret;
}