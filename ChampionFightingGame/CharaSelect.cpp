#include <cmath>
#include "CharaSelect.h"
#include "utils.h"
#include "Debugger.h"
#include "GameTexture.h"
#include "Loader.h"

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

void chara_select_main(GameManager* game_manager) {
	PlayerInfo *player_info[2];
	player_info[0] = game_manager->player_info[0];
	player_info[1] = game_manager->player_info[1];
	const Uint8* keyboard_state;
	Debugger debugger;
	debugger = Debugger();


	CharaSelectLoader* chara_select_loader = new CharaSelectLoader;
	chara_select_loader->player_info[0] = player_info[0];
	chara_select_loader->player_info[1] = player_info[1];


	SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	bool loading = true;

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

	CSS css;
	CssCursor cursors[2];

	SDL_Thread* loading_thread;

	loading_thread = SDL_CreateThread(LoadCharaSelect, "Init.rar", (void*)chara_select_loader);
	SDL_DetachThread(loading_thread);

	game_manager->set_menu_info(nullptr);

	LoadIcon load_icon;
	GameTexture loadingSplash, loadingFlavor, loadingBar;
	loadingSplash.init("resource/ui/menu/loading/splashload.png");
	loadingSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingFlavor.init("resource/ui/menu/loading/FlavorBar.png");
	loadingFlavor.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingBar.init("resource/ui/menu/loading/loadingbar.png");
	loadingBar.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_METER);

	while (loading) {
		frameTimeDelay();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
					return;
				}
				break;
			}
		}

		load_icon.move();
		SDL_LockMutex(mutex);

		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		loadingSplash.render();
		int total_items = 3;
		loadingBar.setTargetPercent(((float)chara_select_loader->loaded_items / total_items), 0.3);
		loadingBar.render();
		loadingFlavor.render();
		load_icon.texture.render();

		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
		SDL_RenderPresent(g_renderer);

		SDL_UnlockMutex(mutex);

		if (chara_select_loader->finished) {
			if (!chara_select_loader->can_ret) {
				css = chara_select_loader->css;
				for (int i = 0; i < 2; i++) {
					cursors[i] = chara_select_loader->css_cursor[i];
					player_info[i] = chara_select_loader->player_info[i];
				}
				game_manager->set_menu_info(&css);
			}
			chara_select_loader->can_ret = true;	
			loading = false;
		}
	}
	SDL_SetRenderTarget(g_renderer, pScreenTexture);
	SDL_RenderClear(g_renderer);
	SDL_SetRenderTarget(g_renderer, NULL);
	SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	while (*game_manager->looping[game_manager->layer]) {
		frameTimeDelay();
		for (int i = 0; i < 2; i++) {
			player_info[i]->check_controllers();
		}
		SDL_RenderClear(g_renderer);
		SDL_SetRenderDrawColor(g_renderer, 100, 100, 100, 255);

		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
				{
					return game_manager->update_state(GAME_STATE_CLOSE);
				} break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < 2; i++) {
			player_info[i]->poll_buttons(keyboard_state);
		}
		if (debugger.check_button_trigger(BUTTON_DEBUG_FULLSCREEN)) {
			if (SDL_GetWindowFlags(g_window) & SDL_WINDOW_FULLSCREEN_DESKTOP) {
				SDL_SetWindowFullscreen(g_window, 0);
			}
			else {
				SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
		}
		for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}

		game_manager->handle_menus();

		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		SDL_RenderClear(g_renderer);

		css.render();

		for (int i = 0; i < 2; i++) {
			css.queryFixedCssSlotPosition(css.aPlayerSelectionIndex[i], &cursors[i].iXTarget, &cursors[i].iYTarget);
			cursors[i].render();
		}

		SDL_SetRenderTarget(g_renderer, nullptr);
		SDL_RenderClear(g_renderer);
		SDL_RenderCopy(g_renderer, pScreenTexture, nullptr, nullptr);
		SDL_RenderPresent(g_renderer);
	}

	delete chara_select_loader;
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
			addFixedCharacter(iCharacterKind, sCharacterDir, sCharacterName);
		}
		getline(fileCssTable, sCharacterName); //100% authentic jank code
	}
	fileCssTable.close();

	for (int iRow = 1; iRow < 4; iRow++) {
		//Following line checks if the row is filled (the tenth item in the row is filled)
		if (aFixedCharacterSlots[iRow * 10 - 1].isInitialized()) {
			//row filled code
			for (int iColumn = 0; iColumn < 10; iColumn++) {
				tmpSlot = &aFixedCharacterSlots[((iRow - 1) * 10) + iColumn];

				tmpSlot->gameTexture.setScaleFactor(1.2);
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

				tmpSlot->gameTexture.setScaleFactor(1.2);
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

	//Anyway so we're also using a 2D array now

	int col = 0;
	int row = 0;
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (aFixedCharacterSlots[i].isInitialized()) {
			aFixedCharacterSlots[i].myCol = col;
			aFixedCharacterSlots[i].myRow = row;
			charaSlotsOrdered[col][row] = aFixedCharacterSlots[i];
			if (col == 9) {
				col = 0;
				row++;
			}
			else {
				col++;
			}
		}
	}
	numCols = col;
	numRows = row;
	centerSlots();

	return 0;
}

void CSS::addFixedCharacter(int id, string cardDir, string cardName) {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (!aFixedCharacterSlots[i].isInitialized()) {
			aFixedCharacterSlots[i].init(id, cardDir, cardName);
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

void CSS::event_select_press() {
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		player_info[player_id]->chara_kind = aFixedCharacterSlots[aPlayerSelectionIndex[player_id]].getCharacterId();
		MobileCharacterSlot tmpSlot;
		tmpSlot.gameTexture = aFixedCharacterSlots[aPlayerSelectionIndex[player_id]].gameTexture;

		switch (player_id) {
			case 0:
				tmpSlot.setTarget(117, 906, 1.5, 1.5);
				break;
			case 1:
				tmpSlot.setTarget(1806, 906, 1.5, 1.5);
				break;
			default:
				printf("oof\n");
				break;
		}
		tmpSlot.gameTexture.setScaleFactor(1.5);
		aMobileCharacterSlots[player_id] = tmpSlot;
	}
}
void CSS::event_back_press() {
	if (player_info[player_id]->chara_kind != CHARA_KIND_MAX) {
		MobileCharacterSlot tmpSlot;
		aMobileCharacterSlots[player_id] = tmpSlot;
		player_info[player_id]->chara_kind = CHARA_KIND_MAX;
	}
	else {
		displayLoadingScreen();
		*game_state = GAME_STATE_MENU;
		*looping = false;
	}
}

void CSS::event_start_press() {
	if (player_info[0]->chara_kind != CHARA_KIND_MAX && player_info[1]->chara_kind != CHARA_KIND_MAX) {
		displayLoadingScreen();
		*game_state = GAME_STATE_BATTLE;
		*looping = false;
	}
}

void CSS::event_right_press() {
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (myCol[player_id] != 9 && charaSlotsOrdered[myCol[player_id] + 1][myRow[player_id]].isInitialized()) {
			myCol[player_id]++;
		}
		isLastRight[player_id] = true;
		selectIndex();
	}
}

void CSS::event_left_press() {
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (myCol[player_id] != 0 && charaSlotsOrdered[myCol[player_id] - 1][myRow[player_id]].isInitialized()) {
			myCol[player_id]--;
		}
		isLastRight[player_id] = false;
		selectIndex();
	}
}

void CSS::event_down_press() {
	bool jump = false;
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (myRow[player_id] < numRows) {
			if (!charaSlotsOrdered[myCol[player_id]][myRow[player_id] + 1].isInitialized()) {
				jump = true;
				bool valid_col = false;
				if (myCol[player_id] >= 5) {
					for (int i = NUM_COLS - 1; i > 0; i--) {
						if (charaSlotsOrdered[i][myRow[player_id] + 1].isInitialized()) {
							myCol[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				else {
					for (int i = 0; i < NUM_COLS; i++) {
						if (charaSlotsOrdered[i][myRow[player_id] + 1].isInitialized()) {
							myCol[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				if (!valid_col) {
					player_info[player_id]->crash_reason = "Couldn't find a valid column!";
					*looping = false;
					*game_state = GAME_STATE_DEBUG_MENU;
					return;
				}
			}
			myRow[player_id]++;
			bool min = myCol[player_id] <= colsOffset / 2;

			if (numCols % 2) {
				if (!isLastRight[player_id] && myRow[player_id] == numRows && !min) {
					myCol[player_id]--;
				}
				if (!jump) {
					isLastRight[player_id] = !isLastRight[player_id];
				}
			}
		}

		selectIndex();
	}
}

void CSS::event_up_press() {
	bool jump = false;
	if (player_info[player_id]->chara_kind == CHARA_KIND_MAX) {
		if (myRow[player_id] != 0) {
			if (!charaSlotsOrdered[myCol[player_id]][myRow[player_id] - 1].isInitialized()) {
				jump = true;
				bool valid_col = false;
				if (myCol[player_id] >= 5) {
					for (int i = NUM_COLS - 1; i > 0; i--) {
						if (charaSlotsOrdered[i][myRow[player_id] - 1].isInitialized()) {
							myCol[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				else {
					for (int i = 0; i < NUM_COLS; i++) {
						if (charaSlotsOrdered[i][myRow[player_id] - 1].isInitialized()) {
							myCol[player_id] = i;
							valid_col = true;
							break;
						}
					}
				}
				if (!valid_col) {
					player_info[player_id]->crash_reason = "Couldn't find a valid column!";
					*looping = false;
					*game_state = GAME_STATE_DEBUG_MENU;
					return;
				}
			}
			myRow[player_id]--;
			bool max = myCol[player_id] >= NUM_COLS - 1;

			if (numCols % 2) {
				if (isLastRight[player_id] && myRow[player_id] == numRows - 1 && !max) {
					myCol[player_id]++;
				}
				if (!jump) {
					isLastRight[player_id] = !isLastRight[player_id];
				}
			}
		}

		selectIndex();
	}
}

void CSS::selectIndex() {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (myCol[player_id] == aFixedCharacterSlots[i].myCol
		&& myRow[player_id] == aFixedCharacterSlots[i].myRow) {
			aPlayerSelectionIndex[player_id] = i;
			break;
		}
	}
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
		FixedCharacterSlot tmpSlot;
		tmpSlot.gameTexture = aFixedCharacterSlots[aPlayerSelectionIndex[i]].gameTexture;
		tmpSlot.name = aFixedCharacterSlots[aPlayerSelectionIndex[i]].name;
		tmpSlot.textureDir = aFixedCharacterSlots[aPlayerSelectionIndex[i]].textureDir;

		switch (i) {
			case 0:
				tmpSlot.gameTexture.destRect.x = 117;
				tmpSlot.gameTexture.destRect.y = 906;
				break;
			case 1:
				tmpSlot.gameTexture.destRect.x = 1806;
				tmpSlot.gameTexture.destRect.y = 906;
				break;
			default:
				cout << "How the fuck" << endl;
				break;
		}
		tmpSlot.gameTexture.setScaleFactor(1.5);
		tmpSlot.gameTexture.setAlpha((Uint8)127);
		tmpSlot.gameTexture.render();
		tmpSlot.gameTexture.setAlpha((Uint8)255);
		if (tmpSlot.textureDir == "default") {
			draw_text_multi_lines("FiraCode-Regular.ttf", tmpSlot.name, tmpSlot.gameTexture.destRect.x, tmpSlot.gameTexture.destRect.y + 70, 24, 255, 255, 255, 255);
		}

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

int CSS::getCharacterKind(int player) {
	return aFixedCharacterSlots[aPlayerSelectionIndex[player]].getCharacterId();
}

void CSS::centerSlots() {
	int empty_cols = 1;
	int empty_row;
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int i2 = 0; i2 < NUM_COLS; i2++) {
			if (!charaSlotsOrdered[i2][i].isInitialized()) { //If this slot is empty
				if (charaSlotsOrdered[0][i].isInitialized()) { //If the first slot on that row is full
					empty_cols = NUM_COLS - i2;
					empty_row = i;
					goto ENDL;
				}
			}
		}
	}
	ENDL: //im so funny
	
	colsOffset = empty_cols;

	for (int i = 0; i < CSS_SLOTS; i++) {
		if (aFixedCharacterSlots[i].myRow == empty_row) {
			aFixedCharacterSlots[i].myCol += (empty_cols / 2);
		}
	}
	for (int i = 0; i < NUM_COLS; i++) {
		for (int i2 = 0; i2 < NUM_ROWS; i2++) {
			charaSlotsOrdered[i][i2].myCol = -1;
			charaSlotsOrdered[i][i2].myRow = -1;
			charaSlotsOrdered[i][i2].bInitialized = false;
			for (int i3 = 0; i3 < CSS_SLOTS; i3++) {
				if (aFixedCharacterSlots[i3].myCol == i
				&& aFixedCharacterSlots[i3].myRow == i2) {
					charaSlotsOrdered[i][i2] = aFixedCharacterSlots[i3];
					break;
				}
			}
		}
	}
}

void CSS::findPrevChara(int chara_kind) {
	for (int i = 0; i < CSS_SLOTS; i++) {
		if (aFixedCharacterSlots[i].iCharacterId == chara_kind) {
			player_info[player_id]->chara_kind = CHARA_KIND_MAX;
			myCol[player_id] = aFixedCharacterSlots[i].myCol;
			myRow[player_id] = aFixedCharacterSlots[i].myRow;
			selectIndex();
			event_select_press();
			return;
		}
	}
}

int FixedCharacterSlot::getCharacterId() {
	return iCharacterId;
}

bool FixedCharacterSlot::isInitialized() {
	return bInitialized;
}

void FixedCharacterSlot::init(int id, string textureDir, string name) {
	gameTexture.init("resource/ui/menu/css/chara/" + textureDir + "/render.png");
	this->name = name;
	this->textureDir = textureDir;
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
		gameTexture.setScaleFactor(1.5);
	}

	fVerticalscale += (1.5 - fVerticalscale) / (iAnimationSpeed / 3);
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

	fHorizontalscaleOffset = w - 1.5;
	fVerticalscale = h;
}