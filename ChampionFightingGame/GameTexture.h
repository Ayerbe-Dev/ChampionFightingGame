#pragma once
#include <SDL.h>
#include "PlayerInfo.fwd.h"
#include "utils.h"
#include "stb_image.h"
#include "Shader.h"

enum {
    GAME_TEXTURE_ANCHOR_MODE_DEFAULT,
    GAME_TEXTURE_ANCHOR_MODE_CENTER,
    GAME_TEXTURE_ANCHOR_MODE_BACKGROUND,
    GAME_TEXTURE_ANCHOR_MODE_METER,
};

enum {
    TEXTURE_FLIP_KIND_NONE,
    TEXTURE_FLIP_KIND_DRAIN,
    TEXTURE_FLIP_KIND_NO_DRAIN,
    
    TEXTURE_FLIP_KIND_MAX,
};

enum {
    METER_DRAIN_KIND_RIGHT,
    METER_DRAIN_KIND_LEFT,
    METER_DRAIN_KIND_NONE,
};

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

class GameTextureNew {
public:
    GameTextureNew(string path);
    void set_pos(vec3 pos);
    void add_pos(vec3 pos);
    void set_rot(vec3 rot);
    void add_rot(vec3 rot);
    void set_orientation(int orientation);
    void attach_shader(Shader* shader);

    void crop_left_percent(float percent);
    void crop_right_percent(float percent);
    void crop_top_percent(float percent);
    void crop_bottom_percent(float percent);
    void scale_left_percent(float percent);
    void scale_right_percent(float percent);
    void scale_top_percent(float percent);
    void scale_bottom_percent(float percent);

    void set_left_target(float percent, float max_change);
    void set_right_target(float percent, float max_change);
    void set_top_target(float percent, float max_change);
    void set_bottom_target(float percent, float max_change);

    void flip_h();
    void flip_v();

    void render();
    
    float target_left_crop = -1.0;
    float target_right_crop = -1.0;
    float target_top_crop = -1.0;
    float target_bottom_crop = -1.0;

    float target_left_max_change = 0.0;
    float target_right_max_change = 0.0;
    float target_top_max_change = 0.0;
    float target_bottom_max_change = 0.0;


    Shader* shader;
    u32 texture;
    GameTextureCoord tex_data[4];
    vec3 pos;
    vec3 rot;
    int orientation = GAME_TEXTURE_ORIENTATION_MIDDLE;

private:
    u32 VAO;
    u32 VBO;
    int width;
    int height;
};

class GameTexture{
public:
    SDL_Rect destRect;
    SDL_Rect srcRect;

    bool render();
    bool init(string sTexturePath, bool delay = true);
    
    void setScaleFactor(float fScaleFactor);

    void setHorizontalScaleFactor(float fScaleFactor);
    void setVerticalScaleFactor(float fScaleFactor);

    void setAnchorMode(int iMode);
    float getScaledWidth();
    float getScaledHeight();
    void setAlpha(Uint8 alpha);
    void clearTexture();
    bool bIsInitialized = false;
    void setPercent(float percent);
    void setTargetPercent(float percent, float rate = 0.2, int frames = 15);
    void changePercent(float rate = -1.0);
    void setFlip(int flip);
    int getFlipKind();
    void setDrainKind(int drain_kind);
private:
    float percent{ 0 };
    float target_percent{ -1 };
    float target_rate{ -1 };
    int target_frames{ 1 };
    int iAnchorMode = GAME_TEXTURE_ANCHOR_MODE_DEFAULT;
    float fVerticalScaleFactor = 1.0;
    float fHorizontalScaleFactor = 1.0;
    int flip{ TEXTURE_FLIP_KIND_NONE }; 
    int drain_kind{ METER_DRAIN_KIND_NONE };
    
    SDL_Texture *pTexture;
};
