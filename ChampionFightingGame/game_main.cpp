using namespace std;
#include <iostream>
#include <functional>
#include <vector>
#include<string>

/*
For each of the user's controls, track what key they're assigned to, whether or not that button is being pressed, and whether or not a change was made
on the current frame
*/



struct Buttons {
	u32 mapping = 0;
	bool button_on = false;
	bool changed = false;
};

class GameCoordinate {
	public:
		f32 x;
		f32 y;
		f32 x_offset;
		f32 y_offset;

		GameCoordinate() {
			x = 0.0;
			y = 0.0;
			x_offset = 0.0;
			y_offset = 0.0;
		}

		GameCoordinate(f32 window_width, f32 window_height) {
			x = 0.0;
			y = 0.0;
			x_offset = window_width/2;
			y_offset = window_height/2;
		}

		GameCoordinate(f32 window_width, f32 window_height, f32 start_x, f32 start_y) {
			x = start_x;
			y = start_y;
			x_offset = window_width / 2;
			y_offset = window_height / 2;
		}

		f32 getRenderCoodrinateX() {
			return x + x_offset;
		}

		f32 getRenderCoodrinateY() {
			return y + y_offset;
		}
};

//Store all relevant information about each character. Treat this like a L2CFighterCommon or Boma.
class PlayerInfo {
public:
	i64 id{ -1 };
	string chara_kind{ "default" };
	GameCoordinate pos;
	GameCoordinate prevpos;
	f32 height { 0.0 };
	f32 width { 0.0 };
	u32 status_kind { 0 };
	Buttons button_info[BUTTON_MAX];
	string resource_dir;
	SDL_Texture* current_texture;
	int frame;
	int eframe;

	PlayerInfo() { }

	PlayerInfo(string chara_kind, SDL_Renderer* renderer){
		// runs on creation of instance;	

		//default texture loading
		resource_dir = "resource/chara/" + chara_kind;
		string texture_path = resource_dir + "/sprite/sprite.png"; 
		current_texture = loadTexture(texture_path.c_str(), renderer);// some shit about const chars is really making this painful
		
		//other numbers
		height = 100;
		width = 100;

		

		//load animation
		string loc;
		SDL_Texture* tmp_texture;
		for (int i = 0; i < 10; i++) {
			loc = "resource/chara/not_ryu/animation_idle/idle_" + to_string(i) + ".png";
			tmp_texture = loadTexture(loc.c_str(), renderer);
			IDLE_ANIMATION[i] = tmp_texture;
		}
		frame = 0;
		eframe = 9;// sizeof(IDLE_ANIMATION);
	}

	void idle_aimation_test() {
		//this is not a good way to handle this, im just testing
		frame++;
		current_texture = IDLE_ANIMATION[frame];
		if (frame == eframe) {
			frame = 0;
		}


		}
		
		
	

	void loadDefaultButtonMap() {
		if (id == 0) {
			button_info[BUTTON_UP].mapping = SDL_SCANCODE_W;
			button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_A;
			button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_S;
			button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_D;

			button_info[BUTTON_START].mapping = SDL_SCANCODE_SPACE;
		}
		else if (id == 1) {
			button_info[BUTTON_UP].mapping = SDL_SCANCODE_UP;
			button_info[BUTTON_DOWN].mapping = SDL_SCANCODE_DOWN;
			button_info[BUTTON_LEFT].mapping = SDL_SCANCODE_LEFT;
			button_info[BUTTON_RIGHT].mapping = SDL_SCANCODE_RIGHT;

			button_info[BUTTON_START].mapping = SDL_SCANCODE_RETURN;
		}
	}

	SDL_Texture* loadTexture(const char* file_path, SDL_Renderer* renderer){
		SDL_Surface* image_surface = IMG_Load(file_path);
		if (image_surface == NULL) {
			std::cout << "Error loading image: " << IMG_GetError() << endl;
		}
		return SDL_CreateTextureFromSurface(renderer, image_surface);
		SDL_FreeSurface(image_surface); // haha no more memory leaks
	}

	void setStateLikePlayer1(){
		id = 0;
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, -200, 0); // Idk if this causes a leak

	}

	void setStateLikePlayer2() {
		id = 1;
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0); // Idk if this causes a leak
	}


	bool check_button_on(u32 button) {
		return button_info[button].button_on;
	}

	bool check_button_trigger(u32 button) {
		return button_info[button].changed && button_info[button].button_on;
	}

	bool check_button_release(u32 button) {
		return button_info[button].changed && !button_info[button].button_on;
	}

	void processInput() {
		if (check_button_on(BUTTON_START)) {
			pos.y = 0.0;
			if (id == 0) {
				pos.x = -200.0;
			}
			else {
				pos.x = 200.0;
			}
		}
		if (check_button_on(BUTTON_UP)) {
			pos.y -= 1.0;
		}
		if (check_button_on(BUTTON_DOWN)) {
			pos.y += 1.0;
		}
		if (check_button_on(BUTTON_LEFT)) {
			pos.x -= 1.0;
		}
		if (check_button_on(BUTTON_RIGHT)) {
			pos.x += 1.0;
		}
	}

	function<void(PlayerInfo*)> wait;
	function<void(PlayerInfo*)> walkf;
	function<void(PlayerInfo*)> walkb;
	function<void(PlayerInfo*)> dash;
	function<void(PlayerInfo*)> dashb;
	function<void(PlayerInfo*)> crouch;
	function<void(PlayerInfo*)> crouchs;
	function<void(PlayerInfo*)> jumpsquat;
	function<void(PlayerInfo*)> jump;
	function<void(PlayerInfo*)> attack;
	function<void(PlayerInfo*)> hitstun;
	function<void(PlayerInfo*)> blockstun;
};

void process_inputs(PlayerInfo* player_info);
void status_wait(PlayerInfo* player_info);
void status_walkf(PlayerInfo* player_info);
void status_walkb(PlayerInfo* player_info);
void status_dash(PlayerInfo* player_info);
void status_dashb(PlayerInfo* player_info);
void status_crouch(PlayerInfo* player_info);
void status_crouchs(PlayerInfo* player_info);
void status_jumpsquat(PlayerInfo* player_info);
void status_jump(PlayerInfo* player_info);
void status_attack(PlayerInfo* player_info);
void status_hitstun(PlayerInfo* player_info);
void status_blockstun(PlayerInfo* player_info);

void set_status_functions(PlayerInfo* player_info);

void set_status_functions(PlayerInfo* player_info) {
	(*player_info).wait = &status_wait;
	(*player_info).walkf = &status_walkf;
	(*player_info).walkb = &status_walkb;
	(*player_info).dash = &status_dash;
	(*player_info).dashb = &status_dashb;
	(*player_info).crouch = &status_crouch;
	(*player_info).crouchs = &status_crouchs;
	(*player_info).jumpsquat = &status_jumpsquat;
	(*player_info).jump = &status_jump;
	(*player_info).attack = &status_attack;
	(*player_info).hitstun = &status_hitstun;
	(*player_info).blockstun = &status_blockstun;
}


void game_main(PlayerInfo* player_info, SDL_Renderer* renderer) {
	//Handle statuses

	set_status_functions(player_info);
	(*player_info).status_kind = CHARA_STATUS_WAIT;
	(*player_info).wait(player_info);

	/*
		Get the player's inputs. This will also probably be where statuses are changed later on
	*/
	player_info->idle_aimation_test();
	player_info->processInput();
}


void status_wait(PlayerInfo* player_info) {

}

void status_walkf(PlayerInfo* player_info) {

}

void status_walkb(PlayerInfo* player_info) {

}

void status_dash(PlayerInfo* player_info) {

}

void status_dashb(PlayerInfo* player_info) {

}

void status_crouch(PlayerInfo* player_info) {

}

void status_crouchs(PlayerInfo* player_info) {

}

void status_jumpsquat(PlayerInfo* player_info) {

}

void status_jump(PlayerInfo* player_info) {

}

void status_attack(PlayerInfo* player_info) {

}

void status_hitstun(PlayerInfo* player_info) {

}

void status_blockstun(PlayerInfo* player_info) {

}
