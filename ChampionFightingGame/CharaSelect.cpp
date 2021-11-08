#include <cmath>
#include "CharaSelect.h"
#include "utils.h"
#include "Debugger.h"
#include "GameTexture.h"
#include "MenuHandler.h"
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

int chara_select_main(PlayerInfo player_info[2]) {
	Uint32 tick = 0, tok = 0;
	const Uint8* keyboard_state;
	Debugger debugger;
	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	CSS cssMenuInstance;
	if (cssMenuInstance.loadCSS()) {
		player_info[0].crash_reason = "Could not open CSS file!";
		return GAME_STATE_DEBUG_MENU;
	}


	printf("Init Cursors\n");
	CssCursor cursors[2];
	cursors[0].init("resource/ui/menu/css/p1Cursor.png");
	cursors[1].init("resource/ui/menu/css/p2Cursor.png");
	cursors[0].cursorTexture.setScaleFactor(0.8);
	cursors[0].cursorTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);
	cursors[1].cursorTexture.setScaleFactor(0.8);
	cursors[1].cursorTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

	MenuHandler menuHandler(&cssMenuInstance, player_info);

	menuHandler.setEventMenuLeft(&CSS::traverseLeft);
	menuHandler.setEventMenuRight(&CSS::traverseRight);
	menuHandler.setEventMenuDown(&CSS::traverseDown);
	menuHandler.setEventMenuUp(&CSS::traverseUp);
	menuHandler.setEventMenuFinish(&CSS::start);
	menuHandler.setEventMenuSelect(&CSS::select);
	menuHandler.setEventMenuBack(&CSS::back);

	menuHandler.setInitialDelay(70);
	menuHandler.setRepeatDelay(20);

	while (cssMenuInstance.bSelecting) {
		frameTimeDelay(&tick, &tok);

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
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

		menuHandler.handleCSS();

		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		SDL_RenderClear(g_renderer);

		cssMenuInstance.render();

		for (int i = 0; i < 2; i++) {
			cssMenuInstance.queryFixedCssSlotPosition(cssMenuInstance.aPlayerSelectionIndex[i], &cursors[i].iXTarget, &cursors[i].iYTarget);
			cursors[i].render();
		}

		SDL_SetRenderTarget(g_renderer, nullptr);
		SDL_RenderClear(g_renderer);
		SDL_RenderCopy(g_renderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(g_renderer);
	}

	player_info[0].chara_kind = cssMenuInstance.getCharacterKind(0);
	player_info[1].chara_kind = cssMenuInstance.getCharacterKind(1);

	return cssMenuInstance.getExitCode();
}

CSS::CSS() {
	//initialize other textures
	backgroundTexture.init("resource/ui/menu/css/CSSbackground.png");
	backgroundTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
	bigBarTexture.init("resource/ui/menu/css/CSSbottombar.png");
	bigBarTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
	topBarTexture.init("resource/ui/menu/css/CSStopbar.png");
	topBarTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);
}

int CSS::loadCSS() {
	ifstream fileCssTable;
	fileCssTable.open("resource/ui/menu/css/css_param.yml");
	int iCharacterKind;
	string sCharacterDir;
	bool bSelectable;
	FixedCharacterSlot* tmpSlot;
	int iRowXdelta = 0;

	if (fileCssTable.fail()) {
		fileCssTable.close();
		return -1;
	}

	string sCharacterName;
	for (int i = 0; getline(fileCssTable, sCharacterName); i++) {
		fileCssTable >> iCharacterKind >> sCharacterDir >> bSelectable;
		if (bSelectable) {
			addFixedCharacter(iCharacterKind, sCharacterDir);
		}
		getline(fileCssTable, sCharacterName); //100% authentic jank code
	}

	if (getSlotsLength() > 10) {
		for (int iRow = 1; iRow < 4; iRow++) {
			//Following line checks if the row is filled (the tenth item in the row is filled)
			if (aFixedCharacterSlots[iRow * 10 - 1].isInitialized()) {
				//row filled code
				for (int iColumn = 0; iColumn < 10; iColumn++) {
					tmpSlot = &aFixedCharacterSlots[((iRow - 1) * 10) + iColumn];

					tmpSlot->gameTexture.setScaleFactor(0.8);
					tmpSlot->gameTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

					/*
						(WINDOW_WIDTH/10) is for the spacing between the cards
						(WINDOW_WIDTH/20) is for the offset to center the row, it should always be 1/2 the previous one
					*/

					tmpSlot->setXPosition(iColumn * (WINDOW_WIDTH / 10) + (WINDOW_WIDTH / 20));
					tmpSlot->setYPosition(iRow * (tmpSlot->gameTexture.getScaledHeight() + 20));
				}
			}
			else {
			 //row not filled code
				for (int iColumn = 0; iColumn < getSlotsLength() % 10; iColumn++) {
					tmpSlot = &aFixedCharacterSlots[((iRow - 1) * 10) + iColumn];

					tmpSlot->gameTexture.setScaleFactor(0.8);
					tmpSlot->gameTexture.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_CENTER);

					/*
						Basically.....
						Calculate the difference in x positions of the first and last card as iRowXdelta
						Divide iRowXdelta by 2 and subtract the WINDOW_WIDTH by iRowXdelta to get the starting point
						from which to render the row.
					*/
					iRowXdelta = (WINDOW_WIDTH / 10) * (getSlotsLength() % 10) - (WINDOW_WIDTH / 10); //yes this gets recalculated every frame, no im not moving it
					tmpSlot->setXPosition(((WINDOW_WIDTH - iRowXdelta) / 2) + iColumn * (WINDOW_WIDTH / 10));
					tmpSlot->setYPosition(iRow * (tmpSlot->gameTexture.getScaledHeight() + 20));
				}
			}
		}
	}

	fileCssTable.close();

	return 0;
}

void CSS::addFixedCharacter(int id, string cardDir) {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (!aFixedCharacterSlots[i].isInitialized()) {
			aFixedCharacterSlots[i].init(id, cardDir);
			return;
		}
	}
}

int CSS::getSlotsLength() {
	int size = 0;
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (aFixedCharacterSlots[i].isInitialized()) {
			size++;
		}
	}
	return size;
}

void CSS::select() {
	MobileCharacterSlot tmpSlot;
	tmpSlot.gameTexture = aFixedCharacterSlots[aPlayerSelectionIndex[playerID]].gameTexture;

	switch (playerID) {
		case 0:
			tmpSlot.setTarget(78, 604, .8, .8);
			break;
		case 1:
			tmpSlot.setTarget(1204, 604, .8, .8);
			break;
		default:
			printf("oof\n");
			break;
	}
	tmpSlot.gameTexture.setScaleFactor(1);
	aMobileCharacterSlots[playerID] = tmpSlot;
}

void CSS::back() {
	MobileCharacterSlot tmpSlot;
	aMobileCharacterSlots[playerID] = tmpSlot;
}

void CSS::start() {
	if (aMobileCharacterSlots[0].gameTexture.bIsInitialized && aMobileCharacterSlots[1].gameTexture.bIsInitialized) {
		iExitCode = GAME_STATE_GAME;
		bSelecting = false;
	}
}

void CSS::traverseRight() {
	if (aPlayerSelectionIndex[playerID] % 10 <= 8 && aPlayerSelectionIndex[playerID] + 1 < getSlotsLength() && aFixedCharacterSlots[aPlayerSelectionIndex[playerID] + 1].isInitialized()) {
		aPlayerSelectionIndex[playerID]++;
	}
	isLastRight[playerID] = true;
	iLastUp[playerID] = -1;
	iLastDown[playerID] = -1;
}

void CSS::traverseLeft() {
	if (aPlayerSelectionIndex[playerID] % 10 >= 1) {
		aPlayerSelectionIndex[playerID]--;
	}
	isLastRight[playerID] = false;
	iLastDown[playerID] = -1;
	iLastUp[playerID] = -1;
}

void CSS::traverseDown() {
	int tmpCurrentX;
	int tmpCurrentY; // we need to know where the current location is

	int closestX = 9999;
	int closestY = 9999;

	int distanceNew, distanceCurrent;

	if (iLastDown[playerID] != -1) {
		aPlayerSelectionIndex[playerID] = iLastDown[playerID];
		return;
	}

	queryFixedCssSlotPosition(aPlayerSelectionIndex[playerID], &tmpCurrentX, &tmpCurrentY);

	tmpCurrentX += 30; //magic sauce

	int num_slots = getSlotsLength();
	int index = aPlayerSelectionIndex[playerID];
	int row = floor(index / 10);
	int num_rows = floor(num_slots / 10);

	cout << "Row: " << row << ", Total Rows: " << num_rows << endl;

	//Go through every possible card, if they are initialized and below the current pos the add them to the list
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (aFixedCharacterSlots[i].isInitialized() && aFixedCharacterSlots[i].isBelow(tmpCurrentY)) {
			distanceNew = twoPointDistance(aFixedCharacterSlots[i].gameTexture.destRect.x, aFixedCharacterSlots[i].gameTexture.destRect.y, tmpCurrentX, tmpCurrentY);
			distanceCurrent = twoPointDistance(closestX, closestY, tmpCurrentX, tmpCurrentY);
			if (distanceNew < distanceCurrent) {
				closestX = aFixedCharacterSlots[i].gameTexture.destRect.x;
				closestY = aFixedCharacterSlots[i].gameTexture.destRect.y;
				aPlayerSelectionIndex[playerID] = i;
				iLastDown[playerID] = i;
			}
		}
	}
/*
	This code is still a wee bit broken, you can try fixing it if you want but I'll try figuring it out
	
	if ((num_slots % 10) % 2) {
		if (row == num_rows - 1) {
			if (isLastRight[playerID] && aPlayerSelectionIndex[playerID] < getSlotsLength()) {
				aPlayerSelectionIndex[playerID]++;
				cout << "Adding one" << endl;
			}
			cout << "Inverting" << endl;
			isLastRight[playerID] = !isLastRight[playerID];
		}
		else {
			cout << "No more Right" << endl;
			isLastRight[playerID] = false;
		}
	}*/
}

void CSS::traverseUp() {
	int tmpCurrentX;
	int tmpCurrentY; // we need to know where the current location is

	int closestX = 9999;
	int closestY = 9999;

	int distanceNew, distanceCurrent;

	if (iLastUp[playerID] != -1) {
		aPlayerSelectionIndex[playerID] = iLastUp[playerID];
		return;
	}

	queryFixedCssSlotPosition(aPlayerSelectionIndex[playerID], &tmpCurrentX, &tmpCurrentY);

	int num_slots = getSlotsLength();
	int index = aPlayerSelectionIndex[playerID];
	int row = floor(index / 10);
	int num_rows = floor(num_slots / 10);
	cout << "Row: " << row << ", Total Rows: " << num_rows << endl;

	//Go through every possible card, if they are initialized and below the current pos the add them to the list
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (aFixedCharacterSlots[i].isInitialized() && aFixedCharacterSlots[i].isAbove(tmpCurrentY)) {
			distanceNew = twoPointDistance(aFixedCharacterSlots[i].gameTexture.destRect.x, aFixedCharacterSlots[i].gameTexture.destRect.y, tmpCurrentX, tmpCurrentY);
			distanceCurrent = twoPointDistance(closestX, closestY, tmpCurrentX, tmpCurrentY);


			if (distanceNew < distanceCurrent) {

				closestX = aFixedCharacterSlots[i].gameTexture.destRect.x;
				closestY = aFixedCharacterSlots[i].gameTexture.destRect.y;
				aPlayerSelectionIndex[playerID] = i;
				iLastUp[playerID] = i;
			}
		}
	}
	/*
	if ((num_slots % 10) % 2) {
		if (row == num_rows) {
			if (isLastRight[playerID] && aPlayerSelectionIndex[playerID] < getSlotsLength()) {
				aPlayerSelectionIndex[playerID]++;
				cout << "Adding one" << endl;
			}
			cout << "Inverting" << endl;
			isLastRight[playerID] = !isLastRight[playerID];
		}
		else {
			cout << "No more Right" << endl;
			isLastRight[playerID] = false;
		}
	}
	*/
}

void CSS::render() {
	backgroundTexture.render();
	bigBarTexture.render();
	topBarTexture.render();

	for (int i = 0; i < CSS_SLOTS; i++) {
		if (aFixedCharacterSlots[i].isInitialized()) {
			aFixedCharacterSlots[i].render();
		}
	}


	for (int i = 0; i < 2; i++) {

		aMobileCharacterSlots[i].playAnim();
		aMobileCharacterSlots[i].gameTexture.render();
	}
}

void CSS::queryFixedCssSlotPosition(int indexLocation, int* xptr, int* yptr) {
	if (indexLocation < getSlotsLength()) {
		*xptr = aFixedCharacterSlots[indexLocation].gameTexture.destRect.x;
		*yptr = aFixedCharacterSlots[indexLocation].gameTexture.destRect.y;
	}
	else {
		printf("CSS::queryFixedCssSlotPosition --> Index out of range!\n");
	}
}

int CSS::getExitCode() {
	return iExitCode;
}

int CSS::getCharacterKind(int player) {
	return aFixedCharacterSlots[aPlayerSelectionIndex[player]].getCharacterId();
}

int FixedCharacterSlot::getCharacterId() {
	return iCharacterId;
}

bool FixedCharacterSlot::isInitialized() {
	return bInitialized;
}

void FixedCharacterSlot::init(int id, string textureDir) {
	gameTexture.init("resource/ui/menu/css/chara/" + textureDir + "/render.png");
	iCharacterId = id;
	bInitialized = true;
}

void FixedCharacterSlot::setYPosition(int y) {
	gameTexture.destRect.y = y;
}

void FixedCharacterSlot::setXPosition(int x) {
	gameTexture.destRect.x = x;
}

int FixedCharacterSlot::getTextureWidth() {
	return gameTexture.destRect.w;
}

void FixedCharacterSlot::render() {
	gameTexture.render();
}

bool FixedCharacterSlot::isBelow(int y) {
	if (gameTexture.destRect.y > y) {
		return true;
	}
	else {
		return false;
	}
}

bool FixedCharacterSlot::isAbove(int y) {
	if (gameTexture.destRect.y < y) {
		return true;
	}
	else {
		return false;
	}
}

void CssCursor::render() {
	partialX += (iXTarget - partialX) / 8;
	partialY += (iYTarget - partialY) / 8;
	cursorTexture.destRect.x = partialX;
	cursorTexture.destRect.y = partialY;
	cursorTexture.render();
};
void CssCursor::init(string sTexturePath) {
	cursorTexture.init(sTexturePath);
};


void CssCursor::setTarget(int x, int y) {
	iXTarget = x;
	iYTarget = y;
}

void MobileCharacterSlot::playAnim() {
	//spaghetti code which makes it rotate once
	iAnimTime++;
	if (cos(fTheta) < 0) {
		fTheta = -3.14 / 2;
	}
	if (iAnimTime < iAnimationSpeed) {
		fTheta += (3.14) / iAnimationSpeed;
		gameTexture.setHorizontalScaleFactor(cos(fTheta) + fHorizontalscaleOffset);
		gameTexture.setVerticalScaleFactor(fVerticalscale);
	}
	else {
		gameTexture.setScaleFactor(1);
	}

	fVerticalscale += (1 - fVerticalscale) / (iAnimationSpeed / 3);
	fHorizontalscaleOffset += (0 - fHorizontalscaleOffset) / (iAnimationSpeed / 3);
	fPosX += (targetX - fPosX) / (iAnimationSpeed / 2);
	fPosY += (targetY - fPosY) / (iAnimationSpeed / 2);

	gameTexture.destRect.x = fPosX;
	gameTexture.destRect.y = fPosY;

}

void MobileCharacterSlot::setTarget(int x, int y, float w, float h) {
	targetX = x;
	targetY = y;

	fPosX = gameTexture.destRect.x;
	fPosY = gameTexture.destRect.y;

	fHorizontalscaleOffset = w - 1;
	fVerticalscale = h;
}