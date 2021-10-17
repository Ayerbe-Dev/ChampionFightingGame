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

SDL_Texture *pBG;

int main()
{
	bool running = true;
	bool visualize_boxes = true;
	bool debug = false;
	bool enable_player_input = true;
	SDL_Rect debug_rect[2] = {0, 0, 0, 0};
	GameCoordinate debug_anchor[2];
	GameCoordinate debug_offset[2];
	Debugger debugger[2];
	debugger[0] = Debugger(0);
	debugger[1] = Debugger(1);

	//init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("error initializing SDL: %s\n", SDL_GetError());
	}

	SDL_Window *window = SDL_CreateWindow("Champions of the Ring", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//Background image
	pBG = loadTexture("resource/stage/training_room/background.png", renderer);
	SDL_Texture *screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);

	//init players
	PlayerInfo *player_info[2];

	IFighter *p1 = new IFighter(CHARA_KIND_ROY, renderer, 0);
	IFighter *p2 = new IFighter(CHARA_KIND_ERIC, renderer, 1);

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
				bool old_button = debugger[o].button_info[i].button_on;
				debugger[o].button_info[i].button_on = keyboard_state[debugger[o].button_info[i].mapping];
				bool new_button = debugger[o].button_info[i].button_on;
				debugger[o].button_info[i].changed = (old_button != new_button);
			}
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

		SDL_RenderClear(renderer);

		SDL_SetRenderTarget(renderer, screen);
		SDL_RenderCopy(renderer, pBG, nullptr, nullptr); //copies the background
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

			if (debugger[i].check_button_trigger(BUTTON_DEBUG))
			{
				debug = !debug;
			}
			if (!debug)
			{
				tickOnce(player_info[i], renderer);
			}
			else
			{
				if (debugger[i].check_button_trigger(BUTTON_DEBUG_2))
				{
					for (int o = 0; o < BUTTON_MAX; o++)
					{
						bool old_button = player_info[i]->button_info[o].button_on;
						player_info[i]->button_info[o].button_on = keyboard_state[player_info[i]->button_info[o].mapping];
						bool new_button = player_info[i]->button_info[o].button_on;
						player_info[i]->button_info[o].changed = (old_button != new_button);
					}
					tickOnce(player_info[i], renderer);
				}
				if (debugger[i].check_button_trigger(BUTTON_LP))
				{
					debug_anchor[i].x = (((player_info[i]->pos.x * player_info[i]->facing_dir)) * player_info[i]->facing_dir) + WINDOW_WIDTH / 2;
					debug_anchor[i].y = WINDOW_HEIGHT - player_info[i]->pos.y;
					debug_offset[i].x = (((player_info[i]->pos.x * player_info[i]->facing_dir)) * player_info[i]->facing_dir) + WINDOW_WIDTH / 2;
					debug_offset[i].y = WINDOW_HEIGHT - player_info[i]->pos.y;
				}
				if (debugger[i].check_button_on(BUTTON_MP))
				{
					if (debugger[i].check_button_on(BUTTON_RIGHT))
					{
						debug_anchor[i].x += 1;
					}
					if (debugger[i].check_button_on(BUTTON_LEFT))
					{
						debug_anchor[i].x -= 1;
					}
					if (debugger[i].check_button_on(BUTTON_UP))
					{
						debug_anchor[i].y -= 1;
					}
					if (debugger[i].check_button_on(BUTTON_DOWN))
					{
						debug_anchor[i].y += 1;
					}
				}
				if (debugger[i].check_button_on(BUTTON_HP))
				{
					if (debugger[i].check_button_on(BUTTON_RIGHT))
					{
						debug_offset[i].x += 1;
					}
					if (debugger[i].check_button_on(BUTTON_LEFT))
					{
						debug_offset[i].x -= 1;
					}
					if (debugger[i].check_button_on(BUTTON_UP))
					{
						debug_offset[i].y -= 1;
					}
					if (debugger[i].check_button_on(BUTTON_DOWN))
					{
						debug_offset[i].y += 1;
					}
				}
				debug_rect[i].x = debug_anchor[i].x;
				debug_rect[i].y = debug_anchor[i].y;
				debug_rect[i].w = debug_offset[i].x;
				debug_rect[i].h = debug_offset[i].y;
				debug_rect[i].w -= debug_rect[i].x;
				debug_rect[i].h -= debug_rect[i].y;
				if (debugger[i].check_button_on(BUTTON_START))
				{
					SDL_Rect temp_rect;
					temp_rect.x = ((debug_anchor[i].x - (player_info[i]->pos.x + WINDOW_WIDTH / 2 * player_info[i]->facing_dir)));
					temp_rect.y = (debug_anchor[i].y - WINDOW_HEIGHT) * -1.0 - player_info[i]->pos.y;
					temp_rect.w = ((debug_offset[i].x - (player_info[i]->pos.x + WINDOW_WIDTH / 2 * player_info[i]->facing_dir)));
					temp_rect.h = (debug_offset[i].y - WINDOW_HEIGHT) * -1.0 - player_info[i]->pos.y;
					cout << "x0: " << temp_rect.x << endl;
					cout << "y0: " << temp_rect.y << endl;
					cout << "x1: " << temp_rect.w << endl;
					cout << "y1: " << temp_rect.h << endl;
				}
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderDrawRect(renderer, debug_rect);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 127);
				SDL_RenderFillRect(renderer, debug_rect);
			}

			SDL_Rect render_pos;
			render_pos.x = player_info[i]->pos.getRenderCoodrinateX();
			render_pos.y = player_info[i]->pos.getRenderCoodrinateY();
			render_pos.w = player_info[i]->anim_kind->sprite_width;
			render_pos.h = player_info[i]->anim_kind->sprite_height;
			const double angle = 0;
			error_render = SDL_RenderCopyEx(renderer, player_info[i]->anim_kind->SPRITESHEET, &(player_info[i]->frame_rect), &render_pos, angle, NULL, flip);
			if (error_render != 0)
			{
				cout << "\n"
					 << SDL_GetError();
			}
		}
		check_attack_connections(player_info[0], player_info[1], renderer, visualize_boxes);

		SDL_Rect camera;

		camera.w = player_info[1]->pos.getRenderCoodrinateX() + 100 - player_info[0]->pos.getRenderCoodrinateX();
		camera.h = camera.w * 0.5625;
		camera.x = player_info[0]->pos.getRenderCoodrinateX();
		camera.y = WINDOW_HEIGHT - camera.h;

		//SDL_RenderSetClipRect(renderer, &viewport);
		//SDL_RenderSetScale(renderer, 2, 1);
		SDL_SetRenderTarget(renderer, nullptr);
		SDL_RenderCopy(renderer, screen, &camera, nullptr);
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}