#include "BattleObject.h"
#include "ParamTable.h"

MoveScript::MoveScript() {
	id = -1;
};

MoveScript::MoveScript(string name, function<void()> move_script, int id) {
	this->name = name;
	this->move_script = move_script;
	this->id = id;
}

BattleObject::BattleObject() {
	this->id = -1;
}