using namespace std;
#include <iostream>
#include <functional>
#include <vector>

/*
For each of the user's controls, track what key they're assigned to, whether or not that button is being pressed, and whether or not a change was made
on the current frame
*/

class Animation {
public:

	vector<SDL_Texture> frame_vector;

	Animation() {}
	Animation(string animation_location, int frame_count) {
		for (int i = 0; i < frame_count; i++) {
			frame_vector.push_back();
		}
	}
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

class ButtonMap {
public:
	SDL_Scancode mappings[BUTTON_MAX];

	ButtonMap() {
		mappings[BUTTON_UP] = SDL_SCANCODE_W;
		mappings[BUTTON_LEFT] = SDL_SCANCODE_A;
		mappings[BUTTON_DOWN] = SDL_SCANCODE_S;
		mappings[BUTTON_RIGHT] = SDL_SCANCODE_D;
			
		mappings[BUTTON_START] = SDL_SCANCODE_SPACE;
	}

	void loadDefaultButtonMap(int player) {
		if (player == 0) {
			mappings[BUTTON_UP] = SDL_SCANCODE_W;
			mappings[BUTTON_LEFT] = SDL_SCANCODE_A;
			mappings[BUTTON_DOWN] = SDL_SCANCODE_S;
			mappings[BUTTON_RIGHT] = SDL_SCANCODE_D;

			mappings[BUTTON_START] = SDL_SCANCODE_SPACE;
		}
		else if (player == 1) {
			mappings[BUTTON_UP] = SDL_SCANCODE_UP;
			mappings[BUTTON_DOWN] = SDL_SCANCODE_DOWN;
			mappings[BUTTON_LEFT] = SDL_SCANCODE_LEFT;
			mappings[BUTTON_RIGHT] = SDL_SCANCODE_RIGHT;

			mappings[BUTTON_START] = SDL_SCANCODE_RETURN;
		} 
	}
};

//Store all relevant information about each character. Treat this like a L2CFighterCommon or Boma.
class PlayerInfo {
public:
	i64 id{ -1 };
	string chara_kind{ "default" };
	GameCoordinate pos;
	GameCoordinate prevpos;
	//f32 pos_x{ 0.0 };
	//f32 pos_y{ 0.0 };
	//f32 prev_pos_x{ 0.0 };
	//f32 prev_pos_y{ 0.0 };
	f32 height{ 0.0 };
	f32 width{ 0.0 };
	u32 status_kind{ 0 };
	ButtonMap loaded_button_map;
	string resource_dir;
	SDL_Texture* default_texture;

	PlayerInfo() { }

	PlayerInfo(string character_type, SDL_Renderer* renderer){
		// runs on creation of instance;	

		//default texture loading
		resource_dir = "resource/chara/" + character_type;
		string texture_path = resource_dir + "/sprite/sprite.png"; 
		default_texture = loadTexture(texture_path.c_str(), renderer);// some shit about const chars is really making this painful
		
		//other numbers
		height = 100;
		width = 100;


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
		loaded_button_map.loadDefaultButtonMap(0);

	}

	void setStateLikePlayer2() {
		id = 1;
		pos = GameCoordinate(WINDOW_WIDTH, WINDOW_HEIGHT, 200, 0); // Idk if this causes a leak
		loaded_button_map.loadDefaultButtonMap(1);
	}

	void processInput(const Uint8* keyboard_state) {
		if (keyboard_state[loaded_button_map.mappings[BUTTON_UP]]) {
			pos.y += 1.0;
		}
		if (keyboard_state[loaded_button_map.mappings[BUTTON_DOWN]]) {
			pos.y -= 1.0;
		}
		if (keyboard_state[loaded_button_map.mappings[BUTTON_LEFT]]) {
			pos.x -= 1.0;
		}
		if (keyboard_state[loaded_button_map.mappings[BUTTON_RIGHT]]) {
			pos.x += 1.0;
		}
		if (keyboard_state[loaded_button_map.mappings[BUTTON_START]]) {
			if (id == 0){
				pos.x = -200.0;
				pos.y = 0.0;
			}
			else {
				pos.x = 200.0;
				pos.y = 0.0;
			}
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

bool check_button_on(PlayerInfo* player_info, u32 button);
bool check_button_trigger(PlayerInfo* player_info, u32 button);
bool check_button_release(PlayerInfo* player_info, u32 button);

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


void game_main(PlayerInfo* player_info, SDL_Renderer* renderer, const Uint8* keyboard_state) {
	//Handle statuses

	set_status_functions(player_info);
	(*player_info).status_kind = CHARA_STATUS_WAIT;
	(*player_info).wait(player_info);

	/*
		Get the player's inputs. This will also probably be where statuses are changed later on
	*/
	
	player_info->processInput(keyboard_state);
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
