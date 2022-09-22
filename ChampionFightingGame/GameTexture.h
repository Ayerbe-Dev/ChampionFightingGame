#pragma once
#include <vector>
#include <iostream>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "TextureCoord.h"
#include "Font.h"

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

class GameTexture {
public:
    GameTexture();
    GameTexture(std::string path);
    GameTexture(Font font, std::string text, glm::vec4 rgba, float border_x = 0.0, float border_y = 0.0);
    GameTexture(const GameTexture& that);

    void init(std::string path);
    void init(GLuint texture, int width = -1, int height = -1);
    void init(Font font, std::string text, glm::vec4 rgba, float border_x = 0.0, float border_y = 0.0);
    void destroy();
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
    std::string get_text();

    void set_left_target(float percent, float frames);
    void set_right_target(float percent, float frames);
    void set_top_target(float percent, float frames);
    void set_bottom_target(float percent, float frames);

    void set_target_pos(glm::vec3 target, float frames);

    void add_alpha(unsigned char alpha);
    void set_alpha(unsigned char alpha);

    void flip_h();
    void flip_v();
    void reorient();

    void process();
    void prepare_render();

    void render();
    void render_prepared();

    void load_spritesheet(std::string spritesheet_dir);
    void set_sprite(int section);

    void update_text(Font font, std::string text, glm::vec4 rgba, float border_x = 0.0, float border_y = 0.0);
    void update_buffer_data();
    
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
    glm::vec3 pos = glm::vec3(0.0);
    glm::vec3 rot = glm::vec3(0.0);
    int orientation = GAME_TEXTURE_ORIENTATION_MIDDLE;
    std::vector<glm::vec2> spritesheet[4];
    bool loaded = false;
    bool using_resource = false;
    std::string name = "";
private:
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


    std::string text;
    glm::mat4 matrix;
    bool depth;
};