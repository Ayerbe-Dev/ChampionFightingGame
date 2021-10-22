#include "UI.h"

HealthBar::HealthBar() {}
HealthBar::HealthBar(SDL_Renderer* renderer, FighterInstance* fighter_instance) {
	this->pRenderer = renderer;
	this->fighter_instance = fighter_instance;
	this->bar_texture = loadTexture("resource/ui/game/hp/bar.png", renderer);
	this->health_texture = loadTexture("resource/ui/game/hp/health.png", renderer);
	this->max_health = fighter_instance->get_param_float("health");
	
	width = 294;
	height = 31;
	scale = 2;

	this->health_rect.x = 5;
	this->health_rect.y = 10;
	//this->health_rect.w = 294*2; should be defined right before render
	this->health_rect.h = 31*2;
	this->bar_rect.x = 5;
	this->bar_rect.y = 10;
	this->bar_rect.w = 294*2;
	this->bar_rect.h = 31*2;

	this->slice_rect.x = 0;
	this->slice_rect.y = 0;
	this->slice_rect.w = width - 150;
	this->slice_rect.h = height;
}

void HealthBar::RenderAsP1() {
	health_rect.w = slice_rect.w * scale;
	SDL_RenderCopy(this->pRenderer,health_texture,&slice_rect,&health_rect);
	SDL_RenderCopy(this->pRenderer,bar_texture,nullptr,&bar_rect);
	// health_bar[i].health_rect.w = health_bar[i].bar_rect.w * (fighter_instance[i]->chara_float[CHARA_FLOAT_HEALTH] / health_bar[i].max_health);
	// SDL_RenderCopyEx(pRenderer, health_bar[i].health_texture, nullptr, &(health_bar[i].bar_rect), angle, NULL, flip);
	// SDL_RenderCopyEx(pRenderer, health_bar[i].bar_texture, nullptr, &(health_bar[i].bar_rect), angle, NULL, flip);
}

void HealthBar::RenderAsP2() {};

PlayerIndicator::PlayerIndicator() {}
PlayerIndicator::PlayerIndicator(SDL_Renderer* renderer, FighterInstance* fighter_instance, string nametag) {
	this->fighter_instance = fighter_instance;
	this->nametag = nametag;
	string resource_dir = "resource/ui/game/tag/";
	if (fighter_instance->id == 0) {
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
	this->texture = loadTexture(file_dir, renderer);
	this->indicator_rect.x = 0;
	this->indicator_rect.y = 0;
	this->indicator_rect.w = 92;
	this->indicator_rect.h = 92;
}

Timer::Timer() {}
Timer::Timer(SDL_Renderer* renderer, int remaining) {
	this->remaining = remaining;
	string resource_dir = "resource/ui/game/timer/";
	if (remaining == -1) {
		resource_dir += "infinite.png";
	}
	else {
		resource_dir += "infinite.png"; //todo: Make a blank version of the timer with no infinite symbol
	}
	const char* file_dir = resource_dir.c_str();
	this->texture = loadTexture(file_dir, renderer);
	this->timer_rect.x = WINDOW_WIDTH / 2 - (92 / 2);
	this->timer_rect.y = 0;
	this->timer_rect.w = 92;
	this->timer_rect.h = 92;
};

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