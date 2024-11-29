#pragma once
#include "Scene.h"

void title_screen_main();

class TitleScreen: public Scene {
public:
    TitleScreen();
    ~TitleScreen();

    void render_main();
};