#include "ScreenTexture.h"
#include "WindowConstants.h"
#include "ShaderManager.h"

ScreenTexture::ScreenTexture() {
#ifndef TEX_IMPL_VULKAN
	this->VAO = 0;
	this->VBO = 0;
	this->tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	this->tex_data[TEX_COORD_BOTTOM_LEFT2] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	this->tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	this->tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	this->tex_data[TEX_COORD_TOP_RIGHT2] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	this->tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 6; i++) {
		this->tex_accessor[i] = &this->tex_data[i];
	}
#endif
	this->shader = nullptr;
	this->path = "";
	this->screen_orientation = TEXTURE_MID;
	this->texture_orientation = TEXTURE_MID;
	this->pos = glm::vec3(0.0);
	this->rot = glm::vec3(0.0);
	this->right_edge_scale = 100.0;
	this->right_edge_crop = 100.0;
	this->left_edge_scale = 100.0;
	this->left_edge_crop = 100.0;
	this->top_edge_scale = 100.0;
	this->top_edge_crop = 100.0;
	this->bottom_edge_scale = 100.0;
	this->bottom_edge_crop = 100.0;
	this->width = 0;
	this->width_scale = 1.0;
	this->height = 0;
	this->height_scale = 1.0;
	this->alpha = 255;
	this->colormod = glm::vec3(0.0);
	this->h_flipped = false;
	this->v_flipped = false;
	this->sprite = 0;
	this->loaded = false;
}

ScreenTexture::ScreenTexture(std::string path) {
#ifndef TEX_IMPL_VULKAN
	this->VAO = 0;
	this->VBO = 0;
	this->tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	this->tex_data[TEX_COORD_BOTTOM_LEFT2] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	this->tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	this->tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	this->tex_data[TEX_COORD_TOP_RIGHT2] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	this->tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 6; i++) {
		this->tex_accessor[i] = &this->tex_data[i];
	}
#endif
	this->shader = nullptr;
	this->path = "";
	this->screen_orientation = TEXTURE_MID;
	this->texture_orientation = TEXTURE_MID;
	this->pos = glm::vec3(0.0);
	this->rot = glm::vec3(0.0);
	this->right_edge_scale = 100.0;
	this->right_edge_crop = 100.0;
	this->left_edge_scale = 100.0;
	this->left_edge_crop = 100.0;
	this->top_edge_scale = 100.0;
	this->top_edge_crop = 100.0;
	this->bottom_edge_scale = 100.0;
	this->bottom_edge_crop = 100.0;
	this->width = 0;
	this->width_scale = 1.0;
	this->height = 0;
	this->height_scale = 1.0;
	this->alpha = 255;
	this->colormod = glm::vec3(0.0);
	this->h_flipped = false;
	this->v_flipped = false;
	this->sprite = 0;
	this->loaded = false;
	init(path);
}

ScreenTexture::ScreenTexture(TEX_DATA_MEMBER_TYPE texture, int width, int height) {
#ifndef TEX_IMPL_VULKAN
	this->VAO = 0;
	this->VBO = 0;
	this->tex_data[TEX_COORD_BOTTOM_LEFT] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	this->tex_data[TEX_COORD_BOTTOM_LEFT2] = { glm::vec3(-1.0, -1.0, 0.0), glm::vec2(0.0, 0.0) };
	this->tex_data[TEX_COORD_BOTTOM_RIGHT] = { glm::vec3(1.0, -1.0, 0.0), glm::vec2(1.0, 0.0) };
	this->tex_data[TEX_COORD_TOP_RIGHT] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	this->tex_data[TEX_COORD_TOP_RIGHT2] = { glm::vec3(1.0, 1.0, 0.0), glm::vec2(1.0, 1.0) };
	this->tex_data[TEX_COORD_TOP_LEFT] = { glm::vec3(-1.0, 1.0, 0.0), glm::vec2(0.0, 1.0) };
	for (int i = 0; i < 6; i++) {
		this->tex_accessor[i] = &this->tex_data[i];
	}
#endif
	this->shader = nullptr;
	this->path = "";
	this->screen_orientation = TEXTURE_MID;
	this->texture_orientation = TEXTURE_MID;
	this->pos = glm::vec3(0.0);
	this->rot = glm::vec3(0.0);
	this->right_edge_scale = 100.0;
	this->right_edge_crop = 100.0;
	this->left_edge_scale = 100.0;
	this->left_edge_crop = 100.0;
	this->top_edge_scale = 100.0;
	this->top_edge_crop = 100.0;
	this->bottom_edge_scale = 100.0;
	this->bottom_edge_crop = 100.0;
	this->width = 0;
	this->width_scale = 1.0;
	this->height = 0;
	this->height_scale = 1.0;
	this->alpha = 255;
	this->colormod = glm::vec3(0.0);
	this->h_flipped = false;
	this->v_flipped = false;
	this->sprite = 0;
	this->loaded = false;
	this->init(texture, width, height);
}

ScreenTexture::ScreenTexture(ScreenTexture& other) {
#ifndef TEX_IMPL_VULKAN
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	for (int i = 0; i < 6; i++) {
		this->tex_data[i] = other.tex_data[i];
		this->tex_accessor[i] = &this->tex_data[i];
	}
#endif
	this->shader = other.shader;
	this->path = other.path;
	this->texture = other.texture;
	this->screen_orientation = other.screen_orientation;
	this->texture_orientation = other.texture_orientation;
	this->pos = other.pos;
	this->rot = other.rot;
	this->right_edge_scale = other.right_edge_scale;
	this->right_edge_crop = other.right_edge_crop;
	this->left_edge_scale = other.left_edge_scale;
	this->left_edge_crop = other.left_edge_crop;
	this->top_edge_scale = other.top_edge_scale;
	this->top_edge_crop = other.top_edge_crop;
	this->bottom_edge_scale = other.bottom_edge_scale;
	this->bottom_edge_crop = other.bottom_edge_crop;
	this->width = other.width;
	this->width_scale = other.width_scale;
	this->height = other.height;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->sprite = other.sprite;
	this->loaded = false;
}

ScreenTexture::ScreenTexture(ScreenTexture&& other) noexcept {
#ifndef TEX_IMPL_VULKAN
	this->VAO = other.VAO;
	this->VBO = other.VBO;
	for (int i = 0; i < 6; i++) {
		this->tex_data[i] = other.tex_data[i];
		this->tex_accessor[i] = &this->tex_data[i];
	}
#endif
	this->shader = other.shader;
	this->path = other.path;
	this->texture = other.texture;
	this->screen_orientation = other.screen_orientation;
	this->texture_orientation = other.texture_orientation;
	this->pos = other.pos;
	this->rot = other.rot;
	this->right_edge_scale = other.right_edge_scale;
	this->right_edge_crop = other.right_edge_crop;
	this->left_edge_scale = other.left_edge_scale;
	this->left_edge_crop = other.left_edge_crop;
	this->top_edge_scale = other.top_edge_scale;
	this->top_edge_crop = other.top_edge_crop;
	this->bottom_edge_scale = other.bottom_edge_scale;
	this->bottom_edge_crop = other.bottom_edge_crop;
	this->width = other.width;
	this->width_scale = other.width_scale;
	this->height = other.height;
	this->height_scale = other.height_scale;
	this->alpha = other.alpha;
	this->colormod = other.colormod;
	this->h_flipped = other.h_flipped;
	this->v_flipped = other.v_flipped;
	this->sprite = other.sprite;
	this->loaded = other.loaded;
	other.loaded = false;
}

ScreenTexture& ScreenTexture::operator=(ScreenTexture& other) {
	if (this != &other) {
#ifndef TEX_IMPL_VULKAN
		this->VAO = other.VAO;
		this->VBO = other.VBO;
		for (int i = 0; i < 6; i++) {
			this->tex_data[i] = other.tex_data[i];
			this->tex_accessor[i] = &this->tex_data[i];
		}
#endif
		this->shader = other.shader;
		this->path = other.path;
		this->texture = other.texture;
		this->screen_orientation = other.screen_orientation;
		this->texture_orientation = other.texture_orientation;
		this->pos = other.pos;
		this->rot = other.rot;
		this->right_edge_scale = other.right_edge_scale;
		this->right_edge_crop = other.right_edge_crop;
		this->left_edge_scale = other.left_edge_scale;
		this->left_edge_crop = other.left_edge_crop;
		this->top_edge_scale = other.top_edge_scale;
		this->top_edge_crop = other.top_edge_crop;
		this->bottom_edge_scale = other.bottom_edge_scale;
		this->bottom_edge_crop = other.bottom_edge_crop;
		this->width = other.width;
		this->width_scale = other.width_scale;
		this->height = other.height;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->sprite = other.sprite;
		this->loaded = false;
	}
	return *this;
}

ScreenTexture& ScreenTexture::operator=(ScreenTexture&& other) noexcept {
	if (this != &other) {
#ifndef TEX_IMPL_VULKAN
		this->VAO = other.VAO;
		this->VBO = other.VBO;
		for (int i = 0; i < 6; i++) {
			this->tex_data[i] = other.tex_data[i];
			this->tex_accessor[i] = &this->tex_data[i];
		}
#endif
		this->shader = other.shader;
		this->path = other.path;
		this->texture = other.texture;
		this->screen_orientation = other.screen_orientation;
		this->texture_orientation = other.texture_orientation;
		this->pos = other.pos;
		this->rot = other.rot;
		this->right_edge_scale = other.right_edge_scale;
		this->right_edge_crop = other.right_edge_crop;
		this->left_edge_scale = other.left_edge_scale;
		this->left_edge_crop = other.left_edge_crop;
		this->top_edge_scale = other.top_edge_scale;
		this->top_edge_crop = other.top_edge_crop;
		this->bottom_edge_scale = other.bottom_edge_scale;
		this->bottom_edge_crop = other.bottom_edge_crop;
		this->width = other.width;
		this->width_scale = other.width_scale;
		this->height = other.height;
		this->height_scale = other.height_scale;
		this->alpha = other.alpha;
		this->colormod = other.colormod;
		this->h_flipped = other.h_flipped;
		this->v_flipped = other.v_flipped;
		this->sprite = other.sprite;
		this->loaded = other.loaded;
		other.loaded = false;
	}
	return *this;
}

ScreenTexture::~ScreenTexture() {
	if (loaded) {
		destroy();
	}
}

void ScreenTexture::init(std::string path) {
	//Load Image, call some vulkan stuff probably
}

void ScreenTexture::init(TEX_DATA_MEMBER_TYPE texture, int width, int height) {

}

void ScreenTexture::destroy() {
	//Call some Vulkan stuff and tell the ResourceManager to unuse one instance of this texture info
}

ScreenTexture ScreenTexture::init_copy() {
	ScreenTexture ret;
	if (path != "") {
		ret.init(path);
	}
	else {
		ret.init(texture);
	}
	return ret;
}

std::string ScreenTexture::get_path() const {
	return path;
}

void ScreenTexture::set_orientation(int screen_orientation, int texture_orientation) {
	if (screen_orientation != SCREEN_TEXTURE_ORIENTATION_MAX) {
		this->screen_orientation = screen_orientation;
	}
	if (texture_orientation != SCREEN_TEXTURE_ORIENTATION_MAX) {
		this->texture_orientation = texture_orientation;
	}
}

int ScreenTexture::get_screen_orientation() const {
	return screen_orientation;
}

int ScreenTexture::get_texture_orientation() const {
	return texture_orientation;
}

void ScreenTexture::set_pos(glm::vec3 pos) {
	this->pos = pos;
}

void ScreenTexture::set_pos(glm::vec3 pos, int frames) {
	this->pos.set_target_val(pos, frames);
}

void ScreenTexture::add_pos(glm::vec3 pos) {
	this->pos += pos;
}

glm::vec3 ScreenTexture::get_pos() const {
	return pos.get_val();
}

void ScreenTexture::set_rot(glm::vec3 rot) {
	this->rot = rot;
}

void ScreenTexture::set_rot(glm::vec3 rot, int frames) {
	this->rot.set_target_val(rot, frames);
}

void ScreenTexture::add_rot(glm::vec3 rot) {
	this->rot += rot;
}

glm::vec3 ScreenTexture::get_rot() const {
	return rot.get_val();
}

void ScreenTexture::scale_right_edge(float percent) {
	right_edge_scale = percent;
}

void ScreenTexture::scale_right_edge(float percent, int frames) {
	right_edge_scale.set_target_val(percent, frames);
}

void ScreenTexture::crop_right_edge(float percent) {
	right_edge_crop = percent;
}

void ScreenTexture::crop_right_edge(float percent, int frames) {
	right_edge_crop.set_target_val(percent, frames);
}

void ScreenTexture::scale_left_edge(float percent) {
	left_edge_scale = percent;
}

void ScreenTexture::scale_left_edge(float percent, int frames) {
	left_edge_scale.set_target_val(percent, frames);
}

void ScreenTexture::crop_left_edge(float percent) {
	left_edge_crop = percent;
}

void ScreenTexture::crop_left_edge(float percent, int frames) {
	left_edge_crop.set_target_val(percent, frames);
}

void ScreenTexture::scale_top_edge(float percent) {
	top_edge_scale = percent;
}

void ScreenTexture::scale_top_edge(float percent, int frames) {
	top_edge_scale.set_target_val(percent, frames);
}

void ScreenTexture::crop_top_edge(float percent) {
	top_edge_crop = percent;
}

void ScreenTexture::crop_top_edge(float percent, int frames) {
	top_edge_crop.set_target_val(percent, frames);
}

void ScreenTexture::scale_bottom_edge(float percent) {
	bottom_edge_scale = percent;
}

void ScreenTexture::scale_bottom_edge(float percent, int frames) {
	bottom_edge_scale.set_target_val(percent, frames);
}

void ScreenTexture::crop_bottom_edge(float percent) {
	bottom_edge_crop = percent;
}

void ScreenTexture::crop_bottom_edge(float percent, int frames) {
	bottom_edge_crop.set_target_val(percent, frames);
}

void ScreenTexture::set_width(int new_width) {
	width = new_width;
}

void ScreenTexture::set_width(int new_width, int frames) {
	width.set_target_val(new_width, frames);
}

void ScreenTexture::set_width_scale(float scale) {
	width_scale = scale;
}

void ScreenTexture::set_width_scale(float scale, int frames) {
	width_scale.set_target_val(scale, frames);
}

void ScreenTexture::set_height(int new_height) {
	height = new_height;
}

void ScreenTexture::set_height(int new_height, int frames) {
	height.set_target_val(new_height, frames);
}

void ScreenTexture::set_height_scale(float scale) {
	height_scale = scale;
}

void ScreenTexture::set_height_scale(float scale, int frames) {
	height_scale.set_target_val(scale, frames);
}

int ScreenTexture::get_base_width() const {
	return width.get_val();
}

int ScreenTexture::get_base_height() const {
	return height.get_val();
}

float ScreenTexture::get_width_scale() const {
	return width_scale.get_val();
}

float ScreenTexture::get_height_scale() const {
	return height_scale.get_val();
}

float ScreenTexture::get_width() {
	return width_scale * width;
}

float ScreenTexture::get_height() {
	return height_scale * height;
}

void ScreenTexture::set_alpha(unsigned char alpha) {
	this->alpha = alpha;
}

void ScreenTexture::set_alpha(unsigned char alpha, int frames) {
	this->alpha.set_target_val(alpha, frames);
}

void ScreenTexture::add_alpha(unsigned char alpha) {
	this->alpha += alpha;
}

unsigned char ScreenTexture::get_alpha() const {
	return alpha.get_val();
}

void ScreenTexture::set_colormod(glm::vec3 color) {
	colormod = color;
}

void ScreenTexture::set_colormod(glm::vec3 color, int frames) {
	colormod.set_target_val(color, frames);
}

void ScreenTexture::add_colormod(glm::vec3 color) {
	colormod += color;
}

glm::vec3 ScreenTexture::get_colormod() const {
	return colormod.get_val();
}

void ScreenTexture::flip_h() {
	h_flipped = !h_flipped;
}

void ScreenTexture::set_h_flipped(bool h_flipped) {
	this->h_flipped = h_flipped;
}

bool ScreenTexture::is_h_flipped() const {
	return h_flipped;
}

void ScreenTexture::flip_v() {
	v_flipped = !v_flipped;
}

void ScreenTexture::set_v_flipped(bool v_flipped) {
	this->v_flipped = v_flipped;
}

bool ScreenTexture::is_v_flipped() const {
	return v_flipped;
}

void ScreenTexture::set_sprite(unsigned int sprite) {
	if (sprite < texture.size()) {
		this->sprite = sprite;
	}
}
	

void ScreenTexture::next_sprite() {
	if (sprite == texture.size() - 1) {
		sprite -= texture.size();
	}
	sprite++;
}

void ScreenTexture::prev_sprite() {
	if (sprite == 0) {
		sprite = texture.size();
	}
	sprite--;
}

unsigned int ScreenTexture::get_sprite() const {
	return sprite;
}

void ScreenTexture::render() {
	glm::vec3 render_pos = pos.get_val();

	if (!(screen_orientation & TEXTURE_H_MID)) {
		if (screen_orientation & TEXTURE_LEFT) {
			render_pos.x -= WINDOW_WIDTH;
		}
		else {
			render_pos *= -1.0;
			render_pos.x += WINDOW_WIDTH;
		}
	}
	if (!(screen_orientation & TEXTURE_V_MID)) {
		if (screen_orientation & TEXTURE_TOP) {
			render_pos *= -1.0;
			render_pos.y += WINDOW_HEIGHT;
		}
		else {
			render_pos.y -= WINDOW_HEIGHT;
		}
	}
	if (!(texture_orientation & TEXTURE_H_MID)) {
		if (texture_orientation & TEXTURE_LEFT) {
			render_pos.x += get_width() / 2;
		}
		else {
			render_pos.x -= get_width() / 2;
		}
	}
	if (!(texture_orientation & TEXTURE_V_MID)) {
		if (texture_orientation & TEXTURE_TOP) {
			render_pos.y -= get_height() / 2;
		}
		else {
			render_pos.y += get_height() / 2;
		}
	}
	//Vulkan things	
}