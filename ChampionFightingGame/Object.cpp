#include "Object.h"
#include "ParamTable.h"

MoveScript::MoveScript() {};

MoveScript::MoveScript(string name, function<void()> move_script, int id) {
	this->name = name;
	this->move_script = move_script;
	this->id = id;
}

Object::Object() {
	this->id = -1;
}