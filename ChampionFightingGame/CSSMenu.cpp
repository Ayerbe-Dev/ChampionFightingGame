#include "CSSMenu.h"
#include "utils.h"
#include "Debugger.h"

CSSMenu::CSSMenu(SDL_Renderer *pRenderer){
    
    //temp vars
    ifstream fileCssTable;
	fileCssTable.open("resource/ui/menu/css/css_param.yml");
    int iCharacterKind;
	string sCharacterDir;
	bool bSelectable;
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
        fileCssTable >> iCharacterKind >> sCharacterDir >> bSelectable;
        if (bSelectable){
            addFixedCharacter(iCharacterKind, pRenderer);
        }
    }
    //

    if (getFixedCharacterSlotsLength() <= 10){

    } else {
        for (int iRow=1; iRow <= 3;iRow++){
            //Following line checks if the row is filled (the tenth item in the row is filled)
            if (aFixedCharacterSlots[iRow * 10 - 1].isInitialized()){
                //row filled code
                for (int iColumn=0; iRow < 10;iRow++){
                    aFixedCharacterSlots[((iRow-1) * 10)+iColumn].setXPosition(iRow * aFixedCharacterSlots[((iRow-1) * 10)+iColumn].getTextureWidth());
                }
            } else {
                //row not filled code
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

int CSSMenu::getFixedCharacterSlotsLength(){
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
}

int FixedCharacterSlot::getTextureWidth(){
    return gameTexture.destRect.w;
}

void FixedCharacterSlot::render(){
    gameTexture.render();
}

bool GameTexture::init(string sTexturePath, SDL_Renderer *pRenderer){
    if (bIsInitialized){
        printf("GameTexture already initialized!\n");
        return false;
    }
    this->pRenderer = pRenderer;
    pTexture = loadTexture(sTexturePath.c_str(),pRenderer);
    SDL_QueryTexture(pTexture,nullptr,nullptr,&destRect.w,&destRect.h);
    destRect.x=0;
    destRect.y=0;
}

void GameTexture::render(){
    SDL_RenderCopy(pRenderer,pTexture,nullptr,&destRect);
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
        //printf("Loop Start\n");
        
        frameTimeDelay(&tick,&tok);

        SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(pWindow) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(pWindow, 0);
			}
			else {
				SDL_SetWindowFullscreen(pWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}
        for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}
        
        SDL_RenderClear(pRenderer);
        
        cssMenuInstance.render();
        
        SDL_RenderPresent(pRenderer);
    }

    return 333;
}