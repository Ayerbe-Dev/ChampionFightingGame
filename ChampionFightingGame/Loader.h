#pragma once
#include <SDL/SDL.h>
#include <iostream>
#include <thread>
#include "utils.h"
extern SDL_Window* g_window;

extern SDL_mutex* file_mutex;
extern SDL_Renderer* g_renderer;

enum {
	GAME_TEXTURE_ANCHOR_MODE_DEFAULT,
	GAME_TEXTURE_ANCHOR_MODE_CENTER,
	GAME_TEXTURE_ANCHOR_MODE_BACKGROUND,
	GAME_TEXTURE_ANCHOR_MODE_METER,
};

enum {
	TEXTURE_FLIP_KIND_NONE,
	TEXTURE_FLIP_KIND_DRAIN,
	TEXTURE_FLIP_KIND_NO_DRAIN,

	TEXTURE_FLIP_KIND_MAX,
};

enum {
	METER_DRAIN_KIND_RIGHT,
	METER_DRAIN_KIND_LEFT,
	METER_DRAIN_KIND_NONE,
};

class GameTextureSDL {
public:
	SDL_Rect destRect;
	SDL_Rect srcRect;

	bool render();
	bool init(std::string sTexturePath, bool delay = true);

	void setScaleFactor(float fScaleFactor);

	void setHorizontalScaleFactor(float fScaleFactor);
	void setVerticalScaleFactor(float fScaleFactor);

	void setAnchorMode(int iMode);
	float getScaledWidth();
	float getScaledHeight();
	void setAlpha(Uint8 alpha);
	void clearTexture();
	bool bIsInitialized = false;
	void setPercent(float percent);
	void setTargetPercent(float percent, float rate = 0.2, int frames = 15);
	void changePercent(float rate = -1.0);
	void setFlip(int flip);
	int getFlipKind();
	void setDrainKind(int drain_kind);
private:
	float percent{ 0 };
	float target_percent{ -1 };
	float target_rate{ -1 };
	int target_frames{ 1 };
	int iAnchorMode = GAME_TEXTURE_ANCHOR_MODE_DEFAULT;
	float fVerticalScaleFactor = 1.0;
	float fHorizontalScaleFactor = 1.0;
	int flip{ TEXTURE_FLIP_KIND_NONE };
	int drain_kind{ METER_DRAIN_KIND_NONE };

	SDL_Texture* pTexture;
};


class LoadIcon {
public:
	LoadIcon();

	GameTextureSDL texture;
	bool move_right;
	bool move_down;
	bool panic_v;
	bool panic_mode;
	float panic_speed;
	float panic_add;
	bool panic_setting; //True = Gradually slows down to a stop, False = Goes flying

	void move();
	void set_attributes();
	bool check_corner_distance(bool init);
};

class GameLoader {
public:
	GameLoader();
	GameLoader(int total_items);

	int loaded_items = 0;
	int total_items;
	int ret = 0;
	bool finished = false;
	bool can_ret = false;
};

static int LoadingScreen(void* void_GameLoader) {
	GameLoader* game_loader = (GameLoader*)void_GameLoader;

	SDL_Texture* pScreenTexture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetTextureBlendMode(pScreenTexture, SDL_BLENDMODE_BLEND);

	LoadIcon load_icon;
	GameTextureSDL loadingSplash, loadingFlavor, loadingBar;
	loadingSplash.init("resource/ui/menu/loading/splashload.png");
	loadingSplash.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingFlavor.init("resource/ui/menu/loading/FlavorBar.png");
	loadingFlavor.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_BACKGROUND);

	loadingBar.init("resource/ui/menu/loading/loadingbar.png");
	loadingBar.setAnchorMode(GAME_TEXTURE_ANCHOR_MODE_METER);

	while (!game_loader->finished) {
		wait_ms();

		load_icon.move();
		SDL_LockMutex(file_mutex);

		SDL_RenderClear(g_renderer);
		SDL_SetRenderTarget(g_renderer, pScreenTexture);
		loadingSplash.render();
		loadingBar.setTargetPercent(((float)game_loader->loaded_items / (float)game_loader->total_items), 0.3);
		loadingBar.render();
		loadingFlavor.render();
		load_icon.texture.render();

		SDL_SetRenderTarget(g_renderer, NULL);
		SDL_RenderCopy(g_renderer, pScreenTexture, NULL, NULL);
		SDL_RenderPresent(g_renderer);

		SDL_UnlockMutex(file_mutex);
	}

	loadingSplash.clearTexture();
	loadingBar.clearTexture();
	loadingFlavor.clearTexture();
	load_icon.texture.clearTexture();
	SDL_DestroyTexture(pScreenTexture);

	return 0;
}