#include "TestFighter.h"

TestFighter::TestFighter(int id) {
	this->id = id;
	resource_dir = "resource/chara/testchar";
	superInit();
}