#pragma once
#include <iostream>
#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"

#include "debug.h"

class Battle;
class StageSelect;

class Fighter;
class GameRect;
class DebugMenu;

void cotr_imgui_init();
void cotr_imgui_debug_dbmenu(DebugMenu* debug_menu);
void cotr_imgui_debug_battle(Battle* battle);
void cotr_imgui_terminate();
