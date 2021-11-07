#include <cmath>
#include "CSSMenu.h"
#include "utils.h"
#include "Debugger.h"
#include "GameTexture.h"
#include "MenuHandler.h"
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

CSSMenu::CSSMenu(){
    
    //temp vars
    ifstream fileCssTable;
	fileCssTable.open("resource/ui/menu/css/css_param.yml");
    int iCharacterKind;
	string sCharacterDir;
	bool bSelectable;
    FixedCharacterSlot *tmpSlot;
    int iRowXdelta=0;
    //

    //initialize other textures
    backgroundTexture.init("resource/ui/menu/css/CSSbackground.png");
    backgroundTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
    bigBarTexture.init("resource/ui/menu/css/CSSbottombar.png");
    bigBarTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
    topBarTexture.init("resource/ui/menu/css/CSStopbar.png");
    topBarTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
    //aMobileCharacterSlots[0].gameTexture.init("resource/ui/menu/css/chara/default/render.png");
    //aMobileCharacterSlots[0].gameTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

	if (fileCssTable.fail()) {
        //it crashes anyways lol
		fileCssTable.close();
		printf("Failed to load CSS table");
	}

    //Initialize fixed slots
    string sCharacterName;
	for (int i = 0; getline(fileCssTable, sCharacterName); i++) {
        //printf("Line %d, found %s\n",i,sCharacterName.c_str());
        fileCssTable >> iCharacterKind >> sCharacterDir >> bSelectable;
        if (bSelectable){
            addFixedCharacter(iCharacterKind);
        }
        getline(fileCssTable, sCharacterName);// 100% authentic jank code
    }
    //

    if (getSlotsLength() <= 10){
        //big scaling code
    } else {
        for (int iRow=1; iRow < 4;iRow++){
           // printf("Row: %d\n",iRow);
            //Following line checks if the row is filled (the tenth item in the row is filled)
            if (aFixedCharacterSlots[iRow * 10 - 1].isInitialized()){
                //row filled code
                for (int iColumn=0; iColumn < 10;iColumn++){
                    //printf("Column: %d\n",iColumn+1);
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
                    //printf("funky Column: %d\n",iColumn+1);
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

void CSSMenu::addFixedCharacter(int id){
    //32 is the length of aFixedCharacterSlots
    for (int i=0; i < 32;i++){
        if (!aFixedCharacterSlots[i].isInitialized()){
            aFixedCharacterSlots[i].init(id);
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
    bigBarTexture.render();
    topBarTexture.render();

    for (int i=0; i < 32;i++){
        //printf("#%d is %d\n",i,aFixedCharacterSlots[i].isInitialized());
        if (aFixedCharacterSlots[i].isInitialized()){
            aFixedCharacterSlots[i].render();
        }
    }

    
    for (int i=0;i<2;i++){
        
        aMobileCharacterSlots[i].playAnim();
        aMobileCharacterSlots[i].gameTexture.render();
        //printf("id %d, is %d\n",i,b);
    }
}

void CSSMenu::traverseRight(){
    if (aPlayerSelectionIndex[iLazyPassthrough] % 10 <= 8 && aPlayerSelectionIndex[iLazyPassthrough]+1 < getSlotsLength() && aFixedCharacterSlots[aPlayerSelectionIndex[iLazyPassthrough]+1].isInitialized()) {
        aPlayerSelectionIndex[iLazyPassthrough]++;
    }
    iLastUp[iLazyPassthrough] = -1;
    iLastDown[iLazyPassthrough] = -1;
}

void CSSMenu::traverseLeft(){
    if (aPlayerSelectionIndex[iLazyPassthrough] % 10 >=1) {
        aPlayerSelectionIndex[iLazyPassthrough]--;
    }
    iLastDown[iLazyPassthrough] = -1;
    iLastUp[iLazyPassthrough] = -1;
}

void CSSMenu::traverseDown(){
    int tmpCurrentX;
    int tmpCurrentY; // we need to know where the current location is

    int closestX=9999;
    int closestY=9999;

    int distanceNew, distanceCurrent;

    if (iLastDown[iLazyPassthrough] != -1){
        aPlayerSelectionIndex[iLazyPassthrough] = iLastDown[iLazyPassthrough];
        return;
    }

    //printf("Querrying with LP:%d FV: %d\n",iLazyPassthrough,aPlayerSelectionIndex[iLazyPassthrough]);
    querryFixedCssSlotPosition(aPlayerSelectionIndex[iLazyPassthrough],&tmpCurrentX,&tmpCurrentY);
    //printf("Passed Querry\n");
    
    tmpCurrentX+= 30; //magic sauce

    int tot=0;

    //Go through every possible card, if they are initialized and below the current pos the add them to the list
    for (int i=0; i < 32;i++){
        if (aFixedCharacterSlots[i].isInitialized() && aFixedCharacterSlots[i].isBelow(tmpCurrentY)){
            //printf("Card %d passed\n",i);

            tot++;

            distanceNew = twoPointDistance(aFixedCharacterSlots[i].gameTexture.destRect.x,aFixedCharacterSlots[i].gameTexture.destRect.y,tmpCurrentX,tmpCurrentY);
            distanceCurrent = twoPointDistance(closestX,closestY,tmpCurrentX,tmpCurrentY);

            //printf("With a distance of %d, fighting %d\n",distanceNew,distanceCurrent);

            if (distanceNew < distanceCurrent){
                
                closestX = aFixedCharacterSlots[i].gameTexture.destRect.x;
                closestY = aFixedCharacterSlots[i].gameTexture.destRect.y;
                //printf("player %d set to %d\n",iLazyPassthrough,i);
                aPlayerSelectionIndex[iLazyPassthrough] = i;
                iLastDown[iLazyPassthrough] = i;
            }
        }
    }
    //printf("Found %d cards below\n",tot);
}

void CSSMenu::traverseUp(){
    int tmpCurrentX;
    int tmpCurrentY; // we need to know where the current location is

    int closestX=9999;
    int closestY=9999;

    int distanceNew, distanceCurrent;

    if (iLastUp[iLazyPassthrough] != -1){
        aPlayerSelectionIndex[iLazyPassthrough] = iLastUp[iLazyPassthrough];
        return;
    }

    querryFixedCssSlotPosition(aPlayerSelectionIndex[iLazyPassthrough],&tmpCurrentX,&tmpCurrentY);

    int tot=0;

    //Go through every possible card, if they are initialized and below the current pos the add them to the list
    for (int i=0; i < 32;i++){
        if (aFixedCharacterSlots[i].isInitialized() && aFixedCharacterSlots[i].isAbove(tmpCurrentY)){
            //printf("Card %d passed\n",i);

            tot++;

            distanceNew = twoPointDistance(aFixedCharacterSlots[i].gameTexture.destRect.x,aFixedCharacterSlots[i].gameTexture.destRect.y,tmpCurrentX,tmpCurrentY);
            distanceCurrent = twoPointDistance(closestX,closestY,tmpCurrentX,tmpCurrentY);

            //printf("With a distance of %d, fighting %d\n",distanceNew,distanceCurrent);

            if (distanceNew < distanceCurrent){
                
                closestX = aFixedCharacterSlots[i].gameTexture.destRect.x;
                closestY = aFixedCharacterSlots[i].gameTexture.destRect.y;
                //printf("player %d set to %d\n",iLazyPassthrough,i);
                aPlayerSelectionIndex[iLazyPassthrough] = i;
                iLastUp[iLazyPassthrough] = i;
            }
        }
    }
    //printf("Found %d cards Above\n",tot);
}

void CSSMenu::querryFixedCssSlotPosition(int indexLocation, int *xptr, int *yptr){
    if (indexLocation < getSlotsLength()){
        *xptr = aFixedCharacterSlots[indexLocation].gameTexture.destRect.x;
        *yptr = aFixedCharacterSlots[indexLocation].gameTexture.destRect.y;
    } else {
        printf("CSSMenu::querryFixedCssSlotPosition --> Index out of range!\n");
    }
}

void CSSMenu::select(){
    MobileCharacterSlot tmpSlot;
    tmpSlot.gameTexture = aFixedCharacterSlots[aPlayerSelectionIndex[iLazyPassthrough]].gameTexture;
    
    switch (iLazyPassthrough)
    {
    case 0:
        tmpSlot.setTarget(78,604,.8,.8);
        break;
    case 1:
        tmpSlot.setTarget(1204,604,.8,.8);
        break;
    default:
        printf("oof\n");
        break;
    }
    tmpSlot.gameTexture.setScaleFactor(1);
    aMobileCharacterSlots[iLazyPassthrough] = tmpSlot;
}

void CSSMenu::back(){
    MobileCharacterSlot tmpSlot;
    aMobileCharacterSlots[iLazyPassthrough] = tmpSlot;
}

bool FixedCharacterSlot::isInitialized(){
    return bInitialized;
}

void FixedCharacterSlot::init(int id){
    gameTexture.init("resource/ui/menu/css/chara/default/render.png");
    iCharacterId = id;
    //printf("Character Id %d was initialized!\n",id);
    bInitialized = true;
}

void FixedCharacterSlot::setYPosition(int y){
    gameTexture.destRect.y = y;
}

void FixedCharacterSlot::setXPosition(int x){
    gameTexture.destRect.x = x;
    //printf("X Set to:%d\n",x);
}

int FixedCharacterSlot::getTextureWidth(){
    return gameTexture.destRect.w;
}

void FixedCharacterSlot::render(){
    gameTexture.render();
}

bool FixedCharacterSlot::isBelow(int y){
    if (gameTexture.destRect.y > y){
        return true;
    } else {
        return false;
    }
}

bool FixedCharacterSlot::isAbove(int y){
    if (gameTexture.destRect.y < y){
        return true;
    } else {
        return false;
    }
}

void CssCursor::render(){
    partialX += (iXTarget-partialX)/8;
    partialY += (iYTarget-partialY)/8;
    cursorTexture.destRect.x = partialX;
    cursorTexture.destRect.y = partialY;
    cursorTexture.render();
};
void CssCursor::init(string sTexturePath){
    cursorTexture.init(sTexturePath);
};


void CssCursor::setTarget(int x, int y){
    iXTarget = x;
    iYTarget = y;
}

void MobileCharacterSlot::playAnim(){

    //spaghetti code which makes it rotate once
    iAnimTime++;
    if(cos(fTheta) < 0){
        fTheta = -3.14/2;
    }
    if (iAnimTime < iAnimationSpeed){
        fTheta += (3.14)/iAnimationSpeed;
        gameTexture.setHorizontalScaleFactor(cos(fTheta) + fHorizontalscaleOffset);
        gameTexture.setVerticalScaleFactor(fVerticalscale);
    } else {
        gameTexture.setScaleFactor(1);
    }
    //

    fVerticalscale += (1 - fVerticalscale) / (iAnimationSpeed/3);
    fHorizontalscaleOffset += (0 - fHorizontalscaleOffset) / (iAnimationSpeed/3);
    fPosX += (targetX - fPosX) / (iAnimationSpeed/2);
    fPosY += (targetY - fPosY) / (iAnimationSpeed/2);

    gameTexture.destRect.x = fPosX;
    gameTexture.destRect.y = fPosY;
   
}

void MobileCharacterSlot::setTarget(int x, int y, float w, float h){
    targetX = x;
    targetY = y;

    fPosX = gameTexture.destRect.x;
    fPosY = gameTexture.destRect.y;

    fHorizontalscaleOffset = w - 1;
    fVerticalscale = h;
}


int cssMenu(PlayerInfo aPlayerInfo[2]){
    printf("Entering CSS Menu\n");
    bool bSelecting = true;
    Uint32 tick=0,tok=0;
    const Uint8* keyboard_state;
	Debugger debugger;
    SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    printf("Initializing Menu\n");
    CSSMenu cssMenuInstance;

    printf("Init Cursors\n");
    CssCursor cursors[2];
    cursors[0].init("resource/ui/menu/css/p1Cursor.png");
    cursors[1].init("resource/ui/menu/css/p2Cursor.png");
    cursors[0].cursorTexture.setScaleFactor(.8);
    cursors[0].cursorTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);
    cursors[1].cursorTexture.setScaleFactor(.8);
    cursors[1].cursorTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

    MenuHandler menuHandler(&cssMenuInstance,aPlayerInfo);

    menuHandler.setEventMenuLeft(&CSSMenu::traverseLeft);
	menuHandler.setEventMenuRight(&CSSMenu::traverseRight);
    menuHandler.setEventMenuDown(&CSSMenu::traverseDown);
    menuHandler.setEventMenuUp(&CSSMenu::traverseUp);
    menuHandler.setEventMenuFinish(&CSSMenu::select);
    menuHandler.setEventMenuBack(&CSSMenu::back);

	menuHandler.setInitialDelay(70);
	menuHandler.setRepeatDelay(20);

    printf("Entering loop\n");
    while (bSelecting){
        //bSelecting=false;
        //printf("Loop Start\n");
        
        frameTimeDelay(&tick,&tok);

        SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

        /* spaghetti time */
        { 
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
			if (SDL_GetWindowFlags(g_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(g_window, 0);
			}
			else {
				SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}
        }
        /*End Of Spaghetti Time*/
        
        //printf("CssHandler\n");
        menuHandler.handleCSSMenu();

        SDL_SetRenderTarget(g_renderer,pScreenTexture);
        SDL_RenderClear(g_renderer);

        cssMenuInstance.render();

        //printf("Cursor Render\n");
        for (int i=0;i<2;i++){
            cssMenuInstance.querryFixedCssSlotPosition(cssMenuInstance.aPlayerSelectionIndex[i], &cursors[i].iXTarget,&cursors[i].iYTarget);
            cursors[i].render();
        }
        //printf("Ending Loop\n");

        {   
        SDL_SetRenderTarget(g_renderer,nullptr);
        SDL_RenderClear(g_renderer);
        SDL_RenderCopy(g_renderer,pScreenTexture,nullptr,nullptr);
        SDL_RenderPresent(g_renderer);
        }
    }

    return 333;
}