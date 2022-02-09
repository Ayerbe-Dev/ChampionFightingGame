#include "UI.h"
extern SDL_Renderer* g_renderer;


HealthBar::HealthBar() {}
HealthBar::HealthBar(Fighter* fighter) {
	this->fighter = fighter;
	max_health = fighter->get_param_float("health");
	health_texture.init("resource/ui/game/hp/health.png");
	bar_texture.init("resource/ui/game/hp/bar.png");

	if (fighter->id == 0) {
		health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
		bar_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_LEFT);
	}
	else {
		health_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		bar_texture.set_orientation(GAME_TEXTURE_ORIENTATION_TOP_RIGHT);
		health_texture.flip_h();
		bar_texture.flip_h();
	}

}

ExBar::ExBar() {}
ExBar::ExBar(Fighter* fighter) {
	this->fighter = fighter;
	max_ex = EX_METER_SIZE;
	ex_texture.init("resource/ui/game/ex/ex.png");
	ex_texture.scale_right_percent(0);

	ex_segment_texture.init("resource/ui/game/ex/ex_segment.png");
	ex_segment_texture.scale_right_percent(0);

	bar_texture.init("resource/ui/game/ex/bar.png");

	if (fighter->id == 0) {
		ex_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		ex_segment_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
		bar_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT);
	}
	else {
		ex_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		ex_segment_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		bar_texture.set_orientation(GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT);
		ex_texture.flip_h();
		ex_segment_texture.flip_h();
		bar_texture.flip_h();
	}
}

PlayerIndicator::PlayerIndicator() {}
PlayerIndicator::PlayerIndicator(Fighter* fighter, string nametag) {
	this->fighter = fighter;
	this->nametag = nametag;
	string resource_dir = "resource/ui/game/tag/";
	if (fighter->id == 0) {
		resource_dir += "p1_tag";
	}
	else {
		resource_dir += "p2_tag";
	}
	if (nametag == "") {
		resource_dir += "_no_nametag";
	}
	resource_dir += ".png";
	const char* file_dir = resource_dir.c_str();
	this->texture = loadSDLTexture(file_dir);
	this->indicator_rect.x = 0;
	this->indicator_rect.y = 0;
	this->indicator_rect.w = 92;
	this->indicator_rect.h = 92;
}
GameTimer::GameTimer() {};
GameTimer::GameTimer(int time) {
	uiDecaseconds = 9;
	uiSeconds = 9;
	uiDecaframes = 6;
	uiFrames = 0;
	ClockMode = 1;
	pBigTypeface = loadSDLTexture("resource/ui/game/timer/bigtypeface.png");
	pSmallTypeface = loadSDLTexture("resource/ui/game/timer/smalltypeface.png");
	pClockFace = loadSDLTexture("resource/ui/game/timer/clockface.png");
};

void GameTimer::Tick() {
	//printf("%d%d:%d%d\n\n",uiDecaseconds,uiSeconds,uiDecaframes,uiFrames);
	if (uiFrames == 0 && uiDecaframes == 0 && uiSeconds == 0 && uiDecaseconds == 0) {
		//end
		uiDecaseconds = 9;
		uiSeconds = 9;
		uiDecaframes = 5;
		uiFrames = 9;
		//this just resets it for now. later it will have to returns something to indicate round over
	}
	if (uiFrames == 0 && uiDecaframes == 0) {
		//reset frame counter
		uiDecaframes = 5;
		uiFrames = 9;
		//count down seconds
		if (uiSeconds == 0) {
			uiSeconds = 9;
			uiDecaseconds--;
		}
		else {
			uiSeconds--;
		}
	}
	else {
		if (uiFrames == 0) {
			uiFrames = 9;
			uiDecaframes--;
		}
		else {
			uiFrames--;
		}
	}
};

void GameTimer::Render() {
	SDL_Rect cClockFace{ (WINDOW_WIDTH / 2) - 63,15,126,130 };
	SDL_Rect cClockFaceSrc{ 84 * ClockMode,0,84,87 };
	SDL_RenderCopy(g_renderer, pClockFace, &cClockFaceSrc, &cClockFace);

	SDL_Rect cDecaDestRect{ 901,22,37,88 };
	SDL_Rect cDecaSourceRect{ uiDecaseconds * 25,0,25,59 };
	SDL_RenderCopy(g_renderer, pBigTypeface, &cDecaSourceRect, &cDecaDestRect);

	SDL_Rect cDestRect{ 942,22,37,88 };
	SDL_Rect cSourceRect{ uiSeconds * 25,0,25,59 };
	SDL_RenderCopy(g_renderer, pBigTypeface, &cSourceRect, &cDestRect);

	SDL_Rect cDecaFrameDestRect{ 984,85,16,28 };
	SDL_Rect cFrameSourceRect{ uiDecaframes * 11,0,11,19 };
	SDL_RenderCopy(g_renderer, pSmallTypeface, &cFrameSourceRect, &cDecaFrameDestRect);

	SDL_Rect cFrameDestRect{ 1003,85,16,28 };
	SDL_Rect cFrameSourceRectOof{ uiFrames * 11,0,11,19 };
	SDL_RenderCopy(g_renderer, pSmallTypeface, &cFrameSourceRectOof, &cFrameDestRect);
}

/*





																  .
																 }ob
																 00O
																~ZZl
																t00^
															   .00Q
															   {OZ
															   zZC
															  `OZn
															  jQO`
															  J0U                ___________________
															 :0wu               (  So, No Head?     )
															 J0m"               (___________________)
															"mwz               /
															+0Q?              /
														   ^pO0^     ^;      /
														   {hhh.   Zkhaak+
														   bakX   nUtJZdhm:
													 :    |QUY)!{l^``"i|ruUX'
													   .tfZwZbpbbwYx/rXUppzX)^"II
													  .OkhbmUumOwpOpqqdkbhabbkopwmhh)
													 ^vkkkpm|pqOQOdqkhkk*#%%%%#ahbXYo_
													'vphbdbpJhbbbbbbbbh#mnLmZOJfqQLXpx
												  .IxkqbdbwX)ZhhkkkkkkhaqYmmmmzxzcuuw[
											   r^?/ffYvqkkkkkkpudbkkkhaoW*88mrr/tttvZ
											  `)ffffjn)dkkkkkkkJtt/1<";](]0):!<~[nJX?
												.,[!^ `kkkkkkkhtttttttt///|(rmmwwwXjl
													   `Zwpddd0ffftttttttttfjJwqqqqw
														  l#*%WWWW*q0vffffffrqpqpaW>
														   *BBB&&&&&WW&&WWW8%8%8WW&j"
														  '##CXXLCYYYYQLYYYLd#hpwb#*
														   'wmmpkkwmmpkkwmmZ%WM##*].
														  0ahmmqppwmmqppwmZq&#h#ob.
														 XaoWmmmmmmmmmmmmmOp#Owdoac
														-hoC:mmmmmmmmmmmwwZaMMwC|`
															1qM##########MMM*pmQn
														   [wb*MMMMMMMMMMMMkkdqZU)
														  1wbkaaMMMMMMMWWpahkbdq0n
														 _qbhaaabLrxuUn  thkkbddmY)
														;mbbkkkk1         |bbdddpOu`
													   'mbbbkkk-           Odbbbbwzl
													  'Okkkkhh>            +kkkkkbOf
													  (hhhhhhI              khhhhhbCl
													 !Chhhhhku.             lhhhhhkp(.
													 {bkkkhhhb_              }hhhhhhZ/
													 lhhhkhhhh:              Chhhhhhhp(
													  )Xmhha*^              .vdhhhhhhkdt
														 J###d,             `Qbahhhhhhd!
														 Ohhhhx               zahhhhk?
														<phhhoY                }#M##~
													   .whhh##h                 o##*
												   :/Qb*#####aa+               [Jooh,
												  _p###*qQ+ '                  tkhhh~
													'`'                        tkhhh+
																			   Cbhhhf
																			  ?wkoooU
																			  lqqakqC)
																				]wo*#*hX
																				:UYOppm0^

















*/