#pragma once
#include "Scene.h"
#include <string>
#include <iostream>
#include <functional>
#include <vector>
#include "Button.h"
#include "Player.h"
#include "MainMenuConstants.h"

void main_menu_main();

class MainMenu: public Scene {
public:
    MainMenu();
    ~MainMenu();

    void process_main();
    void render_main();

    int sub_scene;
private:
    //Menu Wheel Variables
    float offset;
    float magnitude;

    int menu_frame;

};
