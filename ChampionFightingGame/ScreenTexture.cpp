#include "ScreenTexture.h"
#include "utils.h"

ScreenTexture::ScreenTexture() {
	path = "";
	screen_orientation = TEXTURE_MID;
	texture_orientation = TEXTURE_MID;
	pos = glm::vec3(0.0);
	rot = glm::vec3(0.0);
	right_edge_scale = 0.0;
	right_edge_crop = 0.0;
	left_edge_scale = 0.0;
	left_edge_crop = 0.0;
	top_edge_scale = 0.0;
	top_edge_crop = 0.0;
	bottom_edge_scale = 0.0;
	bottom_edge_crop = 0.0;
	width = 0;
	width_scale = 1.0;
	height = 0;
	height_scale = 1.0;
	alpha = 255;
	colormod = glm::vec3(0.0);
	h_flipped = false;
	v_flipped = false;
	sprite = 0;
	loaded = false;
}

ScreenTexture::ScreenTexture(std::string path) {
	init(path);
}

void ScreenTexture::init(std::string path) {
	//Load Image, call some vulkan stuff probably
}

void ScreenTexture::destroy() {
	if (loaded) {
		//Call some Vulkan stuff and tell the ResourceManager to unuse one instance of this texture info
	}
}

ScreenTexture ScreenTexture::new_instance() {
	ScreenTexture ret;
	if (path == "") {
		//Call an init function for ret directly using texture info
	}
	else {
		ret.init(path);
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

void ScreenTexture::set_flip_h(bool flip_h) {
	h_flipped = flip_h;
}

bool ScreenTexture::get_flip_h() const {
	return h_flipped;
}

void ScreenTexture::flip_v() {
	v_flipped = !v_flipped;
}

void ScreenTexture::set_flip_v(bool flip_v) {
	v_flipped = flip_v;
}

bool ScreenTexture::get_flip_v() const {
	return v_flipped;
}

void ScreenTexture::set_sprite(int sprite) {
	this->sprite = sprite;
}

void ScreenTexture::next_sprite() {
	sprite++;
}

void ScreenTexture::prev_sprite() {
	sprite--;
}

int ScreenTexture::get_sprite() const {
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
		if (!(texture_orientation & TEXTURE_LEFT)) {
			render_pos.x += get_width() / 2;
		}
		else {
			render_pos.x -= get_width() / 2;
		}
	}
	if (!(texture_orientation & TEXTURE_V_MID)) {
		if (!(texture_orientation & TEXTURE_TOP)) {
			render_pos.y -= get_height() / 2;
		}
		else {
			render_pos.y += get_height() / 2;
		}
	}
	//Vulkan things	
}