using namespace std;
#include <iostream>
#include <functional>

/*
For each of the user's controls, track what key they're assigned to, whether or not that button is being pressed, and whether or not a change was made
on the current frame
*/
struct ButtonState { 
	u32 mapping;
	bool button_on;
	bool changed;
};

//Smash-style name tag system where custom controls are mapped to a tag
struct NameTag {
	ButtonState buttons[BUTTON_MAX];
};

//Store all relevant information about each character. Treat this like a L2CFighterCommon or Boma.
class PlayerInfo {
	public:
		u64 id;
		string chara_kind;
		f32 pos_x;
		f32 pos_y;
		f32 prev_pos_x;
		f32 prev_pos_y;
		u32 status_kind;
		ButtonState buttons[BUTTON_MAX];
		string resource_dir;
		SDL_Texture* texture_instance;

		void wait() {};
		void walkf() {};
		void walkb() {};
		void dash() {};
		void dashb() {};
		void crouch() {};
		void crouchs() {};
		void jumpsquat() {};
		void attack() {};
		void hitstun() {};
		void blockstun() {};
};

void process_inputs(PlayerInfo player_info);
internal void set_status_functions(PlayerInfo player_info);
bool check_button_on(PlayerInfo player_info, u32 button);
bool check_button_trigger(PlayerInfo player_info, u32 button);
bool check_button_release(PlayerInfo player_info, u32 button);
void status_wait(PlayerInfo player_info);

internal void game_main(PlayerInfo player_info, SDL_Renderer* renderer) {
	/*
		Find the sprite for the current character, map it to a surface, give that surface a texture, then free the surface. The texture instances are all 
		handled together outside of this function.
	*/
	player_info.resource_dir = ("resource/chara/" + player_info.chara_kind + "/");
	string sprite_dir = (player_info.resource_dir + "sprite/sprite.png");
	const char* sprite = sprite_dir.c_str(); //Trying to turn sprite_dir directly into a const *char turned the text into a bunch of spaghetti
	SDL_Surface* surface = IMG_Load(sprite);
	player_info.texture_instance = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	set_status_functions(player_info);
	player_info.status_kind = CHARA_STATUS_WAIT;
	cout << "About to call wait" << endl;
	player_info.wait();
	status_wait(player_info);
	cout << "Called wait" << endl;

	/*
		Once that's all finished, send the player_info over to an input processor which will be used to set up status changes in the future
	*/
	process_inputs(player_info);
}

internal void set_status_functions(PlayerInfo player_info) {
//	player_info.wait() = status_wait;
}

internal void process_inputs(PlayerInfo player_info) {

	//Our position on the last frame is set to prev_pos

	player_info.prev_pos_x = player_info.pos_x;
	player_info.prev_pos_y = player_info.pos_y;

	if (check_button_on(player_info, BUTTON_START)) {
		player_info.pos_x = 0.0;
		player_info.pos_y = 0.0;
	}
	if (check_button_on(player_info, BUTTON_UP)) {
		player_info.pos_y += 1.0;
	}
	if (check_button_on(player_info, BUTTON_DOWN)) {
		player_info.pos_y -= 1.0;
	}
	if (check_button_on(player_info, BUTTON_LEFT)) {
		player_info.pos_x -= 1.0;
	}
	if (check_button_on(player_info, BUTTON_RIGHT)) {
		player_info.pos_x += 1.0;
	}

	/*
		Once I start adding collision, I'm going to add a check here that basically says "if part of your position is otherwise invalid, change that part
		back to whatever it was on the previous frame. Since this function runs before things are rendered, no out of bounds movement will actually be seen
	*/

	//We're done actively checking for any changed inputs, so now we can safely reset them in preparation for the next frame

	for (int i = 0; i < BUTTON_MAX; i++) {
		player_info.buttons[i].changed = false;
	}

	return;
}

bool check_button_on(PlayerInfo player_info, u32 button) {
	return player_info.buttons[button].button_on;
}

bool check_button_trigger(PlayerInfo player_info, u32 button) {
	return player_info.buttons[button].changed && player_info.buttons[button].button_on;
}

bool check_button_release(PlayerInfo player_info, u32 button) {
	return player_info.buttons[button].changed && !player_info.buttons[button].button_on;
}

void status_wait(PlayerInfo player_info) {
	cout << "Player " << player_info.id << " X: " << player_info.pos_x << endl;
	cout << "Player " << player_info.id << " X: " << player_info.pos_x << endl;
}