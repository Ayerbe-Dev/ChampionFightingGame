#pragma once
#include <vector>
#include <iostream>
#include <glew/glew.h>
#include <glm/glm.hpp>
#include "TextureCoord.h"
#include "Font.h"
#include "TargetVar.h"

class Shader;

class GameTexture {
public:
    GameTexture();
    GameTexture(std::string path);
    GameTexture(GLuint texture, int width = -1, int height = -1);
    GameTexture(Font &font, std::string text, glm::vec4 rgba, glm::vec4 border_rgbs);
    GameTexture(const GameTexture& that);

    void init(std::string path);
    void init(GLuint texture, int width = -1, int height = -1);
    void init(Font &font, std::string text, glm::vec4 rgba, glm::vec4 border_rgbs);
    void destroy();
    void set_pos(glm::vec3 pos);
    void add_pos(glm::vec3 pos);
    void set_rot(glm::vec3 rot);
    void add_rot(glm::vec3 rot);
    void set_orientation(int orientation);
    void attach_shader(Shader* shader);
    void attach_anchor_pos(glm::vec3* pos);

    glm::vec3 get_pos_vacuum(GameTexture *that);

    void scale_top_left_percent(float percent, bool crop = true);
    void scale_bottom_left_percent(float percent, bool crop = true);
    void scale_top_right_percent(float percent, bool crop = true);
    void scale_bottom_right_percent(float percent, bool crop = true);
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
    int get_base_width();
    int get_base_height();
    float get_width_scale();
    float get_height_scale();
    float get_width();
    float get_height();
    std::string get_text();

    void set_top_left_target(float percent, float frames);
    void set_bottom_left_target(float percent, float frames);
    void set_top_right_target(float percent, float frames);
    void set_bottom_right_target(float percent, float frames);
    void set_left_target(float percent, float frames);
    void set_right_target(float percent, float frames);
    void set_top_target(float percent, float frames);
    void set_bottom_target(float percent, float frames);

    void set_target_pos(glm::vec3 target_pos, float frames);

    void add_alpha(unsigned char alpha);
    void set_alpha(unsigned char alpha);

    void add_colormod(glm::vec3 color);
    void set_colormod(glm::vec3 color);

    void flip_h();
    void flip_v();
    void reorient();

    void process();
    void prepare_render();

    void render();
    void render_prepared();

    void set_sprite(int sprite);
    void next_sprite();
    void prev_sprite();

    void update_text(Font &font, const std::string& text, glm::vec4 rgba, glm::vec4 border_rgbs);
    void update_text(const std::string& text);
    void update_buffer_data();

    float target_top_left_crop = -2.0;
    float target_bottom_left_crop = -2.0;
    float target_top_right_crop = -2.0;
    float target_bottom_right_crop = -2.0;
    float target_left_crop = -2.0;
    float target_right_crop = -2.0;
    float target_top_crop = -2.0;
    float target_bottom_crop = -2.0;

    float target_top_left_frames = 0.0;
    float target_bottom_left_frames = 0.0;
    float target_top_right_frames = 0.0;
    float target_bottom_right_frames = 0.0;
    float target_left_frames = 0.0;
    float target_right_frames = 0.0;
    float target_top_frames = 0.0;
    float target_bottom_frames = 0.0;
    
    glm::vec3 colormod = glm::vec3(0.0);
    TargetVar<unsigned char> alpha = 255;

    Shader *shader;
    std::vector<unsigned int> texture;
    TextureCoord tex_data[6];
    TextureCoord* tex_accessor[6];
    TargetVar<glm::vec3> pos = glm::vec3(0.0);
    glm::vec3* anchor_pos = nullptr;
    glm::vec3 rot = glm::vec3(0.0);
    int sprite_index;
    int orientation = SCREEN_TEXTURE_ORIENTATION_MIDDLE;
    bool loaded = false;
    bool using_resource = false;
    std::string name = "";
private:
    unsigned int VAO;
    unsigned int VBO;
    float width;
    float height;

    float width_scale;
    float height_scale;
    float width_orientation;
    float height_orientation;

    bool h_flipped;
    bool v_flipped;


    std::string text;
    Font* text_font;
    glm::vec4 text_rgba;
    glm::vec4 border_rgbs;
    glm::mat4 matrix;
    bool depth;
};