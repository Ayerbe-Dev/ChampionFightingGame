#pragma once
#include <iostream>
#include <SDL/SDL.h>
#include <glm/glm.hpp>

enum {
    GAME_TEXTURE_SDL_ORIENTATION_BOTTOM_LEFT,
    GAME_TEXTURE_SDL_ORIENTATION_BOTTOM_MIDDLE,
    GAME_TEXTURE_SDL_ORIENTATION_BOTTOM_RIGHT,
    GAME_TEXTURE_SDL_ORIENTATION_MIDDLE_LEFT,
    GAME_TEXTURE_SDL_ORIENTATION_MIDDLE,
    GAME_TEXTURE_SDL_ORIENTATION_MIDDLE_RIGHT,
    GAME_TEXTURE_SDL_ORIENTATION_TOP_LEFT,
    GAME_TEXTURE_SDL_ORIENTATION_TOP_MIDDLE,
    GAME_TEXTURE_SDL_ORIENTATION_TOP_RIGHT,

    GAME_TEXTURE_SDL_ORIENTATION_MAX,
};

class GameTextureSDL {
public:
    GameTextureSDL();
    GameTextureSDL(std::string path);

    void init(std::string path);

    void process();
    void render();

    void destroy();

    void flip_h();
    void flip_v();

    void set_pos(glm::vec2 pos);
    void add_pos(glm::vec2 pos);
    void set_orientation(int orientation);

    void scale_left_percent(float percent, bool crop = true);
    void scale_right_percent(float percent, bool crop = true);
    void scale_top_percent(float percent, bool crop = true);
    void scale_bottom_percent(float percent, bool crop = true);
    void scale_all_percent(float percent, bool crop = true);

    void set_left_target(float percent, float frames);
    void set_right_target(float percent, float frames);
    void set_top_target(float percent, float frames);
    void set_bottom_target(float percent, float frames);

    void set_target_pos(glm::vec2 target, float frames);

    glm::vec2 get_pos_offset(glm::vec2 offset);

    float left_percent = 1.0;
    float right_percent = 1.0;
    float top_percent = 1.0;
    float bottom_percent = 1.0;

    float left_crop = 1.0;
    float right_crop = 1.0;
    float top_crop = 1.0;
    float bottom_crop = 1.0;

    float target_left_crop = 0.0;
    float target_right_crop = 0.0;
    float target_top_crop = 0.0;
    float target_bottom_crop = 0.0;

    float prev_left_target[2] = {-1.0};
    float prev_right_target[2] = {-1.0};
    float prev_top_target[2] = {-1.0};
    float prev_bottom_target[2] = {-1.0};

    float target_left_frames = 0.0;
    float target_right_frames = 0.0;
    float target_top_frames = 0.0;
    float target_bottom_frames = 0.0;

    glm::vec2 target_pos = glm::vec2(0.0);
    glm::vec2 target_pos_frames = glm::vec2(0.0);
    glm::vec2 pos;
    int orientation = GAME_TEXTURE_SDL_ORIENTATION_MIDDLE;

    SDL_Rect dest_rect;
    SDL_Rect src_rect;

    SDL_Texture* texture;

    int base_width;
    int base_height;

    bool h_flipped;
    bool v_flipped;

    bool whole_texture;
    bool whole_screen;

private:
    void scale_left_percent_internal(SDL_Rect* rect, float percent);
    void scale_right_percent_internal(SDL_Rect* rect, float percent);
    void scale_top_percent_internal(SDL_Rect* rect, float percent);
    void scale_bottom_percent_internal(SDL_Rect* rect, float percent);
};