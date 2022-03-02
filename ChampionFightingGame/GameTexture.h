#pragma once
#include <SDL.h>
#include "PlayerInfo.fwd.h"
#include "utils.h"
#include "stb_image.h"
#include <vector>
using namespace glm;

class Shader;

enum {
    GAME_TEXTURE_ORIENTATION_BOTTOM_LEFT,
    GAME_TEXTURE_ORIENTATION_BOTTOM_MIDDLE,
    GAME_TEXTURE_ORIENTATION_BOTTOM_RIGHT,
    GAME_TEXTURE_ORIENTATION_MIDDLE_LEFT,
    GAME_TEXTURE_ORIENTATION_MIDDLE,
    GAME_TEXTURE_ORIENTATION_MIDDLE_RIGHT,
    GAME_TEXTURE_ORIENTATION_TOP_LEFT,
    GAME_TEXTURE_ORIENTATION_TOP_MIDDLE,
    GAME_TEXTURE_ORIENTATION_TOP_RIGHT,

    GAME_TEXTURE_ORIENTATION_MAX,
};

enum {
    TEX_COORD_BOTTOM_LEFT,
    TEX_COORD_BOTTOM_RIGHT,
    TEX_COORD_TOP_RIGHT,
    TEX_COORD_TOP_LEFT,

    TEX_COORD_MAX,
};

struct GameTextureCoord {
    vec3 pos;
    vec2 tex_coord;
};

class GameTexture {
public:
    GameTexture();
    GameTexture(string path);
    GameTexture(const GameTexture& that);
    ~GameTexture();
    void init(string path);
    void destroy(bool destroy_texture = true);
    void set_pos(vec3 pos);
    void add_pos(vec3 pos);
    void set_rot(vec3 rot);
    void add_rot(vec3 rot);
    void set_orientation(int orientation);
    void attach_shader(Shader* shader);

    vec3 get_pos_vacuum(GameTexture *that);

    void scale_left_percent(float percent, bool crop = true); //broken for cropping, outdated for scaling, if you want to fix these then go ahead
    void scale_right_percent(float percent, bool crop = true); //but i can take care of it if not
    void scale_top_percent(float percent, bool crop = true);
    void scale_bottom_percent(float percent, bool crop = true);
    void scale_all_percent(float percent, bool crop = true);

    void set_width(int new_width);
    void set_width_scale(float scale);
    void set_height(int new_height);
    void set_height_scale(float scale);
    int get_width();
    int get_height();

    void set_left_target(float percent, float max_change);
    void set_right_target(float percent, float max_change);
    void set_top_target(float percent, float max_change);
    void set_bottom_target(float percent, float max_change);

    void set_target_pos(vec3 target, float frames);

    void set_alpha(unsigned char alpha);

    void flip_h(bool update = true);
    void flip_v(bool update = true);
    void reorient();

    void render();
    
    float target_left_crop = -2.0;
    float target_right_crop = -2.0;
    float target_top_crop = -2.0;
    float target_bottom_crop = -2.0;

    float target_left_max_change = 0.0;
    float target_right_max_change = 0.0;
    float target_top_max_change = 0.0;
    float target_bottom_max_change = 0.0;

    vec3 target_pos = vec3(0.0);
    vec3 target_pos_max_change = vec3(0.0);
    
    unsigned char alpha = 255;

    Shader *shader;
    u32 texture;
    GameTextureCoord tex_data[4];
    GameTextureCoord* tex_accessor[4];
    vec3 pos;
    vec3 rot;
    int orientation = GAME_TEXTURE_ORIENTATION_MIDDLE;

    //NEW

    vector<vec2> spritesheet[4];
    void load_spritesheet(); //load this however you want tbh
    void set_sprite(int section); //should crop the texture to the coords specified by the given section of the spritesheet, if it doesn't exist
    //make it yell at us etc.

    void crop_left(float edge); //p self explanatory, note that the private width and height vars contain the sprite's actually width but you'll
    void crop_right(float edge); //want to divide them by WINDOW_WIDTH and HEIGHT respectively before calling update_buffer_data
    void crop_top(float edge); //the other funcs that modify the texture data should be able to show a good example of this
    void crop_bottom(float edge);

private:
    void update_buffer_data();

    u32 VAO;
    u32 VBO;
    float width;
    float height;

    float width_scale_mul;
    float height_scale_mul;
    float width_orientation;
    float height_orientation;

    bool h_flipped;
    bool v_flipped;

    string name;
};