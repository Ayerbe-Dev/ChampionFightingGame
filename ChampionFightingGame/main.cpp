#include <windows.h>
#include <chrono>
#include <thread>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_timer.h>
#include "utils.h"
#include "Game.h"
#include "Animation.h"
#include "Debugger.h"
#undef main
using namespace std;
int error_render;

Uint32 tick;
Uint32 tok;
u32 frame_advance_entry_ms;
u32 frame_advance_ms;
bool debug = false;


SDL_Texture *pBackgroundTexture;

int main()
{
	bool running = true;
	bool visualize_boxes = true;

	Debugger debugger;
	debugger = Debugger();
	SDL_Rect debug_rect[2] = { 0, 0, 0, 0 };

	GameCoordinate debug_anchor[2];
	GameCoordinate debug_offset[2];

	//init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window *window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer *pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);

	//Background image
	pBackgroundTexture = loadTexture("resource/stage/training_room/background.png", pRenderer);
	SDL_Texture *pScreenTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_Rect camera;

	//init players
	PlayerInfo *player_info[2];

	IFighter *p1 = new IFighter(CHARA_KIND_ROY, pRenderer, 0);
	IFighter *p2 = new IFighter(CHARA_KIND_ERIC, pRenderer, 1);

	player_info[0] = p1->get_fighter();
	player_info[1] = p2->get_fighter();

	const Uint8 *keyboard_state;
	tick = SDL_GetTicks();

	while (running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
			{
				running = false;
			}
			break;
			}
		}

		SDL_PumpEvents();
		keyboard_state = SDL_GetKeyboardState(NULL);
		for (int i = 0; i < BUTTON_MAX; i++)
		{
			for (int o = 0; o < 2; o++)
			{
				if (!debug)
				{
					bool old_button = player_info[o]->button_info[i].button_on;
					player_info[o]->button_info[i].button_on = keyboard_state[player_info[o]->button_info[i].mapping];
					bool new_button = player_info[o]->button_info[i].button_on;
					player_info[o]->button_info[i].changed = (old_button != new_button);
				}
			}
		}
		for (int i = 0; i < BUTTON_DEBUG_MAX; i++) {
			bool old_button = debugger.button_info[i].button_on;
			debugger.button_info[i].button_on = keyboard_state[debugger.button_info[i].mapping];
			bool new_button = debugger.button_info[i].button_on;
			debugger.button_info[i].changed = (old_button != new_button);
		}

		tok = SDL_GetTicks() - tick;
		//cout << "delta: " << tok << endl;
		if (tok < TICK_RATE_MS)
		{
			//cout << "delay: " << TICK_RATE_MS - tok << endl;
			SDL_Delay(TICK_RATE_MS - tok);
			//put code below in here
		}
		tick = SDL_GetTicks();

		/*
			Rendering. The method for rendering relies on setting render targets. pScreenTexture is where all the scalable
			textures are rendered. The GUI will be applied after the render target has been set back to the window.
			After RenderTarget has been set, nothing has to be done with pScreenTexture untill it is time to SDL_Present
			the content in pScreenTexture.
		*/
		SDL_RenderClear(pRenderer);
		SDL_SetRenderTarget(pRenderer, pScreenTexture);
		SDL_RenderCopy(pRenderer, pBackgroundTexture, nullptr, nullptr); //copies the background

		// Flip Player code, tbh it shouldnt be here
		for (int i = 0; i < 2; i++)
		{
			SDL_RendererFlip flip = SDL_FLIP_NONE;
			if (player_info[i]->situation_kind == CHARA_SITUATION_GROUND && player_info[i]->is_actionable())
			{
				if (player_info[i]->pos.x > player_info[!i]->pos.x)
				{
					player_info[i]->facing_dir = -1.0;
					player_info[i]->facing_right = false;
					flip = SDL_FLIP_HORIZONTAL;
				}
				else
				{
					player_info[i]->facing_dir = 1.0;
					player_info[i]->facing_right = true;
				}
			}
			else if (!player_info[i]->facing_right)
			{
				flip = SDL_FLIP_HORIZONTAL;
			}

			if (debugger.check_button_trigger(BUTTON_DEBUG_ENABLE) && i == 0) {
				debug = !debug;
			}
			if (!debug) {
				tickOnce(player_info[i], pRenderer);
				frame_advance_entry_ms = SDL_GetTicks();
			}
			else if (i == 0) {
				frame_advance_ms = SDL_GetTicks() - frame_advance_entry_ms;
				if (debugger.check_button_on(BUTTON_DEBUG_PICK_1)) {
					debugger.target = 0;
				}
				if (debugger.check_button_on(BUTTON_DEBUG_PICK_2)) {
					debugger.target = 1;
				}
				if (debugger.check_button_trigger(BUTTON_DEBUG_ADVANCE)) {
					for (int o = 0; o < BUTTON_MAX; o++) {
						bool old_button = player_info[0]->button_info[o].button_on;
						player_info[0]->button_info[o].button_on = keyboard_state[player_info[0]->button_info[o].mapping];
						bool new_button = player_info[0]->button_info[o].button_on;
						player_info[0]->button_info[o].changed = (old_button != new_button);
						old_button = player_info[1]->button_info[o].button_on;
						player_info[1]->button_info[o].button_on = keyboard_state[player_info[1]->button_info[o].mapping];
						new_button = player_info[1]->button_info[o].button_on;
						player_info[1]->button_info[o].changed = (old_button != new_button);
					}
					frame_advance_entry_ms = SDL_GetTicks();
					tickOnce(player_info[0], pRenderer);
					tickOnce(player_info[1], pRenderer);
					if (debugger.print_frames) {
						cout << "Player " << debugger.target + 1 << " Frame: " << player_info[debugger.target]->frame - 1 << endl;
						cout << "Player " << debugger.target + 1 << " Render Frame: " << player_info[debugger.target]->render_frame - 1 << endl;
					}
				}
				debug_mode(&debugger, player_info[debugger.target], pRenderer, &debug_rect[debugger.target], &debug_anchor[debugger.target], &debug_offset[debugger.target]);

			}

			SDL_Rect render_pos;
			render_pos.x = player_info[i]->pos.getRenderCoodrinateX();
			render_pos.y = player_info[i]->pos.getRenderCoodrinateY();
			int width;
			int height;
			SDL_QueryTexture(player_info[i]->anim_kind->SPRITESHEET, NULL, NULL, &width, &height);
			render_pos.w = (width / (player_info[i]->anim_kind->length + 1));
			render_pos.h = height;
			const double angle = 0;
			error_render = SDL_RenderCopyEx(pRenderer, player_info[i]->anim_kind->SPRITESHEET, &(player_info[i]->frame_rect), &render_pos, angle, NULL, flip);
			if (error_render != 0)
			{
				cout << "\n"
					 << SDL_GetError();
			}
		}
		// ^^^ END OF FLIP CODE ^^^

		check_attack_connections(player_info[0], player_info[1], pRenderer, visualize_boxes);

		camera = updateCamera(
			player_info[0]->pos.getRenderCoodrinateX(),
			player_info[0]->pos.getRenderCoodrinateY(),
			player_info[1]->pos.getRenderCoodrinateX(),
			player_info[1]->pos.getRenderCoodrinateY());

		SDL_SetRenderTarget(pRenderer, nullptr);
		SDL_RenderCopy(pRenderer, pScreenTexture, &camera, nullptr);
		SDL_RenderPresent(pRenderer);
	}

	SDL_DestroyRenderer(pRenderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}