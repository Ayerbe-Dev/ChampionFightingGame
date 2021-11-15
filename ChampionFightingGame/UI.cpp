#include "UI.h"
extern SDL_Renderer* g_renderer;


HealthBar::HealthBar() {}
HealthBar::HealthBar(Fighter* fighter) {
	this->fighter = fighter;
	this->bar_texture = loadTexture("resource/ui/game/hp/bar.png");
	this->health_texture = loadTexture("resource/ui/game/hp/health.png");
	this->max_health = fighter->get_param_float("health");
	
	width = 894;
	height = 105;
	scale = 2;

	this->health_rect.x = 0;
	this->health_rect.y = 0;  

	this->health_rect.h = height;
	this->bar_rect.x = 3;
	this->bar_rect.y = 0;
	this->bar_rect.w = width;
	this->bar_rect.h = height;

	this->slice_rect.x = 0;
	this->slice_rect.y = 0;
	this->slice_rect.h = height;
}

void HealthBar::RenderAsP1() {
	slice_rect.w = width * (fighter->fighter_float[FIGHTER_FLOAT_HEALTH] / max_health);
	health_rect.w = slice_rect.w;
	health_rect.x = width - slice_rect.w + (bar_rect.x + 22.5);
	slice_rect.x = width - slice_rect.w;
	
	SDL_RenderCopy(g_renderer, health_texture, &slice_rect, &health_rect);
	SDL_RenderCopy(g_renderer,bar_texture,nullptr,&bar_rect);
}

void HealthBar::RenderAsP2() {
	bar_rect.x = WINDOW_WIDTH - width - 3;
	slice_rect.w = width * (fighter->fighter_float[FIGHTER_FLOAT_HEALTH] / max_health);
	health_rect.w = slice_rect.w;
	health_rect.x = (WINDOW_WIDTH - width) - 25.5;
	slice_rect.x = width - slice_rect.w;
	SDL_RenderCopyEx(g_renderer, health_texture, &slice_rect, &health_rect, 0, nullptr, SDL_FLIP_HORIZONTAL);
	SDL_RenderCopyEx(g_renderer,bar_texture,nullptr,&bar_rect,0,nullptr,SDL_FLIP_HORIZONTAL);
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
	this->texture = loadTexture(file_dir);
	this->indicator_rect.x = 0;
	this->indicator_rect.y = 0;
	this->indicator_rect.w = 92;
	this->indicator_rect.h = 92;
}
GameTimer::GameTimer(){};
GameTimer::GameTimer(int time){
    uiDecaseconds = 9;
    uiSeconds = 9;
	uiDecaframes = 6;
    uiFrames = 0;
	ClockMode = 1;
    pBigTypeface = loadTexture("resource/ui/game/timer/bigtypeface.png");
	pSmallTypeface = loadTexture("resource/ui/game/timer/smalltypeface.png");
	pClockFace= loadTexture("resource/ui/game/timer/clockface.png");
};

void GameTimer::Tick(){
	//printf("%d%d:%d%d\n\n",uiDecaseconds,uiSeconds,uiDecaframes,uiFrames);
	if (uiFrames == 0 && uiDecaframes == 0 && uiSeconds == 0 && uiDecaseconds == 0){
		//end
		uiDecaseconds = 9;
    	uiSeconds = 9;
		uiDecaframes = 5;
   		uiFrames = 9;
		//this just resets it for now. later it will have to returns something to indicate round over
	}
	if (uiFrames == 0 && uiDecaframes == 0){
		//reset frame counter
		uiDecaframes = 5;
   		uiFrames = 9;
		//count down seconds
		if (uiSeconds == 0){
			uiSeconds = 9;
			uiDecaseconds--;
		} else{
			uiSeconds--;
		}	
	} else{
		if (uiFrames == 0){
			uiFrames = 9;
			uiDecaframes--;
		} else{
			uiFrames--;
		}
	}
};

void GameTimer::Render(){
	SDL_Rect cClockFace{(WINDOW_WIDTH/2)-63,15,126,130};
	SDL_Rect cClockFaceSrc{84*ClockMode,0,84,87};
	SDL_RenderCopy(g_renderer,pClockFace,&cClockFaceSrc,&cClockFace);

	SDL_Rect cDecaDestRect{ 901,22,37,88 };
	SDL_Rect cDecaSourceRect{uiDecaseconds*25,0,25,59};
	SDL_RenderCopy(g_renderer,pBigTypeface,&cDecaSourceRect,&cDecaDestRect);

	SDL_Rect cDestRect{942,22,37,88};
	SDL_Rect cSourceRect{uiSeconds*25,0,25,59};
	SDL_RenderCopy(g_renderer,pBigTypeface,&cSourceRect,&cDestRect);

	SDL_Rect cDecaFrameDestRect{984,85,16,28};
	SDL_Rect cFrameSourceRect{uiDecaframes*11,0,11,19};
	SDL_RenderCopy(g_renderer,pSmallTypeface,&cFrameSourceRect,&cDecaFrameDestRect);

	SDL_Rect cFrameDestRect{1003,85,16,28};
	SDL_Rect cFrameSourceRectOof{uiFrames*11,0,11,19};
	SDL_RenderCopy(g_renderer,pSmallTypeface,&cFrameSourceRectOof,&cFrameDestRect);
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