#pragma once
#include <SDL/SDL.h>
#include <vector>
#include <iostream>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "TextureCoord.h"

class Shader;
class ThreadManager;

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

class GameTexture {
public:
    GameTexture();
    GameTexture(std::string path);
    GameTexture(const GameTexture& that);
    ~GameTexture();
    void init(std::string path);
    void init(GLuint gl_tex_locatione);
    void destroy(bool destroy_texture = true);
    void set_pos(glm::vec3 pos);
    void add_pos(glm::vec3 pos);
    void set_rot(glm::vec3 rot);
    void add_rot(glm::vec3 rot);
    void set_orientation(int orientation);
    void attach_shader(Shader* shader);

    glm::vec3 get_pos_vacuum(GameTexture *that);

    void scale_left_percent(float percent, bool crop = true);
    void scale_right_percent(float percent, bool crop = true);
    void scale_top_percent(float percent, bool crop = true);
    void scale_bottom_percent(float percent, bool crop = true);
    void scale_all_percent(float percent, bool crop = true);

    void set_width(int new_width);
    void set_width_scale(float scale);
    void set_height(int new_height);
    void set_height_scale(float scale);
    void set_scale(float scale);
    int get_width();
    int get_height();

    void set_left_target(float percent, float frames);
    void set_right_target(float percent, float frames);
    void set_top_target(float percent, float frames);
    void set_bottom_target(float percent, float frames);

    void set_target_pos(glm::vec3 target, float frames);

    void set_alpha(unsigned char alpha);

    void flip_h(bool update = true);
    void flip_v(bool update = true);
    void reorient();

    void process();
    void prepare_render();

    void render();
    void render_prepared();

    void load_spritesheet(std::string spritesheet_dir);
    void set_sprite(int section);
    
    float target_left_crop = -2.0;
    float target_right_crop = -2.0;
    float target_top_crop = -2.0;
    float target_bottom_crop = -2.0;

    float target_left_frames = 0.0;
    float target_right_frames = 0.0;
    float target_top_frames = 0.0;
    float target_bottom_frames = 0.0;

    glm::vec3 target_pos = glm::vec3(0.0);
    glm::vec3 target_pos_frames = glm::vec3(0.0);
    
    unsigned char alpha = 255;

    Shader *shader;
    unsigned int texture;
    TextureCoord tex_data[4];
    TextureCoord* tex_accessor[4];
    glm::vec3 pos;
    glm::vec3 rot;
    int orientation = GAME_TEXTURE_ORIENTATION_MIDDLE;
    std::vector<glm::vec2> spritesheet[4];
private:
    void update_buffer_data();

    ThreadManager* thread_manager;

    unsigned int VAO;
    unsigned int VBO;
    float width;
    float height;

    float width_scale_mul;
    float height_scale_mul;
    float width_orientation;
    float height_orientation;

    bool h_flipped;
    bool v_flipped;

    std::string name;
    glm::mat4 matrix;
    bool depth;
};