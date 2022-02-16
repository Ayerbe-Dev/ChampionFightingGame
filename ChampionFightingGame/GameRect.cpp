#include "GameRect.h"
#include "RenderManager.h"
extern RenderManager g_rendermanager;
using namespace glm;

GameRect::GameRect() {

}

GameRect::GameRect(vec2 c1, vec2 c2) {
	this->c1 = c1;
	this->c2 = c2;
}

void GameRect::render() {

}

