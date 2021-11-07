#include "CSSMenu.h"
#include "utils.h"
#include "Debugger.h"
#include "GameTexture.h"

CSSMenu::CSSMenu(SDL_Renderer *pRenderer){
    
    //temp vars
    ifstream fileCssTable;
	fileCssTable.open("resource/ui/menu/css/css_param.yml");
    int iCharacterKind;
	string sCharacterDir;
	bool bSelectable;
    FixedCharacterSlot *tmpSlot;
    int iRowXdelta=0;
    //

    this->pRenderer = pRenderer;

    //initialize other textures
    backgroundTexture.init("resource/ui/menu/css/CSSbackground.png",pRenderer);

	if (fileCssTable.fail()) {
        //it crashes anyways lol
		fileCssTable.close();
		printf("Failed to load CSS table");
	}

    //Initialize fixed slots
    string sCharacterName;
	for (int i = 0; getline(fileCssTable, sCharacterName); i++) {
        printf("Line %d, found %s\n",i,sCharacterName.c_str());
        fileCssTable >> iCharacterKind >> sCharacterDir >> bSelectable;
        if (bSelectable){
            addFixedCharacter(iCharacterKind, pRenderer);
        }
        getline(fileCssTable, sCharacterName);// 100% authentic jank code
    }
    //

    if (getSlotsLength() <= 10){
        //big scaling code
    } else {
        for (int iRow=1; iRow < 4;iRow++){
            printf("Row: %d\n",iRow);
            //Following line checks if the row is filled (the tenth item in the row is filled)
            if (aFixedCharacterSlots[iRow * 10 - 1].isInitialized()){
                //row filled code
                for (int iColumn=0; iColumn < 10;iColumn++){
                    printf("Column: %d\n",iColumn+1);
                    tmpSlot = &aFixedCharacterSlots[((iRow-1) * 10)+iColumn];
                    
                    tmpSlot->gameTexture.setScaleFactor(.8);
                    tmpSlot->gameTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

                    /*
                        (WINDOW_WIDTH/10) is for the spacing between the cards
                        (WINDOW_WIDTH/20) is for the offset to center the row, it should always be 1/2 the previous one
                    */
                    tmpSlot->setXPosition(iColumn * (WINDOW_WIDTH/10) + (WINDOW_WIDTH/20));
                    tmpSlot->setYPosition(iRow * (tmpSlot->gameTexture.getScaledHeight() + 20));   
                }
            } else {
                //row not filled code
                for (int iColumn=0; iColumn < getSlotsLength() % 10;iColumn++){
                    printf("funky Column: %d\n",iColumn+1);
                    tmpSlot = &aFixedCharacterSlots[((iRow-1) * 10)+iColumn];
                    
                    tmpSlot->gameTexture.setScaleFactor(.8);
                    tmpSlot->gameTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

                    /*
                        Basically.....
                        Calculate the difference in x positions of the first and last card as iRowXdelta
                        Divide iRowXdelta by 2 and subtract the WINDOW_WIDTH by iRowXdelta to get the starting point
                        from which to render the row. 
                    */
                    iRowXdelta = (WINDOW_WIDTH/10) * (getSlotsLength() % 10) - (WINDOW_WIDTH/10); //yes this gets recalculated every frame, no im not moving it
                    tmpSlot->setXPosition(((WINDOW_WIDTH-iRowXdelta)/2) + iColumn * (WINDOW_WIDTH/10));
                    tmpSlot->setYPosition(iRow * (tmpSlot->gameTexture.getScaledHeight()+20));
                }
            }
        }
    }
};

void CSSMenu::addFixedCharacter(int id, SDL_Renderer *pRenderer){
    //32 is the length of aFixedCharacterSlots
    for (int i=0; i < 32;i++){
        if (!aFixedCharacterSlots[i].isInitialized()){
            aFixedCharacterSlots[i].init(id, pRenderer);
            return;
        }
    }
}

int CSSMenu::getSlotsLength(){
    int size=0;
    for (int i=0; i < 32;i++){
        if (aFixedCharacterSlots[i].isInitialized()){
            size++;
        }
    }
    return size;
}

void CSSMenu::render(){
    backgroundTexture.render();

    for (int i=0; i < 32;i++){
        //printf("#%d is %d\n",i,aFixedCharacterSlots[i].isInitialized());
        if (aFixedCharacterSlots[i].isInitialized()){
            aFixedCharacterSlots[i].render();
        }
    }
}

bool FixedCharacterSlot::isInitialized(){
    return bInitialized;
}

void FixedCharacterSlot::init(int id, SDL_Renderer *pRenderer){
    gameTexture.init("resource/ui/menu/css/chara/default/render.png", pRenderer);
    iCharacterId = id;
    printf("Character Id %d was initialized!\n",id);
    bInitialized = true;
}

void FixedCharacterSlot::setYPosition(int y){
    gameTexture.destRect.y = y;
}

void FixedCharacterSlot::setXPosition(int x){
    gameTexture.destRect.x = x;
    printf("X Set to:%d\n",x);
}

int FixedCharacterSlot::getTextureWidth(){
    return gameTexture.destRect.w;
}

void FixedCharacterSlot::render(){
    gameTexture.render();
}



int cssMenu(SDL_Renderer *pRenderer, SDL_Window *pWindow, PlayerInfo aPlayerInfo[2]){
    printf("Entering CSS Menu\n");
    bool bSelecting = true;
    Uint32 tick=0,tok=0;
    const Uint8* keyboard_state;
	Debugger debugger;
    printf("Initializing Menu\n");
    CSSMenu cssMenuInstance(pRenderer);

    printf("Entering loop\n");
    while (bSelecting){
        //bSelecting=false;
        //printf("Loop Start\n");
        
        frameTimeDelay(&tick,&tok);

        SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

        /* spaghetti time */
        SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: {
					return GAME_STATE_CLOSE;
				} break;
			}
		}
        for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}
		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(pWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(pWindow, 0);
			}
			else {
				SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}
        /*End Of Spaghetti Time*/
        
        SDL_RenderClear(pRenderer);
        
        cssMenuInstance.render();
        
        SDL_RenderPresent(pRenderer);
    }

    return 333;
}